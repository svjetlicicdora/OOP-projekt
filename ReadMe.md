README.md
KING OF TOKYO — C++ / SFML projekt
Ovo je implementacija društvene igre King of Tokyo u C++‑u koristeći SFML 3.0.2.
Projekt uključuje kompletan izvorni kod, sve potrebne assete i izvršnu datoteku.

Opis igre
King of Tokyo je strateška igra u kojoj igrači preuzimaju ulogu čudovišta
koja se bore za dominaciju nad Tokijom. Cilj je skupiti 20 bodova ili
eliminirati sve ostale igrače.

Struktura projekta
Projekt se sastoji od sljedećih direktorija:

* src/        → izvorni kod (.cpp)
* include/    → zaglavlja (.h)
* assets/     → ikone za kockice, ikone za igrače, fontovi, pravila
* x64/Release/ → izvršna datoteka (KING\_OF\_TOKYO.exe)
* main.cpp    → ulazna točka programa
* KING\_OF\_TOKYO.sln → Visual Studio solution

Kako pokrenuti projekt
Opcija A — Pokretanje preko .exe datoteke

* Otvoriti folder x64/Release/
* Pokrenuti KING\_OF\_TOKYO.exe.
* Provjeriti da se u istom direktoriju nalaze:

  * assets/ folder
  * SFML DLL datoteke
    Ako se igra ne pokrene, pogledati sekciju Rješenja za moguće probleme.

Opcija B — Pokretanje iz izvornog koda (Visual Studio)

* Otvoriti datoteku KING\_OF\_TOKYO.sln u Visual Studiu.
* Buildati projekt (Ctrl + Shift + B).
* Pokrenuti (F5).

Kontrole u igri
Početni ekran

* Započni — pokreće igru nakon odabira imena i broja igrača.
* Pravila — otvara ekran s pravilima igre.
  - Gumb natrag vraća na početni ekran
  Tijekom igre
* Klik na kockicu — označava ili uklanja označavanje za reroll. Rerollaju se samo neoznačene kockice.
* BACI — baca sve kockice (ili rerolla neoznačene).
* POTVRDI — završava potez i prelazi na sljedećeg igrača.
* Klik na Izlazim/Ostajem — donosi odluku o napuštanju Tokija.
* Izađi — zatvara igru.
  -Nova Igra-ponovo pokreće igru krenuvši od početnog ekrana, odabira broja igrača i imena lika.

Ovisnosti

SFML 3.0.2
Projekt koristi SFML 3.0.2 za:

* prikaz grafike
* obradu događaja
* rad s fontovima
* rad s teksturama
  C++17
  Projekt zahtijeva:
* Visual Studio 2022 ili noviji

Rješenja za moguće probleme
❗ Igra se ne pokreće
Provjeriti da se .exe nalazi u istom direktoriju kao i:

* assets/ folder
* SFML DLL datoteke
  ❗ Font se ne učitava
  Provjeriti postoji li:
  assets/fonts/DejaVuSans.ttf
  ❗ Pravila se ne učitavaju
  Provjeriti da postoji:
  assets/pravila.txt
  ❗ Slike kockica se ne prikazuju
  Provjeriti da su sve ikone u assts
