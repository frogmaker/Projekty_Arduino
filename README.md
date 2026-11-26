# Projekty Arduino

Edukacyjne projekty Arduino i Raspberry Pi Pico: kod, schematy połączeń i instrukcje uruchomienia. Każdy projekt ma własny folder, opis sprzętu i materiały.

## Katalog projektów

| Projekt | Płytka | Materiały |
| --- | --- | --- |
| [EMG — rejestracja sygnału mięśniowego](projekty/emg/README.md) | Nano / Uno, ATmega328P, 5 V | Kod Arduino, protokół transmisji, schematy i przykład syntetyczny |
| [EKG — rejestracja z nakładką Olimex](projekty/ekg/README.md) | UNO R3 + SHIELD-EKG-EMG, 5 V | Oryginalny kod, analiza, schematy i dostarczony wynik JPG |
| [System do obsługi i wyświetlania sygnalu](projekty/system_sygnalu/README.md) | Pico 2 W + źródło UART | Przycisk „zbieraj dane”, zapis następnych 5 s, wykres WWW, format 1–4 kanałów, nadajniki i schemat z EKG jako przykładem |

## Układ repozytorium

```text
projekty/
├── emg/                # rejestracja sygnału mięśniowego
├── ekg/                # rejestracja EKG z nakładką Olimex
└── system_sygnalu/
    ├── README.md
    ├── PROTOKOL.md
    ├── URZADZENIE_NADAWCZE.md
    ├── pico_serwer/     # Arduino IDE: odbiornik UART, rejestrator i strona WWW
    ├── przyklady/       # testowe źródło i odczyt analogowy dla UNO/Nano
    └── schematy/        # montaż i połączenia z przykładowym urządzeniem EKG
szablon_projektu/        # punkt wyjścia do następnego projektu
DODAWANIE_PROJEKTU.md
```

## Kolejny projekt

Skopiuj [szablon projektu](szablon_projektu/README.md) do nowego folderu w `projekty/`, uzupełnij jego materiały i dodaj wiersz do katalogu powyżej. Szczegóły: [dodawanie projektu krok po kroku](DODAWANIE_PROJEKTU.md).

Wybierz projekt z tabeli i zacznij od jego README. Są w nim informacje o płytce, napięciu zasilania, połączeniach i sposobie uruchomienia.

Nowy system wykresów jest uniwersalnym odbiornikiem **danych liczbowych UART**, nie tylko EKG. W [instrukcji urządzenia nadawczego](projekty/system_sygnalu/URZADZENIE_NADAWCZE.md) opisano, jak wysyłać do Pico ten sam strumień, który wcześniej trafiał do Serial Monitora, oraz kiedy potrzebny jest adapter lub dopasowanie napięć. Po kliknięciu „zbieraj dane” powstaje nowy zapis kolejnych **5 sekund**; wynik jest wyświetlany w przeglądarce przez lokalny serwer Pico 2 W.
