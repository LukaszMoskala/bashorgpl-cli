#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>

#include <ctime>

using namespace std;
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
//tutaj sie losuje i wypisuje cytat na ekran
void printQuote(auto dist) {
  int randomPosition=dist(rng); //losowa pozycja
  int realDataBegin=findwhatweneed(randomPosition)+3; //początek pierwszego cytatu po tej pozycji
  int next=findwhatweneed(realDataBegin); //początek kolejnego cytatu (koniec pierwszego)
  while(realDataBegin < next) { //i cyk pętla do wyświetlenia
    cout<<bashdata[realDataBegin++];
  }
  cout<<endl;
}
//pokazuje ostrzeżenie jak plik nie byl modyfikowany przez więcej niż 14 dni
void checkModTime() {
  struct stat result;
  if(stat("bashdata.txt", &result)==0)
  {
    auto mod_time = result.st_ctime;
    double diff=(time(NULL)-mod_time)/60.0/60.0/24.0;
    cout<<"bashdata.txt zmodifikowany ostatnio "<<(int)diff<<" dni temu"<<endl;
    if(diff > 14.0) cout<<"Zalecana aktualizacja: ./pobierz.sh"<<endl;
  }
  else {
    cout<<"Nie udało się wykonać stat na bashdata.txt: "<<strerror(errno)<<endl;
  }
  cout<<endl;
}
int loadFile() {
  ifstream plik;
  plik.open("bashdata.txt");
  if(!plik.is_open()) {
    cout<<"Nie znaleziono bashdata.txt"<<endl;
    cout<<"Uruchom skrypt pobierz.sh"<<endl;
    return 1;
  }
  plik.seekg(0,ios_base::end);
  l=plik.tellg();
  plik.seekg(0);

  bashdata=(char*)malloc(l+1);
  bashdata[l]=0;
  plik.read(bashdata,l);
  size_t k=plik.gcount();
  plik.close();
  return (k != l); //return 0 if k == l
}
int main(int args, char** argv) {
  //sprawdzanie czasu modyfikacji pliku
  checkModTime();
  if(loadFile()) return 1; //konczymy jak sie nie załadowało

  //generator liczb losowych podjebany ze stack overflow
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist(0,l*0.95);

  //parametr zawiera liczbe cytatów do wyświetlenia
  if(args > 1) {
    int n=atoi(argv[1]);
    //wyświetlamy n cytatów
    for(int i=0;i<n;i++) {
      printQuote(dist);
    }
  }
  else { //tryb interaktywny
    string s;
    do {
      //czyszczenie ekranu
      cout<<(char)0x1b<<"[2J";

      printQuote(dist);
      cout<<"^D albo ^C aby wyjść, enter aby wyświetlać dalej"<<endl;
      getline(cin,s);
    }
    while(cin.good() && s != "exit");
  }

  //zwalniamy pamięć
  free(bashdata);
  return 0;
}
