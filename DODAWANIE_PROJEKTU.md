# Dodawanie kolejnego projektu

1. Skopiuj cały folder `szablon_projektu` do `projekty/<nazwa>`, np. `projekty/lampka_nocna`. Używaj krótkich nazw bez spacji i polskich znaków.
2. W kopii zmień nazwę folderu `nazwa_projektu` i pliku `nazwa_projektu.ino` na tę samą nazwę, np. `lampka_nocna/lampka_nocna.ino`. Arduino IDE wymaga zgodności nazwy głównego szkicu z nazwą jego folderu.
3. Uzupełnij README projektu: cel, płytkę, napięcia, listę elementów, połączenia i uruchomienie. Podaj wymagane biblioteki oraz ustawienia IDE, jeśli są potrzebne.
4. Umieść schematy w `schematy/`, a rzeczywiste przykłady lub wyraźnie opisane dane syntetyczne w `przyklady/`. Popraw odnośniki w README na nazwy dodanych plików.
5. Sprawdź kod oraz połączenia. W opisie rozróżnij kompilację, symulację i próbę na fizycznym sprzęcie. Dodaj źródła parametrów części, jeżeli projekt opiera się na ich konkretnych właściwościach.
6. Dopisz jeden wiersz do tabeli w [głównym README](README.md), z odnośnikiem do README nowego projektu.

Docelowo każdy projekt jest samodzielny: jego kod, dokumentacja i schematy znajdują się w jednym folderze. Możesz dodać np. `PROTOKOL.md` lub folder `komputer/`, kiedy wymaga tego projekt; opisz je w jego README.

[← Katalog projektów](README.md)
