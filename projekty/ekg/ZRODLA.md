# Źródła i zakres weryfikacji

Schematy i opis odnoszą się do **Arduino UNO R3 / ATmega328P oraz Olimex SHIELD-EKG-EMG**. Nie należy stosować ich bez sprawdzenia do innego modułu EKG, nakładki lub kabla elektrod.

| Źródło pierwotne | Co zweryfikowano |
| --- | --- |
| [Olimex — SHIELD-EKG-EMG](https://www.olimex.com/Products/Duino/Shields/SHIELD-EKG-EMG/) | Przeznaczenie nakładki, zgodność z platformą Arduino i dostępna dokumentacja. |
| [Instrukcja Olimex, rewizja E](https://www.olimex.com/Products/Duino/Shields/SHIELD-EKG-EMG/resources/SHIELD-EKG-EMG.pdf) | Ustawienia `3.3V/5V`, `AIN_SEL` 1–2/A0, `REF_E`, `CAL`, wybór D4/D9; zewnętrzne odniesienie około 3 V i ostrzeżenia dotyczące kontaktu ze skórą. Zobacz szczególnie rozdział 4 i opisy zworek. |
| [Schemat producenta, SHIELD-EKG-EMG rev. B](https://www.olimex.com/Products/Duino/Shields/SHIELD-EKG-EMG/resources/SHIELD-EKG-EMG-REV-B-SCHEMATIC.pdf) | Tor 5 V/GND/A0 przez listwy Arduino, połączenie odniesienia przez `REF_E`, polaryzacja toru około 1,5 V i odrębna funkcja DRL. |
| [Olimex — przewód SHIELD-EKG-EMG-PRO](https://www.olimex.com/Products/Duino/Shields/SHIELD-EKG-EMG-PRO/) | Kabel jack 3,5 mm z trzema zatrzaskami; kolory L/czerwony, R/biały, DRL/czarny. Producent wyraźnie podaje, że elektrody żelowe nie są dołączone. |
| [Olimex — ECG-GEL-ELECTRODE](https://www.olimex.com/Products/Modules/Biofeedback/ECG-GEL-ELECTRODE/) | Samoprzylepne elektrody żelowe wskazane przez producenta jako osobne wyposażenie do przewodu PRO. |
| [Arduino — dokumentacja `analogReference()`](https://docs.arduino.cc/language-reference/en/functions/analog-io/analogReference/) | Domyślne odniesienie 5 V na UNO; tryb `EXTERNAL`; zakaz podawania zewnętrznego napięcia na AREF przy aktywnym wewnętrznym odniesieniu. |
| [Arduino — źródło dokumentacji `analogReference()`](https://github.com/arduino/reference-en/blob/master/Language/Functions/Analog%20IO/analogReference.adoc) | Treść ostrzeżenia dotyczącego konfliktu źródeł AREF i ryzyka uszkodzenia mikrokontrolera. |

Warunek **laptop tylko na baterii, bez innych połączeń przewodowych** przy elektrodach na skórze przyjęto dla tego projektu, aby ograniczyć drogi połączenia z siecią elektryczną. Podobne zalecenie zespołu Olimex jest opisane na [forum producenta dla pokrewnego projektu EEG](https://www.olimex.com/forum/index.php?topic=205.0). Nie jest to certyfikacja bezpieczeństwa ani dowód zgodności tego układu z normą medyczną.
