# Protokół Nano ↔ komputer

Port szeregowy **115200 baud, 8N1**, jeden kanał A0, nominalnie **4000 próbek/s**, surowy ADC 10-bit. Szkic po resecie czeka na polecenie. Nie jest zgodny z Chords Web ani Arduino Serial Plotter.

- Komputer wysyła pojedynczy bajt ASCII **`R`**: rozpocznij nowy pomiar, wyzeruj bufor i indeksy. Restart w trakcie pomiaru rozpoczyna nową serię — nie łącz jej ze starą.
- Komputer wysyła pojedynczy bajt ASCII **`S`**: zatrzymaj pomiar, wyślij pozostałe próbki i ramkę końcową.
- Przepełnienie bufora zatrzymuje pomiar. Nano zachowuje wcześniejsze poprawne próbki, następnie wysyła ramkę końcową z błędem. Nie kontynuuje z pozornie regularnymi danymi po utracie próbki.

Wszystkie liczby wielobajtowe mają kolejność **little-endian**.

| Przesunięcie | Długość | Pole |
| --- | --- | --- |
| 0 | 2 | Synchronizacja `A5 5A` |
| 2 | 1 | Wersja `1` |
| 3 | 1 | Liczba próbek `N`, od 0 do 32 |
| 4 | 4 | Indeks pierwszej próbki `uint32`, od 0 |
| 8 | 2 | Flagi `uint16` |
| 10 | 2 | Częstotliwość `uint16`, wartość 4000 |
| 12 | 2 × N | Próbki `uint16`, wartości 0–1023 |
| 12 + 2 × N | 2 | CRC16 zapisane little-endian |

CRC16-CCITT-FALSE: wielomian `0x1021`, wartość początkowa `0xFFFF`, bez odbijania bitów, końcowy XOR = 0. Obejmuje bajty od wersji pod przesunięciem 2 do ostatniego bajtu próbek; nie obejmuje synchronizacji ani samego CRC. Przykład kontrolny algorytmu: ASCII `123456789` daje `0x29B1`.

Flagi: bit 0 (`1`) — **OVERRUN**, bit 1 (`2`) — **CLIPPING**, bit 2 (`4`) — **STOPPED**. CLIPPING oznacza co najmniej jedną próbkę ≤1 lub ≥1022 w ramce. Jest to wskaźnik zbliżenia do szyn ADC; nie wykrywa każdego możliwego nasycenia wewnątrz wzmacniacza. STOPPED i OVERRUN są przekazywane w ramce końcowej z `N = 0`. Jej indeks wskazuje liczbę wysłanych próbek.

Zwykła ramka ma 78 bajtów i 32 próbki. Przy 4 kHz daje to **9750 bajtów/s**. Łącze 115200 8N1 przenosi teoretycznie 11520 bajtów/s. Kontrola CRC i ciągłości indeksów wykrywa uszkodzenia i ubytki transmisji; nie zastępuje pomiarowego sprawdzenia sprzętu. Program na komputerze zachowuje informacje o jakości i nie powinien analizować okien przecinających luki danych.
