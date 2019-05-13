/*
Copyright (C) 2019 Łukasz Konrad Moskała

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
//tych 3 chyba nie muszę tłumaczyć
#include <iostream>
#include <fstream>
#include <random>
//memset jest w tej bibliotece
//memcpy chyba też
#include <cstring>
//do odczytu daty modyfikacji
#include <sys/types.h>
#include <sys/stat.h>
//potrzebujemy date żeby porównać
//z datą modyfikacji
#include <ctime>
//do trybu interaktywnego
#include <ncurses.h>
//do pobierania pliku
#include <curl/curl.h>
//do kopiowania do schowka
extern "C" {
  #include "x11copy.h"
}
using namespace std;
//rozmiar ekranu
int rows=0;
int cols=0;

char* bashdata;
//jeden cytat od drugiego oddzielony jest sekwencją % nowa linia #
//ta funkcja szuka tej sekwencji zaczynając na pozycji begin
int findwhatweneed(int begin) {
  while(memcmp(&bashdata[begin],"%\n#", 3)) begin++;
  return begin;
}
//do generatora liczb losowych
//skopiowane ze stack overflow
std::mt19937 rng;
//dlugosc pliku
size_t l;

bool beVerbose=true;
bool printHeader=true;
//$HOME/.bashdata.txt
string bashdata_location;

//tutaj sie losuje i wypisuje cytat na ekran

/*
W zasadzie w tej funkcji jest bug.
Gdy randomPosition wskazuje na ostatni cytat, nie znajdziemy następnego
wtedy funkcja findwhatweneed może spróbować odczytać pamięć innego procesu
i program zostanie zabity przez jądro. Mimo to szanse na to że tak się stanie
są niewielkie, a nawet jak tak się stanie to po prostu program się wysypie
i nie będzie z tym jakiegoś wielkiego problemu, więc nie widzę sensu naprawiania
tego buga
*/
char* stripedbashdata=nullptr;
size_t datalen;
void printQuote(auto dist, bool useCurses=false) {
  int randomPosition=dist(rng); //losowa pozycja
  int realDataBegin=findwhatweneed(randomPosition)+3; //początek pierwszego cytatu po tej pozycji
  int next=findwhatweneed(realDataBegin); //początek kolejnego cytatu (koniec pierwszego)
  free(stripedbashdata);
  datalen=next-realDataBegin+1;

  stripedbashdata=(char*)malloc(datalen);
  memset(stripedbashdata,0,sizeof(stripedbashdata));
  memcpy(stripedbashdata, bashdata+realDataBegin, next-realDataBegin);
  string s(stripedbashdata);
  int firstNl=s.find("\n");
  string header=s.substr(0,firstNl);
  string quote=s.substr(firstNl+1);
  if(useCurses) {
    if(printHeader) {
      attron(COLOR_PAIR(1)); //zielony tekst
      attron(A_BOLD); //pogrubiony tekst
      printw(header.c_str()); //wypisz nagłówek
      attroff(A_BOLD); //wyłącz pogrubiony tekst
      printw("\n");
      //dorysuj linie pod nagłówkiem
      for(int i=0;i<cols;i++)
        printw("=");
      if(quote[0] != '\n');
        printw("\n");
      attroff(COLOR_PAIR(1));//wyłącz zielony tekst
      mvprintw(2,0,quote.c_str()); //wypisz cytat
    }
    else {
      mvprintw(0,0,quote.c_str());
    }

  }
  else {
    if(printHeader) {
      cout<<header<<endl;
      for(int i=0;i<header.size();i++)
        cout<<"=";
      if(quote[0] != '\n');
        cout<<endl;
    }
    cout<<quote<<endl;
  }
}
//Ta funkcja i kolejna, to połączenie kodu z kilku przykładowych programów ze strony
//https://curl.haxx.se
static size_t write_bashdata_to_memory(void *ptr, size_t size, size_t nmemb, void *stream)
{
  char* tmp=(char*)realloc(bashdata, l+size*nmemb);
  if(tmp == NULL) return 0;
  bashdata=tmp;
  memcpy(bashdata+l, ptr, size*nmemb);
  l+=size*nmemb;
  return size*nmemb;
}
int downloadbashdata() {
  CURL *curl;
  CURLcode res;
  bashdata=(char*)malloc(1);
  curl = curl_easy_init();
  if(curl) {
    if(beVerbose)
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(curl, CURLOPT_URL, "http://bash.org.pl/text");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_bashdata_to_memory);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      cout<<"curl_easy_perform(): "<<curl_easy_strerror(res)<<endl;
      return 1;
    }
    curl_easy_cleanup(curl);
  }
  else return 1;
  ofstream plik;
  plik.open(bashdata_location.c_str());
  if(plik.is_open()) {
    plik.write(bashdata, l);
    plik.close();
  }
  else {
    cout<<"Nie udało się zapisać nowych danych do "<<bashdata_location<<endl;
  }
  return 0;
}
//pokazuje ostrzeżenie jak plik nie byl modyfikowany przez więcej niż 14 dni
int checkModTime() {
  struct stat result;
  if(stat(bashdata_location.c_str(), &result)==0)
  {
    auto mod_time = result.st_ctime;
    double diff=(time(NULL)-mod_time)/60.0/60.0/24.0;
    if(beVerbose)
      cout<<bashdata_location<<" zmodifikowany ostatnio "<<(int)diff<<" dni temu"<<endl;
    if(diff > 14.0) return 1; //zwróć 1 jak plik jest stary i trzeba zaktualizować
  }
  else {
    cout<<"Nie udało się wykonać stat na bashdata.txt: "<<strerror(errno)<<endl;
    return 1;//zwróć 1 jak się nie udało wykonać stat
  }
  return 0;

}
//Ta funkcja odczytuje cały plik do pamięci
//na chwile obecną plik zajmuje około 1.8MB więc można sobie na to pozwolić
int loadFile() {
  ifstream plik;
  plik.open(bashdata_location.c_str());
  if(!plik.is_open()) {
    cout<<"Nie znaleziono "<<bashdata_location<<endl;
    return 1;
  }
  //przesuwamy kursor odczytu na koniec pliku żeby sprawdzić jego długość
  plik.seekg(0,ios_base::end);
  //odczytujemy pozycje na jakiej jesteśmy
  l=plik.tellg();
  //przechodzmy z powrotem na początek pliku
  plik.seekg(0);
  //alokujemy pamięć
  bashdata=(char*)malloc(l+1);
  bashdata[l]=0;
  //odczytujemy dane z pliku
  plik.read(bashdata,l);
  //sprawdzamy ile danych faktycznie odczytaliśmy
  size_t k=plik.gcount();
  //zamykamy plik
  plik.close();
  //zwraca 1 jeżeli k jest różne od l, co może być spowodowane
  //chyba tylko przez błąd odczytu
  return (k != l);
}
int args;
char** argv;
//Odczytaj parametr z postaci arg=val
string getval(string arg, string def="") {
  for(int i=0;i<args;i++) {
    string ca=argv[i];
    int pos=ca.find("=");
    if(pos > 0) {
      if(ca.substr(0, pos) == arg) {
        return ca.substr(pos+1);
      }
    }
  }
  return def;
}
int main(int _args, char** _argv) {
  //na widowsie inicjuje winsockowe rzeczy
  curl_global_init(CURL_GLOBAL_ALL);
  args=_args;
  argv=_argv;
  //informacje o stanie pliku
  beVerbose=(getval("badzrozmowny","tak") == "tak");
  //pokazywać nagłówek cytatu?
  //chodzi o to w tej postaci: 9695 (http://bash.org.pl/9695/)
  printHeader=(getval("pokaznaglowek","tak") == "tak");
  //ilosc cytatów do wyświetlenia, 0 oznacza tryb interaktywny
  int n=atoi(getval("n","0").c_str());

  //wymagane dla UTF-8
  //i w systemie musi być wygenerowane LOCALE wspierające utf-8
  setlocale(LC_CTYPE, "");

  //zainicjuj bufor żeby free nie zwróciło zaraz błędu
  stripedbashdata=(char*)malloc(1);

  //ustalamy gdzie jest katalog domowy użytkownika
  char* home=getenv("HOME");
  if(home != NULL) {
    bashdata_location=string(home)+"/.bashdata.txt";
  }
  else {
    cout<<"Nie udało się ustalić wartości $HOME"<<endl;
    return 1;
  }
  //sprawdzanie czasu modyfikacji pliku
  if(checkModTime() == 0) {
    if(beVerbose)
      cout<<bashdata_location<<" młodszy niż 14 dni, nie aktualizuj"<<endl;
    if(loadFile()) {
      cout<<"Lol, nie udało się załadować xD"<<endl;
      return 1;
    }
  }
  else {
    if(beVerbose)
      cout<<"Aktualizuje plik"<<endl;
    if(downloadbashdata()) {
      cout<<"Nie udało się zaktualizować pliku, próbuje odczytać z dysku"<<endl;
      if(loadFile()) {
        cout<<"Też się nie udało, siema"<<endl;
        return 1;
      }
    }
  }
  //generator liczb losowych podjebany ze stack overflow
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist(0,l-1);

  //Kompatybilność wsteczna
  if(args == 2 && n == 0 && string(argv[1]).find("=") == -1)
    n=atoi(argv[1]);
  //N oznacza ilość cytatów do wyświetlenia, jak jest równy 0
  //używamy trybu interaktywnego
  if(n > 0) {
    //wyświetlamy n cytatów
    for(int i=0;i<n;i++) {
      printQuote(dist);
    }
  }
  else { //tryb interaktywny
    Atom xSelection=XCopyInit();
    initscr();
    noecho();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    printw("bashorgpl-cli Copyright (C) 2019 Łukasz Konrad Moskała\n");
    printw("This program comes with ABSOLUTELY NO WARRANTY.\n");
    printw("This is free software, and you are welcome to redistribute it\n");
    printw("under certain conditions; Read attached license file for details.\n");
    printw("\n");
    printw("You should have received a copy of the GNU General Public License\n");
    printw("along with this program.  If not, see <https://www.gnu.org/licenses/>.\n");
    getch();
    char c;
    do {
      //czyszczenie ekranu
      clear();
      //odczyt rozmiaru ekranu
      //potrzebny do rysowania linii pod nagłówkiem i obliczenia pozycji stopki
      getmaxyx(stdscr, rows, cols);
      printQuote(dist, true);
      //włącz kolor i wypisz stopkę
      attron(COLOR_PAIR(2));
      mvprintw(rows-2,0,"bashorgpl-cli Copyright (C) 2019 Łukasz Konrad Moskała\n");
      printw("Q żeby wyjść, spacja albo enter żeby przeglądać dalej");
      attroff(COLOR_PAIR(2));
      do {
        c=getch();
        if(c == 'c' || c == 'C') {
          //skopiuj do schowka
          XCopy(xSelection, stripedbashdata, datalen);
        }
      }
      while(c != 'Q' && c != 'q' && c != ' ' && c != '\n');
    }
    while(c != 'Q' && c != 'q');
    endwin();
  }

  //zwalniamy pamięć
  free(bashdata);
  return 0;
}
