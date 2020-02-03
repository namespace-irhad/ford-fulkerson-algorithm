#include "tezinskigraf.h"
#include "hash.h"
#include <climits>
#include <set>
#include <algorithm>
#include <queue>
#include <iostream>
#include <unordered_map>
using namespace std;

TezinskiGraf::TezinskiGraf(int broj_cvorova, bool usmjeren):n(broj_cvorova),
    usmjeren(usmjeren)
{

    lista_susjedstva.resize(n);
}

void TezinskiGraf::dodajGranu(int i, int j, double tezina)
{
    lista_susjedstva[i].push_back(make_pair(j, tezina));
    if(!usmjeren)
        lista_susjedstva[j].push_back(make_pair(i,tezina));
}


/**
 ** STANDARD KOD MATRICA
 **/

double TezinskiGraf::maksimalanProtok(int pocetni, int krajnji)
{
    //najprije pravimo matricu udaljenosti pomocnog grafa i popunjavamo je nulama
    vector<vector<double>> matricaUdaljenosti(n);
    for(int i = 0; i < n; i++)
        matricaUdaljenosti[i].resize(n);

    //sada pravimo listu susjedstva pomocnog grafa
    //bitno je samo da li postoji grana (ne i njen kapacitet)
    //moramo pamtiti i da li je ta grana u originalnom grafu (true ako jeste)
    vector<list<pair<int,bool>>> susjedi(n);
    for(int i=0; i<n; i++)
        for(auto it = lista_susjedstva[i].begin(); it!=lista_susjedstva[i].end(); it++)
        {
            susjedi[i].push_back({it->first,true});
            susjedi[it->first].push_back({i,false});
            matricaUdaljenosti[i][it->first] = it->second;
            //na pocetku samo postoji grana od i do it->first koja je jednaka kapacitetu
            //grana u suprotnom smjeru zasad ostaje na 0
        }
        /*
        for(int i=0; i<n; i++) {
                cout << i << ": ";
            for(auto it = susjedi[i].begin(); it!=susjedi[i].end(); it++) {
                cout << it->first << " " << it->second << " ";
            }
        cout << endl;
            }
        */
    double maksimalan_protok = 0;
    //sada trazimo puteve od pocetnog do krajnjeg u pomocnom grafu
    //kada vise ne bude puteva onda prekidamo petlju
    while(true)
    {
        //koristimo BFS
        queue<int> red;
        red.push(pocetni);
        //za svaki cvor pamtimo ko ga je pronasao kako bi rekonstruisali put
        //-1 znaci da nije pronadjen, a -2 da se radi o pocetnom cvoru
        //pored toga pamtimo da li ta grana po kojoj je pronadjen pripada originalnom grafu
        vector<pair<int,bool>> ko_ga_je_nasao(n, {-1,false});
        ko_ga_je_nasao[pocetni].first = -2;
        while(!red.empty())
        {
            int trenutni = red.front();
            red.pop();
            for(auto it = susjedi[trenutni].begin(); it!=susjedi[trenutni].end(); it++)
            {
                //moguce je da u pomocnom grafu postoji grana ali ako je njena tezina 0 to znaci da je u sustini nema
                if(matricaUdaljenosti[trenutni][it->first] == 0) {
                    continue;
                }
                if(ko_ga_je_nasao[it->first].first == -1)
                {
                    red.push(it->first);
                    ko_ga_je_nasao[it->first] = {trenutni,it->second};
                }
            }
            //ako smo vec nasli krajnji nema potrebe dalje traziti
            if(ko_ga_je_nasao[krajnji].first != -1)
                break;
        }


        /*
                for (int i = 0; i < ko_ga_je_nasao.size(); i++) {
            cout << ko_ga_je_nasao[i].first << " i " << ko_ga_je_nasao[i].second << endl;
        }
        */
        //ako nema puta znaci da smo nasli najveci protok
        if(ko_ga_je_nasao[krajnji].first == -1)
            break;
        //sada treba rekonstruisati put
        //u putu cuvamo grane i za svaku granu pamtimo da grana pripada originalnom grafu
        vector<pair<pair<int,int>,bool>> put;
        //usput ćemo i vidjeti koja je najmanja grana na tom putu
        double duzina_najmanje_grane = INT_MAX;
        int trenutni = krajnji;
        do
        {
            put.push_back({{ko_ga_je_nasao[trenutni].first,trenutni},ko_ga_je_nasao[trenutni].second});
            //cout << trenutni << " t: " << matricaUdaljenosti[ko_ga_je_nasao[trenutni].first][trenutni] << endl;
            if(matricaUdaljenosti[ko_ga_je_nasao[trenutni].first][trenutni] < duzina_najmanje_grane)
                duzina_najmanje_grane = matricaUdaljenosti[ko_ga_je_nasao[trenutni].first][trenutni];
            //cout << matricaUdaljenosti[ko_ga_je_nasao[trenutni].first][trenutni] << endl;
            trenutni = ko_ga_je_nasao[trenutni].first;
        }
        while(trenutni != pocetni);
        //povecavamo protok za vrijednost najmanje grane
        maksimalan_protok += duzina_najmanje_grane;
        //modifikujemo graf

        for(int i = 0; i<put.size(); i++)
        {

            if(put[i].second)
            {
                //cout << "TEZINA1: " << duzina_najmanje_grane << endl;

                // cout << "ULAZ: " << matricaUdaljenosti[put[i].first.first][put[i].first.second] << endl;
                matricaUdaljenosti[put[i].first.first][put[i].first.second] -= duzina_najmanje_grane;
                matricaUdaljenosti[put[i].first.second][put[i].first.first] += duzina_najmanje_grane;
            }
            else
            {
               //cout << "IZLAZ: " << matricaUdaljenosti[put[i].first.first][put[i].first.second] << endl;
                matricaUdaljenosti[put[i].first.first][put[i].first.second] += duzina_najmanje_grane;
                matricaUdaljenosti[put[i].first.second][put[i].first.first] -= duzina_najmanje_grane;
            }
        }

    }
    return maksimalan_protok;
}

/**
 **  KOD HASH TABELA
 **/

//Koristenjem mape (hash tabela) umjesto 2D nizova(matrica)
double TezinskiGraf::maksimalanProtokHash(int pocetni, int krajnji)
{
    double grana_tezina;

//unordered_map<int, unordered_map<int,double>> mapaUdaljenosti;
    vector<unordered_map<int, double>> mapaUdaljenosti(n);
    vector<unordered_map<int, int>> mapaVrijednostNula(n);



    vector<list<pair<int,bool>>> susjedi(n);
    for(int i=0; i<n; i++)
        for(auto it = lista_susjedstva[i].begin(); it!=lista_susjedstva[i].end(); it++)
        {
            //rezervisanje prostora najveceg broja susjeda
            //U slucaju da se nalazi previse kolizije ismedju elemenata
            mapaUdaljenosti[i].max_load_factor(0.25);
            susjedi[i].push_back({it->first,true});
            susjedi[it->first].push_back({i,false});
            mapaUdaljenosti[i][it->first] = it->second;

        }

    double maksimalan_protok = 0;

    while(true)
    {
        queue<int> red;
        red.push(pocetni);
        vector<pair<int,bool>> ko_ga_je_nasao(n, {-1,false});
        ko_ga_je_nasao[pocetni].first = -2;
        while(!red.empty())
        {
            int trenutni = red.front();
            red.pop();
            for(auto it = susjedi[trenutni].begin(); it!=susjedi[trenutni].end(); it++)
            {

                /*
                 * Prolaz kroz vrijednosti tako sto se koristi iteracija kroz mapu.
                 * Nakon pronalaska, vrijednost tezine se drzi u iteraciji koja se provjerava
                 */

                auto pronadji = mapaUdaljenosti[trenutni].find(it->first);
                if(pronadji != mapaUdaljenosti[trenutni].end())
                    if (pronadji->second == (double)0)
                        continue;

                if(ko_ga_je_nasao[it->first].first == -1)
                {
                    red.push(it->first);
                    ko_ga_je_nasao[it->first] = {trenutni,it->second};
                }
            }
            if(ko_ga_je_nasao[krajnji].first != -1)
                break;
        }
        if(ko_ga_je_nasao[krajnji].first == -1)
            break;

        vector<pair<pair<int,int>,bool>> put;
        double duzina_najmanje_grane = INT_MAX;
        int trenutni = krajnji;
        do
        {
            put.push_back({{ko_ga_je_nasao[trenutni].first,trenutni},ko_ga_je_nasao[trenutni].second});
            /*
             * Provjera da li manja grana postoji, ovim putem ne mora 2 puta traziti za svaku granu
             */
            auto pronadjena_grana = mapaUdaljenosti[ko_ga_je_nasao[trenutni].first].find(trenutni);
            if (pronadjena_grana != mapaUdaljenosti[ko_ga_je_nasao[trenutni].first].end())
                grana_tezina = pronadjena_grana->second;
            else
                grana_tezina = 0;

            if(grana_tezina < duzina_najmanje_grane)
                duzina_najmanje_grane = grana_tezina;
            trenutni = ko_ga_je_nasao[trenutni].first;

        }
        while(trenutni != pocetni);
        maksimalan_protok += duzina_najmanje_grane;
        for(int i = 0; i<put.size(); i++)
        {

            if(put[i].second)
            {

                mapaUdaljenosti[put[i].first.first][put[i].first.second] -= duzina_najmanje_grane;
                mapaUdaljenosti[put[i].first.second][put[i].first.first] += duzina_najmanje_grane;
            }
            else
            {
                mapaUdaljenosti[put[i].first.first][put[i].first.second] += duzina_najmanje_grane;
                mapaUdaljenosti[put[i].first.second][put[i].first.first] -= duzina_najmanje_grane;
            }

        }
    }
    return maksimalan_protok;

}

/**
 **  KOD LISTA SUSJEDI
 **/

double TezinskiGraf::maksimalanProtokLista(int pocetni, int krajnji)
{
    vector<vector<Grana>> susjedi(n);
    //prolaz kroz listu susjedstva
    for(int i=0; i<n; i++)
        for(auto it = lista_susjedstva[i].begin(); it!=lista_susjedstva[i].end(); it++)
        {
            //pravljenje grana koje ce se dodabati u listu susjedi
            Grana g1 = {i, it->first, it->second, true};
            Grana g2 = {it->first, i, 0, false};

            //obiljezavanje suprotne grane sa obje grane
            g1.suprotna = &g2;
            g2.suprotna = &g1;

            susjedi[i].push_back(g1);
            susjedi[it->first].push_back(g2);

        }
/*
          for (int i = 0; i < susjedi.size(); i++) {
                cout << i << ": ";
          for (int j = 0; j < susjedi[i].size(); j++) {
            cout << susjedi[i][j].krajnji << " " << susjedi[i][j].originalnaGrana << " ";
          }
          cout << endl;
          }
*/

    double maksimalan_protok = 0;

    while(true)
    {
        //koristimo BFS
        queue<int> red;
        red.push(pocetni);
        //za svaki cvor pamtimo ko ga je pronasao kako bi rekonstruisali put
        //pocetni cvor postavlja vrijednost -2 kako ne bi prolazili ponovo
        vector<pair<int,bool>> ko_ga_je_nasao(n, {-1,false});
        ko_ga_je_nasao[pocetni].first = -2;
        while(!red.empty())
        {
            int trenutni = red.front();
            red.pop();

            //prolaz kros sve susjede trenutnog cvora
            for (int i = 0; i < susjedi[trenutni].size(); i++)
            {
                //ukoliko je neka od tezina jednaka 0 zaobilazimo
                if (susjedi[trenutni][i].tezina == 0) {
                    continue;
                }

                //ukoliko nije cvor pronadjes pronalazimo ga i stavljamo u ko_ga_je_nasao
                int kraj_grane = susjedi[trenutni][i].krajnji;
                if(ko_ga_je_nasao[kraj_grane].first == -1)
                {
                    red.push(kraj_grane);
                    ko_ga_je_nasao[kraj_grane] = {trenutni, susjedi[trenutni][i].originalnaGrana};

                }
            }

            //ako smo vec nasli krajnji nema potrebe dalje traziti
            if(ko_ga_je_nasao[krajnji].first != -1)
                break;
        }

        //ako nema puta znaci da smo nasli najveci protok
        if(ko_ga_je_nasao[krajnji].first == -1)
            break;

        //sada treba rekonstruisati put
        //u putu cuvamo grane i za svaku granu pamtimo da grana pripada originalnom grafu
        vector<pair<pair<int,int>,bool>> put;
        //usput ćemo i vidjeti koja je najmanja grana na tom putu
        double duzina_najmanje_grane = INT_MAX;
        int trenutni = krajnji;
        do
        {

            //prolaz kroz zadnjeg do pocetnog, dodavanjem na put
            put.push_back({{ko_ga_je_nasao[trenutni].first,trenutni},ko_ga_je_nasao[trenutni].second});

            //trazenje puta od kraja koristenjem grane
            Grana* kraj_puta;
            for (int i = 0; i < susjedi[ko_ga_je_nasao[trenutni].first].size(); i++)
            {
                //trazimo granu koja se podudara sa trenutnim cvorom i cija je tezina pozitivna
                if (susjedi[ko_ga_je_nasao[trenutni].first][i].krajnji == trenutni && susjedi[ko_ga_je_nasao[trenutni].first][i].tezina > 0)
                {
                    kraj_puta = &susjedi[ko_ga_je_nasao[trenutni].first][i];
                    break;
                }
            }

            if (kraj_puta->tezina < duzina_najmanje_grane)
                duzina_najmanje_grane = kraj_puta->tezina;
            trenutni = ko_ga_je_nasao[trenutni].first;
        }
        while(trenutni != pocetni);
        //povecavamo protok za vrijednost najmanje grane
        maksimalan_protok += duzina_najmanje_grane;
        //modifikujemo graf

        for(int i = 0; i < put.size(); i++)
        {

            //trazenje grane koja ima krajnji cvor kao put
            //modifikacija te grane i njegove suprotne
            Grana* modifikacija;
            for (int j = 0; j < susjedi[put[i].first.first].size(); j++) {
                if (susjedi[put[i].first.first][j].krajnji == put[i].first.second) {
                    modifikacija = &susjedi[put[i].first.first][j];
                    break;
                }
            }
            //modifikacija oduzimanje na glavnom putu
            if(put[i].second)
            {
                modifikacija->tezina -= duzina_najmanje_grane;
                modifikacija->suprotna->tezina += duzina_najmanje_grane;
            }
            else
            {
                //cout << "IZLAZ: " << susjedi[put[i].first.first][j].tezina << endl;
                modifikacija->tezina += duzina_najmanje_grane;
                modifikacija->suprotna->tezina -= duzina_najmanje_grane;

            }


        }
    }
    return maksimalan_protok;
}

