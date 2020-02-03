#ifndef TEZINSKI_GRAF_H
#define TEZINSKI_GRAF_H
#include <list>
#include <vector>
#include <utility>
using namespace std;

struct Grana {
    int pocetni, krajnji;
    double tezina;
    Grana* suprotna;
    bool originalnaGrana;


    Grana(int p, int k, double t, bool s):pocetni{p},krajnji{k},tezina{t}, originalnaGrana{s} {}
    friend bool operator<(Grana e1, Grana e2) { return e1.tezina < e2.tezina; }
};

class TezinskiGraf {

  int n;
  bool usmjeren;
  vector<list<pair<int, double>>> lista_susjedstva;
  public:
  TezinskiGraf(int broj_cvorova, bool usmjeren);
  int brojCvorova() { return n; }
  void dodajGranu(int i, int j, double tezina);

  double maksimalanProtok(int pocetni, int krajnji);

  double maksimalanProtokHash(int pocetni, int krajnji);
  double maksimalanProtokLista(int pocetni, int krajnji);

};

#endif // TEZINSKI_GRAF_H
