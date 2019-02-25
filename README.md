# IMPORTANT
This program is used to fetch data from Polish website, and it's not intended to use by foreginers. You can, but commit messages and comments are in Polish, because that is target audience
# Jak to działa?
Program pobiera całą zawartość z bash.org.pl (w zasadzie robisz to Ty, włączając skrypt `pobierz.sh`).
Program po uruchomieniu sprawdza datę modyfikacji tego pliku, żeby utrzymywać go w miarę aktualnego.
Następnie cały plik ładowany jest do pamięci.
Program losuje losowe miejsce w pliku z przedziału <1,0.95*ROZMIAR_PLIKU>, znajduje tam sekwencję `%\n#` oznaczającą początek cytatu, następnie znajduje początek następnego cytatu i wyswietla wszystko pomiędzy nimi.
# Tryb interaktywny
Program po uruchomieniu wyczyści ekran i wypisze losowy cytat. Po wciśnięciu enter, wyczyści ekran i ponownie wypisze losowy cytat.
Będzie to powtarzane aż program otrzyma `^D` lub `^C`, lub w inny sposób zostanie zamknięty
# Tryb nieinteraktywny
Program jako parametr przyjmuje ilość cytatów do wypisania, na przykład `./bashorgpl 5` wypisze 5 losowych cytatów, bez czyszczenia ekranu i oczekiwania na akcje użytkownika.
Stworzone z myślą o dopisaniu tego do `.bashrc`, `.zshrc`, lub jakiej powłoki używasz.
# Zewnętrzne zależności
Do skompilowania wymagana jest biblioteka `ncurses` - https://invisible-island.net/ncurses/. Większość dystrybucji linuxa dostarcza ją w repozytoriach i nie trzeba kompilować ręcznie.

Ubuntu: `sudo apt install libncurses-dev`

Archlinux: `sudo pacman -S ncurses`
