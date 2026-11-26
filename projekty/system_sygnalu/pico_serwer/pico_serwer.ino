/*
  System do obslugi i wyswietlania sygnalu.
  Raspberry Pi Pico 2 W, Arduino IDE + Arduino-Pico 6.3.2.
  UART input: Serial1 RX GP1, 115200 baud, 8N1, continuous numeric lines.
  Connect to Pico-Sygnal, password in config.h, open http://192.168.4.1/.
  Acquisition is owned by core 1. Wi-Fi and HTTP run only on core 0.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <atomic>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <hardware/uart.h>
#include <pico/mutex.h>

#include "CaptureEngine.h"
#include "config.h"
#include "webpage.h"

#if !defined(ARDUINO_RASPBERRY_PI_PICO_2W)
#error "Wybierz Raspberry Pi Pico 2W z pakietu Arduino-Pico Earle F. Philhower."
#endif

bool core1_separate_stack = true;
WebServer server(80);
const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_MASK(255, 255, 255, 0);

signal_capture::CaptureEngine capture;
mutex_t captureMutex;
std::atomic<bool> sharedReady(false);
bool apRunning = false;
uint32_t lastApAttempt = 0;
constexpr uint32_t AP_RETRY_MS = 20000;
constexpr size_t PAGE_LIMIT = 256;
// Global: 5 kB must not be allocated on the default core-0 stack.
signal_capture::Sample pageBuffer[PAGE_LIMIT];

void noCache() {
  server.sendHeader("Cache-Control", "no-store");
  server.sendHeader("X-Content-Type-Options", "nosniff");
}

void sendError(int status, const char* message) {
  noCache();
  String body = "{\"error\":\"";
  body += message;
  body += "\"}";
  server.send(status, "application/json; charset=utf-8", body);
}

bool readUnsignedArgument(const char* name, uint32_t& value) {
  if (!server.hasArg(name)) return false;
  const String input = server.arg(name);
  if (!input.length() || input.length() > 10) return false;
  uint32_t parsed = 0;
  for (size_t i = 0; i < input.length(); ++i) {
    if (input[i] < '0' || input[i] > '9') return false;
    const uint32_t digit = static_cast<uint32_t>(input[i] - '0');
    if (parsed > (UINT32_MAX - digit) / 10U) return false;
    parsed = parsed * 10U + digit;
  }
  value = parsed;
  return true;
}

void handleHome() {
  noCache();
  server.sendHeader("Content-Security-Policy", "default-src 'self'; script-src 'self' 'unsafe-inline'; style-src 'self' 'unsafe-inline'; connect-src 'self'; img-src 'self' data:; object-src 'none'; base-uri 'none'; frame-ancestors 'none'");
  server.send(200, "text/html; charset=utf-8", INDEX_HTML, sizeof(INDEX_HTML) - 1);
}

void handleCapture() {
  if (server.method() != HTTP_POST) {
    server.sendHeader("Allow", "POST");
    sendError(405, "Use POST to start capture");
    return;
  }
  mutex_enter_blocking(&captureMutex);
  const bool accepted = capture.request();
  const auto status = capture.snapshot(micros());
  mutex_exit(&captureMutex);
  if (!accepted) {
    sendError(409, "Capture already in progress");
    return;
  }
  String body = "{\"capture_id\":";
  body += status.captureId;
  body += ",\"state\":\"";
  body += signal_capture::stateName(status.state);
  body += "\"}";
  noCache();
  server.send(202, "application/json; charset=utf-8", body);
}

void handleStatus() {
  mutex_enter_blocking(&captureMutex);
  const auto status = capture.snapshot(micros());
  mutex_exit(&captureMutex);
  String body;
  if (!body.reserve(384)) { sendError(503, "Not enough memory"); return; }
  body = "{\"capture_id\":";
  body += status.captureId;
  body += ",\"state\":\"";
  body += signal_capture::stateName(status.state);
  body += "\",\"duration_ms\":5000,\"elapsed_ms\":";
  body += status.elapsedMs;
  body += ",\"samples\":";
  body += status.samples;
  body += ",\"channels\":";
  body += status.channels;
  body += ",\"max_samples\":";
  body += signal_capture::kMaxSamples;
  body += ",\"invalid_lines\":";
  body += status.invalidLines;
  body += ",\"dropped_samples\":";
  body += status.droppedSamples;
  body += ",\"uart_overflows\":";
  body += status.uartOverflows;
  body += ",\"truncated\":";
  body += status.truncated ? "true" : "false";
  body += '}';
  noCache();
  server.send(200, "application/json; charset=utf-8", body);
}

void handleData() {
  uint32_t id = 0, offset = 0, limit = PAGE_LIMIT;
  if (!readUnsignedArgument("id", id) || id == 0 ||
      (server.hasArg("offset") && !readUnsignedArgument("offset", offset)) ||
      (server.hasArg("limit") && !readUnsignedArgument("limit", limit)) ||
      limit == 0 || limit > PAGE_LIMIT) {
    sendError(400, "Invalid id, offset or limit");
    return;
  }
  signal_capture::Status status;
  size_t copied = 0;
  mutex_enter_blocking(&captureMutex);
  const bool available = capture.copyPage(id, offset, limit, pageBuffer, copied, status, micros());
  mutex_exit(&captureMutex);
  if (id != status.captureId || status.state != signal_capture::State::Done) {
    sendError(409, "Capture changed or is not finished");
    return;
  }
  if (!available) { sendError(400, "Offset exceeds sample count"); return; }

  String body;
  if (!body.reserve(160 + copied * 92)) { sendError(503, "Not enough memory"); return; }
  body = "{\"capture_id\":";
  body += status.captureId;
  body += ",\"state\":\"done\",\"channels\":";
  body += status.channels;
  body += ",\"total\":";
  body += status.samples;
  body += ",\"offset\":";
  body += offset;
  body += ",\"points\":[";
  char number[32];
  for (size_t i = 0; i < copied; ++i) {
    if (i) body += ',';
    body += '[';
    std::snprintf(number, sizeof(number), "%.3f", static_cast<double>(pageBuffer[i].timeUs) / 1000.0);
    body += number;
    for (uint8_t channel = 0; channel < status.channels; ++channel) {
      body += ',';
      // 9 significant digits preserve a finite float32 value on round trip.
      std::snprintf(number, sizeof(number), "%.9g", static_cast<double>(pageBuffer[i].values[channel]));
      body += number;
    }
    body += ']';
  }
  body += "]}";
  noCache();
  server.send(200, "application/json; charset=utf-8", body);
}

void configureRoutes() {
  server.on("/", HTTP_GET, handleHome);
  server.on("/api/capture", HTTP_ANY, handleCapture);
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/data", HTTP_GET, handleData);
  server.on("/favicon.ico", HTTP_GET, []() { server.send(204); });
  server.onNotFound([]() { sendError(404, "Not found"); });
}

void startAccessPoint() {
  lastApAttempt = millis();
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAPConfig(AP_IP, AP_IP, AP_MASK)) return;
  if (WiFi.beginAP(AP_SSID, AP_PASSWORD, AP_CHANNEL) != WL_CONNECTED) return;
  server.begin();
  apRunning = true;
  Serial.print("Siec: "); Serial.println(AP_SSID);
  Serial.print("Strona: http://"); Serial.println(WiFi.softAPIP());
}

void setup() {
  mutex_init(&captureMutex);
  sharedReady.store(true, std::memory_order_release);
  Serial.begin(115200);
  configureRoutes();
  startAccessPoint();
}

void loop() {
  if (apRunning && WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  } else {
    if (apRunning) { server.stop(); WiFi.disconnectAP(); apRunning = false; }
    if (uint32_t(millis() - lastApAttempt) >= AP_RETRY_MS) startAccessPoint();
  }
  delay(1);
}

void setup1() {
  while (!sharedReady.load(std::memory_order_acquire)) delay(1);
  Serial1.setRX(SOURCE_RX_PIN);
  Serial1.setTX(SOURCE_TX_PIN);
  Serial1.setFIFOSize(UART_RX_BUFFER);
  Serial1.begin(SOURCE_BAUD, SERIAL_8N1);
}

void loop1() {
  mutex_enter_blocking(&captureMutex);
  const auto state = capture.snapshot(micros()).state;
  mutex_exit(&captureMutex);
  if (state == signal_capture::State::Arming) {
    // Never include the bytes accumulated before the capture command. A
    // bounded pass also avoids an endless arming loop with a faulty sender.
    size_t budget = UART_RX_BUFFER * 2;
    while (budget-- && Serial1.available() > 0) Serial1.read();
    Serial1.overflow();
    uart_get_hw(uart0)->rsr = 0;
    if (Serial1.available() == 0) {
      mutex_enter_blocking(&captureMutex);
      capture.start(micros());
      mutex_exit(&captureMutex);
    }
  }

  const bool queueOverflow = Serial1.overflow();
  const uint32_t uartErrors = uart_get_hw(uart0)->rsr & 0x0f;
  if (uartErrors) uart_get_hw(uart0)->rsr = 0;
  if (queueOverflow || uartErrors) {
    mutex_enter_blocking(&captureMutex);
    if (queueOverflow || (uartErrors & 0x08)) capture.uartOverflow();
    else capture.transportError();
    mutex_exit(&captureMutex);
  }

  size_t budget = 1024;
  while (budget-- && Serial1.available() > 0) {
    const int byte = Serial1.read();
    if (byte < 0) break;
    const uint32_t now = micros();
    mutex_enter_blocking(&captureMutex);
    capture.feed(static_cast<char>(byte), now);
    mutex_exit(&captureMutex);
  }
  mutex_enter_blocking(&captureMutex);
  capture.tick(micros());
  mutex_exit(&captureMutex);
  delayMicroseconds(40);
}
