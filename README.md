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
Program jako parametr przyjmuje ilość cytatów do wypisania, na przykład `./bashorgpl n=5` wypisze 5 losowych cytatów, bez czyszczenia ekranu i oczekiwania na akcje użytkownika.
Stworzone z myślą o dopisaniu tego do `.bashrc`, `.zshrc`, lub jakiej powłoki używasz.
# Wyciszanie
Program domyślnie wyświetla informacje o stanie pliku `.bashdata.txt`, oraz, podczas jego pobierania, ustawia CURLa żeby pokazywał pełne informacje (`CURLOPT_VERBOSE`). Jeżeli chcesz pozbyć się tych wiadomości, użyj argumentu `badzrozmowny=nie`
# Wypisywanie nagłówków
Aby nie wyświetlać nagłówków cytatów, dodaj argument `pokaznaglowek=nie`. Działa to zarówno w trybie interaktywnym, jak i nieinteraktywnym.
# Zewnętrzne zależności
Do skompilowania wymagana jest biblioteka `ncurses` oraz `libcurl`. Są one używane odpowiednio do trybu interaktywnego oraz pobierania danych.

Ubuntu: `sudo apt install libncurses-dev libcurl3-gnutls-dev`

Archlinux: `sudo pacman -S ncurses libcurl-gnutls`
# Kompilacja (Linux)
```
git clone https://github.com/LukaszMoskala/bashorgpl-cli.git
cd bashorgpl-cli
make
```
# Instalacja (Linux)
Program zostanie zainstalowany do /usr/local/bin
```
sudo make install
```
Aby zmienić katalog do instalacji, użyj flagi PREFIX.
```
sudo make PREFIX=/moj/folder/do/instalacji install
```
W tym przykładzie program zostanie zainstalowany w `/moj/folder/do/instalacji/bin/`
