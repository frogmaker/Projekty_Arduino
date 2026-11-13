/*
   ============================================================
   Arduino UNO + OLIMEX SHIELD-EKG/EMG
   EKG + filtr + detekcja R-peak + BPM
   ============================================================

   CO ROBI PROGRAM:

   1. Odczytuje A0 z częstotliwością 256 Hz.
   2. Filtruje sygnał:
        - HIGH-PASS ~0.5 Hz
        - LOW-PASS  ~25 Hz
   3. Wykrywa potencjalne R-peak.
   4. Oblicza BPM na podstawie odstępów R-R.
   5. Do Serial Plottera wysyła:
        - RAW
        - FILTERED
        - R-PEAK

   WAŻNE:
   - Pomiar i detekcja odbywają się z częstotliwością 256 Hz.
   - Do Serial Plottera wysyłana jest co 4. próbka = 64 Hz.
   - Dzięki temu wykres przesuwa się wolniej i jest czytelniejszy.

   UWAGA:
   Projekt eksperymentalny/edukacyjny.
   Nie jest urządzeniem medycznym ani diagnostycznym.
*/


// ============================================================
// USTAWIENIA
// ============================================================

// Wejście analogowe Arduino.
// Przy odpowiednim ustawieniu AIN_SEL na shieldzie
// kanał 1 powinien być dostępny na A0.

const int ECG_PIN = A0;


// ============================================================
// CZĘSTOTLIWOŚĆ PRÓBKOWANIA
// ============================================================

// Wewnętrzne próbkowanie EKG.
// Oryginalny program Olimexa używał 256 Hz.

const float SAMPLE_RATE = 256.0;


// Czas jednej próbki.
//
// 1 / 256 sekundy = 3906,25 mikrosekundy

const unsigned long SAMPLE_PERIOD_US = 3906;


// ============================================================
// PARAMETRY FILTRÓW
// ============================================================

// Filtr HIGH-PASS.
//
// Usuwa bardzo wolne zmiany poziomu sygnału,
// np. dryf elektrod.

const float HP_CUTOFF = 0.5;


// Filtr LOW-PASS.
//
// Ogranicza szybkie zakłócenia.

const float LP_CUTOFF = 25.0;


// Współczynniki filtrów.
// Zostaną obliczone automatycznie w setup().

float hpAlpha;
float lpAlpha;


// ============================================================
// ZMIENNE FILTRU
// ============================================================

float previousInput = 0;

float highPass = 0;

float filtered = 0;


// ============================================================
// PARAMETRY DETEKCJI R-PEAK
// ============================================================

// Próg wykrywania szczytu.
//
// Jeżeli sygnał będzie miał zbyt małą amplitudę,
// trzeba będzie zmniejszyć tę wartość.
//
// Jeżeli będzie dużo fałszywych wykryć,
// trzeba będzie ją zwiększyć.

float peakThreshold = 20.0;


// Minimalny odstęp pomiędzy dwoma R-peak.
//
// 250 ms = maksymalnie około 240 BPM.
//
// Chroni przed wielokrotnym liczeniem tego samego
// szczytu.

const unsigned long MIN_RR_MS = 250;


// Maksymalny odstęp R-R, który wykorzystujemy
// do obliczenia BPM.
//
// 2000 ms = 30 BPM.

const unsigned long MAX_RR_MS = 2000;


// ============================================================
// ZMIENNE DETEKTORA R-PEAK
// ============================================================

// Czas ostatniego wykrytego R-peak.

unsigned long lastRPeakTime = 0;


// Czy znaleźliśmy już pierwszy R-peak?

bool havePreviousPeak = false;


// Aktualne BPM.

float bpm = 0;


// ============================================================
// ZMIENNE CZASOWE
// ============================================================

unsigned long lastSampleTime = 0;


// ============================================================
// DZIELNIK DANYCH DLA SERIAL PLOTTERA
// ============================================================
//
// Pomiar i detekcja nadal odbywają się 256 razy/s.
//
// Do Serial Plottera wysyłamy jednak tylko co 4. próbkę:
//
// 256 / 4 = 64 próbki/s
//
// Dzięki temu wykres jest mniej "ściśnięty".

byte plotDivider = 0;


// ============================================================
// SETUP
// ============================================================

void setup()
{
  // ----------------------------------------------------------
  // Uruchomienie komunikacji z komputerem.
  // ----------------------------------------------------------

  Serial.begin(115200);


  // ----------------------------------------------------------
  // Obliczenie współczynników filtrów.
  // ----------------------------------------------------------

  float dt = 1.0 / SAMPLE_RATE;


  // ----------------------------------------------------------
  // HIGH-PASS
  // ----------------------------------------------------------

  float hpRC =
    1.0 / (2.0 * PI * HP_CUTOFF);

  hpAlpha =
    hpRC / (hpRC + dt);


  // ----------------------------------------------------------
  // LOW-PASS
  // ----------------------------------------------------------

  float lpRC =
    1.0 / (2.0 * PI * LP_CUTOFF);

  lpAlpha =
    dt / (lpRC + dt);


  // ----------------------------------------------------------
  // A0 jako wejście.
  // ----------------------------------------------------------

  pinMode(ECG_PIN, INPUT);


  // Krótka chwila na uruchomienie portu.

  delay(500);
}


// ============================================================
// LOOP
// ============================================================

void loop()
{
  unsigned long currentTime = micros();


  // ----------------------------------------------------------
  // CZY CZAS NA KOLEJNĄ PRÓBKĘ?
  // ----------------------------------------------------------

  if (currentTime - lastSampleTime >= SAMPLE_PERIOD_US)
  {
    lastSampleTime += SAMPLE_PERIOD_US;


    // ========================================================
    // 1. ODCZYT ADC
    // ========================================================

    int rawADC = analogRead(ECG_PIN);

    float input = (float)rawADC;


    // ========================================================
    // 2. FILTR HIGH-PASS
    // ========================================================

    highPass =
      hpAlpha *
      (highPass + input - previousInput);

    previousInput = input;


    // ========================================================
    // 3. FILTR LOW-PASS
    // ========================================================

    filtered =
      filtered +
      lpAlpha * (highPass - filtered);


    // ========================================================
    // 4. DETEKCJA R-PEAK
    // ========================================================

    unsigned long nowMs = millis();


    // Domyślnie w tej próbce nie znaleziono R-peak.

    bool peakDetected = false;


    // --------------------------------------------------------
    // Czy filtrowany sygnał przekroczył próg?
    // --------------------------------------------------------

    if (filtered > peakThreshold)
    {
      unsigned long timeSinceLastPeak =
        nowMs - lastRPeakTime;


      // ------------------------------------------------------
      // PIERWSZY R-PEAK
      // ------------------------------------------------------

      if (!havePreviousPeak)
      {
        lastRPeakTime = nowMs;

        havePreviousPeak = true;

        peakDetected = true;
      }


      // ------------------------------------------------------
      // KOLEJNE R-PEAK
      // ------------------------------------------------------

      else if (timeSinceLastPeak >= MIN_RR_MS)
      {
        // Mamy odstęp R-R.

        if (timeSinceLastPeak <= MAX_RR_MS)
        {
          // --------------------------------------------------
          // OBLICZENIE BPM
          //
          // np.:
          //
          // R-R = 800 ms
          //
          // BPM = 60000 / 800
          //     = 75
          // --------------------------------------------------

          bpm =
            60000.0 /
            (float)timeSinceLastPeak;
        }


        // Zapamiętujemy czas aktualnego R-peak.

        lastRPeakTime = nowMs;

        peakDetected = true;
      }
    }


    // ========================================================
    // 5. WYSYŁANIE DANYCH DO SERIAL PLOTTER
    // ========================================================
    //
    // UWAGA:
    //
    // Pomiar odbywa się 256 Hz.
    //
    // Ale wykres otrzymuje tylko co 4. próbkę:
    //
    // 256 / 4 = 64 Hz
    //
    // To powoduje, że wykres przesuwa się wolniej.
    //
    // Wysyłamy trzy serie:
    //
    // 1. RAW
    // 2. FILTERED
    // 3. R-PEAK
    //
    // ========================================================

    plotDivider++;


    if (plotDivider >= 4)
    {
      plotDivider = 0;


      // ------------------------------------------------------
      // RAW
      // ------------------------------------------------------

      Serial.print(rawADC);

      Serial.print('\t');


      // ------------------------------------------------------
      // FILTERED
      //
      // Filtr jest wycentrowany wokół 0.
      //
      // Dodajemy 512, żeby był widoczny w okolicach
      // środka zakresu Arduino ADC.
      // ------------------------------------------------------

      Serial.print(filtered + 512);

      Serial.print('\t');


      // ------------------------------------------------------
      // R-PEAK
      //
      // Normalnie = 512
      //
      // W momencie wykrycia R-peak = 750
      //
      // Dzięki temu na wykresie zobaczymy,
      // które szczyty zostały rozpoznane.
      // ------------------------------------------------------

      if (peakDetected)
      {
        Serial.println(750);
      }
      else
      {
        Serial.println(512);
      }
    }


    // ========================================================
    // 6. BPM
    // ========================================================
    //
    // Aktualna wartość BPM jest przechowywana w zmiennej:
    //
    //     bpm
    //
    // Na razie NIE wysyłamy jej tutaj jako tekstu,
    // ponieważ Serial Plotter potraktowałby tekst jako
    // dodatkowe dane i popsułby wykres.
    //
    // Możemy w kolejnej wersji zrobić osobny sposób
    // wyświetlania BPM.
    // ========================================================
  }
}