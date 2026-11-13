# Projekty Arduino

Edukacyjne projekty Arduino: kod, schematy połączeń i instrukcje uruchomienia. Każdy projekt ma własny folder, opis sprzętu i materiały.

## Katalog projektów

| Projekt | Płytka | Materiały |
| --- | --- | --- |
| [EMG — rejestracja sygnału mięśniowego](projekty/emg/README.md) | Nano / Uno, ATmega328P, 5 V | Kod Arduino, protokół transmisji, schematy i przykład syntetyczny |

## Układ repozytorium

```text
projekty/
└── emg/
    ├── README.md
    ├── PROTOKOL.md
    ├── emg_nano/       # szkic Arduino i nagłówek
    ├── schematy/       # montaż i połączenia elektryczne
    └── przyklady/      # ilustracje i przykładowe wyniki
szablon_projektu/       # punkt wyjścia do następnego projektu
DODAWANIE_PROJEKTU.md
```

## Kolejny projekt

Skopiuj [szablon projektu](szablon_projektu/README.md) do nowego folderu w `projekty/`, uzupełnij jego materiały i dodaj wiersz do katalogu powyżej. Szczegóły: [dodawanie projektu krok po kroku](DODAWANIE_PROJEKTU.md).

Wybierz projekt z tabeli i zacznij od jego README. Są w nim informacje o płytce, napięciu zasilania, połączeniach i sposobie uruchomienia.
