#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "tezinskigraf.h"
#include <chrono>
using namespace std;

int main()
{
    //testiranje maksimalnog protoka
/*
      TezinskiGraf G(6,true);

      G.dodajGranu(0,1,16);
      G.dodajGranu(0,2,13);
      G.dodajGranu(2,1,4);
      G.dodajGranu(1,3,12);
      G.dodajGranu(3,2,9);
      G.dodajGranu(2,4,14);
      G.dodajGranu(4,3,7);
      G.dodajGranu(3,5,20);
      G.dodajGranu(4,5,4);


*/

    /**
     ** GRAFOVI
     **/

    //TezinskiGraf G(97,true);
    //ifstream grafProtok("./grafovi/elist96d.txt");

    //TezinskiGraf G(501, true);
    //ifstream grafProtok("./grafovi/elist500d.txt");

    //TezinskiGraf G(161, true);
    //ifstream grafProtok("./grafovi/elist160d.txt");

    //TezinskiGraf G(201, true);
    //ifstream grafProtok("./grafovi/elist200d.txt");

    //TezinskiGraf G(641, true);
    //ifstream grafProtok("./grafovi/elist640d.txt");

    //TezinskiGraf G(961, true);
    //ifstream grafProtok("./grafovi/elist960d.txt");

    TezinskiGraf G(1441, true);
    ifstream grafProtok("./grafovi/elist1440d.txt");


    /**
     ** POSTAVKA
     **/

    vector<int> protokGrane;
    vector<double> tezinaGrane;

    if (grafProtok.is_open())
    {
        int pocetni_cvor;
        int krajnji_cvor;
        double tezina_grane;
        while(true)
        {
            grafProtok >> pocetni_cvor >> krajnji_cvor >> tezina_grane;
            if( grafProtok.eof() ) break;

            protokGrane.push_back(pocetni_cvor);
            protokGrane.push_back(krajnji_cvor);
            tezinaGrane.push_back(tezina_grane);
        }
        grafProtok.close();
    }
    int j = 0;
    for (int i = 0; i < tezinaGrane.size(); i++)
    {
        G.dodajGranu(protokGrane[j+1], protokGrane[j], tezinaGrane[i]);
        j += 2;
    }

    /**
     ** ISPIS FORD-FULKERSONA
     **/

    auto t1 = chrono::high_resolution_clock::now();
  cout<<G.maksimalanProtok(1,1440) << endl;
    auto t2 = chrono::high_resolution_clock::now();

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    cout <<  "ZAHTJEVALO VREMENA (STANDARD): " << duration1 << " milisekundi." << endl;

    auto t3 = chrono::high_resolution_clock::now();
  cout<<G.maksimalanProtokLista(1,1440) << endl;
    auto t4 = chrono::high_resolution_clock::now();

    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>( t4 - t3 ).count();
    cout << "ZAHTJEVALO VREMENA (LISTA): " << duration2 << " milisekundi." << endl;


    //cout << G.maksimalanProtokHash(1,640) << endl;


/*****************************************************************************************************/

    //TezinskiGraf G(289, true); ///DODATNI OD 97
    //ifstream grafProtok("./grafovi/elist96.txt");


    //TezinskiGraf G(700, true); ///DODATNI OD 205
    //ifstream grafProtok("./grafovi/elist200.txt");
/*
    TezinskiGraf G(3500, true); ///DODATNI OD 970
    ifstream grafProtok("./grafovi/elist960.txt");


    int dodatni_cvor = 970;

    if (grafProtok.is_open())
    {
        int pocetni_cvor;
        int krajnji_cvor;
        double tezina_grane;
        while(true)
        {
            grafProtok >> pocetni_cvor >> krajnji_cvor >> tezina_grane;
            if( grafProtok.eof() ) break;


            G.dodajGranu(pocetni_cvor, krajnji_cvor, tezina_grane);
            //cout << "POCETNI: " << pocetni_cvor << " KRAJNJI: " << krajnji_cvor << " TEZINA:" << tezina_grane << endl;
            G.dodajGranu(krajnji_cvor, dodatni_cvor, tezina_grane);
            //cout << "POCETNI: " << krajnji_cvor << " KRAJNJI: " << dodatni_cvor << " TEZINA:" << tezina_grane << endl;
            G.dodajGranu(dodatni_cvor, pocetni_cvor, tezina_grane);
            //cout << "POCETNI: " << dodatni_cvor << " KRAJNJI: " << pocetni_cvor << " TEZINA:" << tezina_grane << endl;

        dodatni_cvor++;
        }
        grafProtok.close();
    }
    auto t1 = chrono::high_resolution_clock::now();
  cout<<G.maksimalanProtok(1,960) << endl;
    auto t2 = chrono::high_resolution_clock::now();

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    cout <<  "ZAHTJEVALO VREMENA (STANDARD): " << duration1 << " milisekundi." << endl;

    auto t3 = chrono::high_resolution_clock::now();
  cout<<G.maksimalanProtokLista(1,960) << endl;
    auto t4 = chrono::high_resolution_clock::now();

    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>( t4 - t3 ).count();
    cout << "ZAHTJEVALO VREMENA (LISTA): " << duration2 << " milisekundi." << endl;
*/
}
