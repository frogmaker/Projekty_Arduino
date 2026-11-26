/*
  Test transmisji: dwa SYNTETYCZNE kanaly, sinus i cosinus, okolo 100 Hz.
  To nie jest sygnal EKG ani pomiar z czujnika.
  Arduino Uno / klasyczne Nano: Serial = UART na D1 (TX), 115200, 8N1.
  D1 przez dzielnik 5 V -> 3,3 V do GP1 Pico, wspolna masa.
*/
#include <Arduino.h>
#include <math.h>

const uint32_t SAMPLE_PERIOD_MS = 10;
uint32_t lastSampleAt = 0;
uint8_t phase = 0;

void setup() {
  Serial.begin(115200);
  lastSampleAt = millis();
}

void loop() {
  const uint32_t now = millis();
  if (uint32_t(now - lastSampleAt) < SAMPLE_PERIOD_MS) return;
  lastSampleAt = now;

  const float angle = TWO_PI * phase / 100.0f;
  Serial.print(sinf(angle), 4);
  Serial.print('\t');
  Serial.println(cosf(angle), 4);
  phase = (phase + 1) % 100;
}
