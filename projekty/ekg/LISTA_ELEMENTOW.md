# Lista elementów — projekt EKG

Zestaw dla niezmienionego szkicu `ekg_test4_ino/ekg_test4_ino.ino` i schematu z tego projektu.

| Element | Ilość | Co przygotować |
| --- | ---: | --- |
| **Arduino UNO R3, ATmega328P, 5 V** | 1 szt. | Płytka ze złączami zgodnymi z UNO R3 i portem USB-B; dla tej wersji kodu wybierz w IDE Arduino Uno. |
| **[Olimex SHIELD-EKG-EMG](https://www.olimex.com/Products/Duino/Shields/SHIELD-EKG-EMG/)** | 1 szt. | Kompletna nakładka ze złączami i zworkami, montowana bezpośrednio na listwach UNO. W tym projekcie zasilanie ustawione na 5 V. |
| **[Olimex SHIELD-EKG-EMG-PRO](https://www.olimex.com/Products/Duino/Shields/SHIELD-EKG-EMG-PRO/)** | 1 szt. | Kabel do nakładki: jack 3,5 mm oraz trzy końcówki zatrzaskowe L, R i DRL. |
| **[Elektrody żelowe ECG-GEL-ELECTRODE](https://www.olimex.com/Products/Modules/Biofeedback/ECG-GEL-ELECTRODE/)** lub kompatybilne z zatrzaskami przewodu PRO | 3 szt. | Trzy elektrody używane jednocześnie: L, R i DRL. Przy zakupie opakowania sprawdź liczbę sztuk. |
| **Przewód USB A–B do transmisji danych** | 1 szt. | Do gniazda USB-B UNO; końcówkę po stronie laptopa dopasuj do jego portu, np. USB-C–B dla USB-C. |
| **Laptop z Arduino IDE** | 1 szt. | Praca z baterii; w IDE pakiet Arduino AVR Boards i Serial Plotter, 115200 baud. |

**Ważne przy zamawianiu:** SHIELD-EKG-EMG to dodatkowa płytka, a SHIELD-EKG-EMG-PRO to przewód. Są to różne produkty. Producent podaje, że **do przewodu PRO nie dołącza elektrod żelowych** — należy przygotować je oddzielnie.

## Co robi dodatkowa płytka?

Olimex SHIELD-EKG-EMG zawiera analogowy wzmacniacz i filtry. Przetwarza niewielki sygnał różnicowy z elektrod na sygnał analogowy, który w tym projekcie trafia na **A0**. Dopiero przetwornik ADC w UNO zamienia go na liczby; program wykonuje dalszą filtrację i wysyła dane przez USB. Taki podział potwierdza [opis producenta nakładki](https://www.olimex.com/Products/Duino/Shields/SHIELD-EKG-EMG/).

Nakładka łączy się z UNO przez listwy. Do pokazanego układu nie trzeba dokładać płytki stykowej, przewodów Dupont, zewnętrznego wzmacniacza, luźnych rezystorów ani kondensatorów. Nie jest potrzebny osobny zasilacz.

## Warunki montażu tego wariantu

- Zworki: **5V**, **AIN_SEL 1–2 / A0**, **REF_E otwarta**, **CAL otwarta**. Ustaw je zgodnie z nadrukami i instrukcją konkretnej rewizji nakładki.
- Dla przewodu PRO: **L/czerwony — lewa ręka, R/biały — prawa ręka, DRL/czarny — prawa noga**. DRL nie łącz bezpośrednio z GND Arduino.
- Przy elektrodach na skórze laptop pracuje wyłącznie na baterii, bez ładowarki, stacji dokującej i pozostałych kabli. Jest to projekt edukacyjny, bez walidacji medycznej.

Schemat, uzasadnienie ustawień i ograniczenia: [README.md](README.md). Dokumentacja pierwotna: [ZRODLA.md](ZRODLA.md).
