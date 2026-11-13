/*
  Jednokanałowy surowy sEMG: Nano/Uno ATmega328P, 16 MHz, logika 5 V.
  BioAmp BisCute: VCC -> 5V, GND -> GND, OUT -> A0.
  Elektrody IN-/IN+/REF podłączaj WYŁĄCZNIE do modułu BioAmp.

  Podczas kontaktu ze skórą: laptop wyłącznie na baterii, odłączona
  ładowarka i inne przewody/peryferia. Instrukcja: ../README.md.

  Nie otwieraj Serial Plottera. To protokół BINARNY 115200 8N1.
  komputer/capture_emg.py wysyła R (start), następnie S (stop).
  FFT/MDF wykonuje komputer, nie Nano. Brak dodatkowych bibliotek.
*/
#include <Arduino.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "EmgProtocol.h"

#if !defined(__AVR_ATmega328P__)
#error "Ten szkic wymaga klasycznego Nano lub Uno z ATmega328P."
#endif
#if F_CPU != 16000000UL
#error "Szkic skonfigurowano dla zegara 16 MHz."
#endif

constexpr uint8_t STATUS_LED = 8;  // opcjonalnie: D8 -> 1k -> LED -> GND
constexpr uint8_t ADC_PRESCALER = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
constexpr uint16_t TIMER_TOP = F_CPU / 8UL / EmgProtocol::SAMPLE_RATE - 1;
static_assert(TIMER_TOP == 499, "Nieprawidlowy okres Timer1.");

// 256 pozycji, 255 użytecznych. Jeden element pozostaje pusty, aby odróżnić
// bufor pełny od pustego. Indeksy uint8_t zawijają się bez dzielenia.
volatile uint16_t ringBuffer[256];
volatile uint8_t writeIndex = 0;
volatile uint8_t readIndex = 0;
volatile bool collecting = false;
volatile bool overrun = false;
bool finalFramePending = false;
uint32_t emittedSamples = 0;

void stopHardware() {
  // Wywołuj z wyłączonymi przerwaniami (również z ADC ISR).
  TCCR1B = _BV(WGM12);  // zatrzymaj zegar Timer1
  ADCSRA = 0;          // przerwij również ewentualną konwersję w toku
  collecting = false;
}

ISR(ADC_vect) {
  const uint16_t value = ADC;
  // OCF1B jest flagą W1C. Musi wrócić do 0 przed kolejnym wyzwoleniem.
  // Nie używamy TIFR1 |= ..., ponieważ mogłoby to wyzerować inne flagi.
  TIFR1 = _BV(OCF1B);
  if (!collecting) return;
  const uint8_t next = static_cast<uint8_t>(writeIndex + 1);
  if (next == readIndex) {
    overrun = true;
    stopHardware();  // zatrzymaj zamiast ukrywać brakujące próbki
    return;
  }
  ringBuffer[writeIndex] = value;
  writeIndex = next;
}

void startAcquisition() {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    stopHardware();
    writeIndex = readIndex = 0;
    overrun = false;
    emittedSamples = 0;
    finalFramePending = true;

    // A0, napięcie odniesienia AVCC (~5 V). Kondensator AREF jest na Nano.
    ADMUX = _BV(REFS0);
    ADCSRB = 0;
    DIDR0 |= _BV(ADC0D);
    ADCSRA = _BV(ADEN) | ADC_PRESCALER;
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC)) {}  // pierwsza konwersja: rozgrzewka ADC
    (void)ADC;

    // Timer1 CTC: 16 MHz / 8 / (499 + 1) = nominalnie 4000 Hz.
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
    TCNT1 = 0;
    OCR1A = TIMER_TOP;
    OCR1B = TIMER_TOP / 2;
    TIFR1 = _BV(OCF1B) | _BV(OCF1A) | _BV(TOV1);
    ADCSRB = _BV(ADTS2) | _BV(ADTS0);  // źródło 101: Timer1 Compare B
    collecting = true;
    ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADIF) | ADC_PRESCALER;
    TCCR1B = _BV(WGM12) | _BV(CS11);
  }
}

void sendSamples(uint8_t count) {
  uint16_t samples[EmgProtocol::MAX_SAMPLES];
  uint16_t flags = 0;
  for (uint8_t i = 0; i < count; ++i) {
    // Producent nie nadpisze tej komórki, dopóki nie przesuniemy readIndex.
    const uint8_t index = readIndex;
    samples[i] = ringBuffer[index];
    readIndex = static_cast<uint8_t>(index + 1);
    if (samples[i] <= 1 || samples[i] >= 1022) flags |= EmgProtocol::CLIPPING;
  }
  uint8_t frame[EmgProtocol::MAX_FRAME_BYTES];
  const size_t bytes = EmgProtocol::encode(frame, samples, count, emittedSamples, flags);
  Serial.write(frame, bytes);  // przerwania ADC nadal działają podczas transmisji
  emittedSamples += count;
}

void sendFinalFrame() {
  uint8_t frame[EmgProtocol::MAX_FRAME_BYTES];
  const uint16_t flags = EmgProtocol::STOPPED | (overrun ? EmgProtocol::OVERRUN : 0);
  const size_t bytes = EmgProtocol::encode(frame, nullptr, 0, emittedSamples, flags);
  Serial.write(frame, bytes);
  finalFramePending = false;
}

void setup() {
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);
  pinMode(A0, INPUT);
  Serial.begin(115200);
  // Oczekiwanie na R z programu rejestrującego. Nie wysyłamy tekstu ASCII.
}

void loop() {
  while (Serial.available()) {
    const int command = Serial.read();
    if (command == 'R') startAcquisition();
    if (command == 'S') {
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { stopHardware(); }
    }
  }

  const uint8_t available = static_cast<uint8_t>(writeIndex - readIndex);
  if (available >= EmgProtocol::MAX_SAMPLES) {
    sendSamples(EmgProtocol::MAX_SAMPLES);
  } else if (!collecting && available) {
    sendSamples(available);
  } else if (!collecting && finalFramePending) {
    sendFinalFrame();
  }

  if (overrun) digitalWrite(STATUS_LED, (millis() / 200) % 2 ? HIGH : LOW);
  else digitalWrite(STATUS_LED, collecting ? HIGH : LOW);
}
