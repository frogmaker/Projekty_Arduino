#pragma once
#include <stdint.h>
#include <stddef.h>

// Specyfikacja ramki: patrz ../PROTOKOL.md. Bez zależności od Arduino.
namespace EmgProtocol {
constexpr uint16_t SAMPLE_RATE = 4000;
constexpr uint8_t MAX_SAMPLES = 32;
constexpr size_t MAX_FRAME_BYTES = 14 + 2 * MAX_SAMPLES;
constexpr uint16_t OVERRUN = 1;
constexpr uint16_t CLIPPING = 2;
constexpr uint16_t STOPPED = 4;

inline void put16(uint8_t* out, uint16_t value) {
  out[0] = static_cast<uint8_t>(value);
  out[1] = static_cast<uint8_t>(value >> 8);
}

inline void put32(uint8_t* out, uint32_t value) {
  for (uint8_t i = 0; i < 4; ++i) out[i] = static_cast<uint8_t>(value >> (8 * i));
}

inline uint16_t crc16(const uint8_t* data, size_t size) {
  uint16_t crc = 0xFFFF;
  while (size--) {
    crc ^= static_cast<uint16_t>(*data++) << 8;
    for (uint8_t bit = 0; bit < 8; ++bit) {
      crc = (crc & 0x8000) ? static_cast<uint16_t>((crc << 1) ^ 0x1021)
                           : static_cast<uint16_t>(crc << 1);
    }
  }
  return crc;
}

inline size_t encode(uint8_t* out, const uint16_t* samples, uint8_t count,
                     uint32_t firstIndex, uint16_t flags) {
  if (count > MAX_SAMPLES) return 0;
  out[0] = 0xA5;
  out[1] = 0x5A;
  out[2] = 1;
  out[3] = count;
  put32(out + 4, firstIndex);
  put16(out + 8, flags);
  put16(out + 10, SAMPLE_RATE);
  for (uint8_t i = 0; i < count; ++i) put16(out + 12 + 2 * i, samples[i]);
  const size_t beforeCrc = 12 + 2 * count;
  put16(out + beforeCrc, crc16(out + 2, beforeCrc - 2));
  return beforeCrc + 2;
}
}
