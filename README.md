# Projekty Arduino

Edukacyjne projekty Arduino: kod, schematy połączeń i instrukcje uruchomienia. Każdy projekt ma własny folder, opis sprzętu i materiały.

## Katalog projektów

| Projekt | Płytka | Materiały |
| --- | --- | --- |
| [EMG — rejestracja sygnału mięśniowego](projekty/emg/README.md) | Nano / Uno, ATmega328P, 5 V | Kod Arduino, protokół transmisji, schematy i przykład syntetyczny |
| [EKG — rejestracja z nakładką Olimex](projekty/ekg/README.md) | UNO R3 + SHIELD-EKG-EMG, 5 V | Oryginalny kod, analiza, schematy i dostarczony wynik JPG |

## Układ repozytorium

```text
projekty/
├── emg/                # rejestracja sygnału mięśniowego
└── ekg/
    ├── README.md
    ├── ANALIZA_KODU.md
    ├── ZRODLA.md
    ├── ekg_test4_ino/   # oryginalny szkic Arduino
    ├── schematy/        # montaż i połączenia: PNG/JPG/SVG/PDF
    └── przyklady/       # oryginalny wynik JPG
szablon_projektu/        # punkt wyjścia do następnego projektu
DODAWANIE_PROJEKTU.md
```

## Kolejny projekt

Skopiuj [szablon projektu](szablon_projektu/README.md) do nowego folderu w `projekty/`, uzupełnij jego materiały i dodaj wiersz do katalogu powyżej. Szczegóły: [dodawanie projektu krok po kroku](DODAWANIE_PROJEKTU.md).

Wybierz projekt z tabeli i zacznij od jego README. Są w nim informacje o płytce, napięciu zasilania, połączeniach i sposobie uruchomienia.
