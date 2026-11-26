#pragma once

// Local access point. Choose another password before sharing a real setup.
constexpr char AP_SSID[] = "Pico-Sygnal";
constexpr char AP_PASSWORD[] = "12341234";
constexpr int AP_CHANNEL = 6;

constexpr unsigned long SOURCE_BAUD = 115200;
constexpr int SOURCE_RX_PIN = 1;  // GP1, physical pin 2, UART0 RX / Serial1
constexpr int SOURCE_TX_PIN = 0;  // GP0 is configured, but leave it unconnected
constexpr size_t UART_RX_BUFFER = 8192;
