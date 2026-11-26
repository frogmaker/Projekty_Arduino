# Protokół danych i rejestracji

## UART źródło → Pico

| Parametr | Wartość domyślna |
|---|---|
| Interfejs | UART, jednokierunkowo TX źródła → RX Pico |
| UART Pico | Serial1 / UART0, RX **GP1**, fizyczny pin 2 |
| Szybkość | **115200 baud** |
| Ramka | **8N1**: 8 bitów danych, bez parzystości, 1 bit stopu |
| Dane | ASCII, 1–4 liczby w wierszu, stała liczba kolumn w rejestracji |
| Separator kolumn | Spacja, tabulator, przecinek albo średnik |
| Zakończenie | LF (`\n`) albo CRLF (`\r\n`) |
| Maksymalny wiersz | 192 bajty przed LF |
| Zapis wartości | Float32; liczby skończone, kropka dziesiętna, dopuszczalny zapis wykładniczy |

Przykład trzech kanałów, pokazany z tabulatorami jako `\t`:

```text
510\t512.35\t750\r\n
```

Źródło ma wysłać rzeczywiste znaki tabulatora i końca linii, nie dosłowny tekst `\t` i `\r\n`. Puste wiersze są ignorowane. Nagłówki, jednostki, etykiety, brakujące pola, `NaN`, `Inf`, liczby powodujące przepełnienie lub niedomiar przy konwersji Float32 oraz zmiana liczby kanałów są odrzucane. Przecinek jest separatorem kolumn — `1,25` oznacza dwie liczby, nie liczbę 1,25. Bardzo duże liczby całkowite nie zachowują w Float32 wszystkich cyfr.

Nie ma sumy kontrolnej ani automatycznej konwersji protokołów producentów. Wskaźniki błędów wykrywają wybrane problemy odbioru i formatu; nie dowodzą bezbłędności fizycznej transmisji.

## Początek i koniec okna

Polecenie z WWW rezerwuje rejestrację, po czym odbiornik czyści zaległy RX. Pierwszy LF służy do odzyskania granicy wiersza. Zapis obejmuje kompletne, poprawne wiersze zakończone w przedziale **od 0 do poniżej 5 000 000 µs** od startu okna na Pico.

Wiersz ukończony dokładnie na granicy 5 sekund nie jest dopisywany. Czas punktu to moment odbioru kompletnego wiersza przez Pico, a nie znacznik źródłowego ADC. Synchronizacja może pominąć pierwszą pełną linię. Przepływ danych ma być ciągły; nie ma komendy start/stop wysyłanej do źródła.

Bufor mieści **4096 rekordów**, każdy z czasem i maksymalnie czterema wartościami. Po jego zapełnieniu okno nadal kończy się po 5 sekundach, lecz kolejne poprawne rekordy są zliczane jako pominięte. Dane poprzedniej rejestracji są zastępowane po rozpoczęciu nowej; restart zasilania kasuje wynik.

## HTTP — lokalna strona Pico

Strona i API działają pod **http://192.168.4.1/** w sieci Pico-Sygnal. API zwraca JSON, z wyłączonym buforowaniem przeglądarki. To format odpowiedzi HTTP; w repozytorium nie są wymagane pliki `.json`.

### Rozpoczęcie

`POST /api/capture`

Odpowiedź **202** zawiera `capture_id` i stan `arming` lub `recording`. **409** oznacza trwającą rejestrację; GET nie uruchamia zapisu. Przycisk na stronie wywołuje ten endpoint.

### Stan

`GET /api/status`

| Pole | Znaczenie |
|---|---|
| `capture_id` | Identyfikator bieżącego zapisu; 0 przed pierwszym zapisem |
| `state` | `idle`, `arming`, `recording` albo `done` |
| `duration_ms` | 5000 |
| `elapsed_ms` | Postęp okna, od 0 do 5000 ms |
| `samples`, `channels` | Liczba zapisanych rekordów i kanałów |
| `max_samples` | 4096 |
| `invalid_lines` | Odrzucone wiersze i wykryte błędy transmisji |
| `dropped_samples` | Poprawne rekordy pominięte z powodu pełnego bufora |
| `uart_overflows` | Wykryte zdarzenia przepełnienia odbioru UART |
| `truncated` | Informacja o przepełnieniu bufora lub UART |

Pierwszy poprawny wiersz po synchronizacji ustala liczbę kanałów. Przy braku poprawnych danych może ona pozostać równa 0. `invalid_lines` także jest istotnym sygnałem niekompletności, niezależnie od flagi `truncated`.

### Dane po zakończeniu

`GET /api/data?id=N&offset=0&limit=256`

Odpowiedź zawiera `capture_id`, `state: "done"`, `channels`, `total`, `offset` i `points`. Każdy punkt ma postać:

```text
[czas_w_milisekundach, kanal_1, kanal_2, ...]
```

Strona zamienia milisekundy na sekundy osi X. Pobiera kolejne części wyniku, zwiększając `offset`, aż osiągnie `total`. Maksymalny `limit` to 256 rekordów. Niepoprawne argumenty dają **400**, a identyfikator starego lub niezakończonego zapisu — **409**. Klient powinien kontrolować identyfikator na każdej stronie danych, żeby nie połączyć różnych rejestracji.
