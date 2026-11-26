# Jak przygotować urządzenie nadawcze

System przyjmuje sygnał jako **liczby wysyłane przez UART**, a nie napięcie analogowe. Urządzenie źródłowe wykonuje pomiar lub obliczenia i wysyła wynik. Serial Monitor na komputerze jest odbiornikiem tego strumienia; Pico staje się drugim odbiornikiem danych.

## Co trzeba zrobić w urządzeniu

1. Udostępnić sprzętowy port **UART TX**, domyślnie **115200 baud, 8N1**. Jeśli źródło ma inną stałą prędkość, ustaw tę samą w `SOURCE_BAUD` w [config.h](pico_serwer/config.h).
2. Stale wysyłać od **1 do 4 liczb w wierszu**. Kolumny mogą być rozdzielane tabulatorem, spacją, przecinkiem lub średnikiem. Stosuj kropkę dziesiętną i zakończenie LF albo CRLF.
3. Zachować tę samą liczbę kanałów podczas pojedynczej rejestracji. Nie dodawać do strumienia opisów typu `Temperatura:`, jednostek, komunikatów debugowania ani nagłówków.
4. Dopasować elektrycznie TX do Pico i połączyć GND, według [schematu](schematy/schemat_elektryczny.png). Pierwsza poprawna linia po synchronizacji ustala liczbę kanałów.

Przykłady poprawnych wierszy — każdy jako osobny wariant źródła:

```text
512
```

```text
21.75;48.2
```

```text
510 512.35 750
```

Nie dodawaj osobnej kolumny czasu, jeśli chcesz tylko czas na osi X: Pico nadaje własny znacznik czasu odbioru. Kolumnę z czasem wysłaną przez źródło potraktuje jak kolejny kanał, a nie specjalne pole protokołu.

## Arduino UNO / klasyczne Nano

Na płytkach z ATmega328P `Serial` korzysta z UART: **D1 to TX**, D0 to RX. W programie uruchom:

```cpp
Serial.begin(115200);
```

Po przygotowaniu wartości nadaj wiersz, np.:

```cpp
Serial.print(wartosc1, 3);
Serial.print('\t');
Serial.println(wartosc2, 3);
```

To fragment własnego programu — `wartosc1` i `wartosc2` muszą zawierać Twoje pomiary. Gotowe, kompletne przykłady:

- [Nadajnik testowy](przyklady/nadajnik_testowy/nadajnik_testowy.ino): dwie sinusoidy, około 100 wierszy/s; **dane syntetyczne, nie EKG**.
- [Nadajnik analogowy](przyklady/nadajnik_analogowy/nadajnik_analogowy.ino): odczyt A0 w zakresie 0–5 V, około 100 wierszy/s. Wysyła jednostki ADC 0–1023. Nie podłączaj elektrod bezpośrednio do A0.

Połącz **D1/TX przez pokazany dzielnik 3×1 kΩ z GP1 Pico**, a GND ze wspólną masą. Nie jest potrzebny powrót danych z Pico do źródła. W zwykłym teście bez elektrod USB UNO może równolegle pokazywać ten sam strumień w Serial Monitorze. Przy EKG z elektrodami obowiązuje zasilanie i odłączenie kabli opisane w [README](README.md).

## Płytki z natywnym USB, np. drugie Pico

Nazwa `Serial` nie zawsze oznacza fizyczne TX/RX. W Arduino-Pico jest to port USB CDC. Do wysłania danych z drugiego Pico przez UART użyj **Serial1**, np. w Arduino-Pico:

```cpp
// W setup() urzadzenia NADAWCZEGO:
Serial1.setTX(0);   // GP0: TX UART0
Serial1.setRX(1);   // GP1: wejscie RX, nie musi byc polaczone
Serial1.begin(115200);

// Przy wysylaniu gotowej liczby:
Serial1.println(wartosc);
```

Połączenie dwóch Pico z logiką 3,3 V: **GP0/TX źródła → GP1/RX odbiornika**, GND → GND, bez dzielnika z wariantu UNO 5 V. Numery portów i pinów innych platform dobierz z ich dokumentacji. Samo `Serial.println()` wysyłające tylko przez USB nie kieruje automatycznie danych na UART. Źródło powinno uruchamiać nadawanie samodzielnie, bez oczekiwania na otwarcie monitora USB na komputerze.

## Inne typy interfejsów

| Wyjście urządzenia | Co jest potrzebne |
|---|---|
| UART z logiką 3,3 V | TX → GP1, wspólna masa i zgodny format danych. |
| UART z logiką 5 V, np. UNO | Dopasowanie poziomów, np. dzielnik z tego projektu. |
| USB CDC, USB–UART lub port USB producenta | Zmiana programu źródła na sprzętowy UART albo dodatkowy pośrednik z funkcją USB host. Ten projekt nie realizuje USB hosta. |
| RS‑232 | Właściwy transceiver RS‑232/UART; nie podłączaj linii RS‑232 wprost do GPIO. |
| RS‑485 | Właściwy transceiver i obsługa protokołu urządzenia; samo złącze A/B nie daje jeszcze opisanych wierszy liczb. |
| Sygnał analogowy | Przetwornik ADC i nadajnik, który zamieni pomiar na dane UART. |

Uniwersalność oznacza wspólny sposób przekazywania i wyświetlania liczb. Nie oznacza zgodności z każdym interfejsem, napięciem i szybkością bez adapterów lub zmiany programu źródła.

## Istniejące EKG z repozytorium

[Oryginalny szkic EKG](https://github.com/frogmaker/Projekty_Arduino/blob/main/projekty/ekg/ekg_test4_ino/ekg_test4_ino.ino) ma `Serial.begin(115200)` i już wysyła trzy liczby rozdzielone tabulatorami. Na UNO można wykorzystać ten program bez zmiany formatu: dane z D1/TX przechodzą przez dzielnik do Pico. Układ pomiarowy i zworki muszą pozostać zgodne z [instrukcją EKG](https://github.com/frogmaker/Projekty_Arduino/blob/main/projekty/ekg/README.md).

Pico pokaże trzy ogólne kanały: RAW, wynik filtracji +512 oraz marker. Nie przeprowadza dodatkowej analizy medycznej i nie przelicza tych liczb automatycznie na napięcie na elektrodach.

Źródła: [Arduino UNO — pinout](https://docs.arduino.cc/resources/pinouts/A000066-full-pinout.pdf), [Serial w Arduino-Pico](https://arduino-pico.readthedocs.io/en/latest/serial.html), [przykładowy tor EKG Olimex](https://www.olimex.com/Products/Duino/Shields/SHIELD-EKG-EMG/).
