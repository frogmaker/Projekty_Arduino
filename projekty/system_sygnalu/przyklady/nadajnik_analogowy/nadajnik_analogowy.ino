/*
  Minimalny przyklad zrodla: odczyt A0 Arduino Uno / Nano, okolo 100 Hz.
  Wejscie A0 musi otrzymywac napiecie w zakresie 0..5 V, wspolna masa.
  Serial wysyla liczby 0..1023, nie wolty. Nie podlaczaj elektrod do A0.
  D1 (TX) przez dzielnik 5 V -> 3,3 V do GP1 Pico.
*/
#include <Arduino.h>

const uint32_t SAMPLE_PERIOD_MS = 10;
uint32_t lastSampleAt = 0;

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  lastSampleAt = millis();
}

void loop() {
  const uint32_t now = millis();
  if (uint32_t(now - lastSampleAt) < SAMPLE_PERIOD_MS) return;
  lastSampleAt = now;
  Serial.println(analogRead(A0));
}
