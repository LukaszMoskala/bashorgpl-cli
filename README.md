# IMPORTANT
This program is used to fetch data from Polish website, and it's not intended to use by foreginers. You can, but commit messages and comments are in Polish, because that is target audience
# Jak to działa?
Program pobiera całą zawartość z bash.org.pl i zapisuje go do pliku `~/.bashdata.txt`.
Program po uruchomieniu sprawdza datę modyfikacji tego pliku, żeby utrzymywać go w miarę aktualnego.
Następnie cały plik ładowany jest do pamięci.
Program losuje losowe miejsce w pliku z przedziału <0,ROZMIAR-1>, znajduje tam sekwencję `%\n#` oznaczającą początek cytatu, następnie znajduje początek następnego cytatu i wyswietla wszystko pomiędzy nimi.
# Tryb interaktywny
Program po uruchomieniu wyczyści ekran i wypisze losowy cytat. Po wciśnięciu enter, wyczyści ekran i ponownie wypisze losowy cytat.
Będzie to powtarzane aż użytkownik wciśnie `q`
# Tryb nieinteraktywny
Program jako parametr przyjmuje ilość cytatów do wypisania, na przykład `./bashorgpl 5` wypisze 5 losowych cytatów, bez czyszczenia ekranu i oczekiwania na akcje użytkownika.
Stworzone z myślą o dopisaniu tego do `.bashrc`, `.zshrc`, lub jakiej powłoki używasz.
# Zewnętrzne zależności
Do skompilowania wymagana jest biblioteka `ncurses` oraz `libcurl`. Są one używane odpowiednio do trybu interaktywnego oraz pobierania danych.

Ubuntu: `sudo apt install libncurses-dev libcurl3-gnutls-dev`

Archlinux: `sudo pacman -S ncurses libcurl-gnutls`
