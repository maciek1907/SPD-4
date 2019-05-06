#include <iostream>
#include <stdio.h>
#include <fstream>
#include <queue>
#include <vector>
#include <iomanip>

using namespace std;
struct zadanie {
    int nr;
    int r; // moment dostepnosci
    int p; // czas wykonywania tab
    int q; // ogonek
};

ostream& operator << (ostream& os, const zadanie& z1) {
    os << z1.nr << ' ' << z1.r << ' ' << z1.p << ' ' << z1.q;
    return os;
}



class rosnaco_zadanie {
public:
    bool operator()(const zadanie& z1, const zadanie& z2) {
        return z1.r > z2.r;
    }
};

class malejaco_zadanie {
public:
    bool operator()(const zadanie& z1, const zadanie& z2) {
        return z1.q < z2.q;
    }
};

int main() {
    priority_queue<zadanie, vector<zadanie>, rosnaco_zadanie> k1;
    priority_queue<zadanie, vector<zadanie>, malejaco_zadanie> k2;
    //kolejki priorytetowa, bo sa najszybciej (O(logn)), lista O(n), tablica z qsortem O(nlogn)

    int c = 0;
    char nazwapliku[100];
    fstream plik;
    int n = -1; // liczba zadan
    zadanie *wsk = NULL;

    cout << "Podaj nazwe pliku" << endl;
    cin >> nazwapliku;
    plik.open(nazwapliku, ios::in);
    if (plik.good() != true) {
        cout << "Blad wczytywania pliku" << endl;
        return -1;
    }
    plik >> n;
    cout << "ilosc zadan:" << n << endl;
    zadanie tab[n];
    for (int i = 0; i < n; i++) {
        wsk = new zadanie;
        tab[i].nr = i;
        wsk->nr = i;
        plik >> tab[i].r;
        wsk->r = tab[i].r;
        plik >> tab[i].p;
        wsk->p = tab[i].p;
        plik >> tab[i].q;
        wsk->q = tab[i].q;
        k1.push(*wsk);
       // cout<<(*wsk)<<endl;
    }
    //wyliczanie d --Schrage -- optymalizujemy maksymalny termin dostarczenia
    //schrage jest algorytmem przyblizonym
    //zlozonosc algorytmu nlogn dla kolejek (bo n razy wstawiamy elementy do kolejki i n razy usuwamy z niej) - dla tablic n^2logn, dla list - n^2
    //jaki logarytm to nie ma znaczenia w przypadku nlogn
    //jezeli od razu wpada na maszyne to O(n)
    c = 0;
    int licznik = 1, d=0, e=0; //d - spamietywanie ile trwac bedzie najdluzsze dostarczenie zadania, e - zmienna to przechowywania biezacego czasu dostarczenia
    zadanie temp,temp1;
    temp = k1.top();
    c=temp.r;//zaczynamy prace w momencie dostepnosci pierwszego zadania
    int f=0;
    while (!(k1.empty() && k2.empty())) //dopoki nie oproznie kolejki z zadaniami
    {
        if (k2.empty()) f = -1; //jezeli mamy przerwe to flaga do gory
        while (c>=temp.r && (!k1.empty()) || f<0) // dopoki mamy zadania gotowe do wykonania i nie skonczyly sie wszystkie zadania lub flaga jest w gorze
        {
            k1.pop();
            k2.push(temp);
            f=0;
            temp = k1.top();
        }
        temp1 = k2.top(); // biore zadanie z najwiekszym ogonem, ktore jest dostepne
        k2.pop();
        //cout << licznik << ". permutacja z zadaniem nr: "
        cout << temp1.nr<<",";
        licznik ++;
        if (c<temp1.r) c = temp1.r; //jezeli mamy przeriwe to zaczynamy z czasem zadania po przeriwie
        c=c+temp1.p;
        e = c + temp1.q; //czas realizacji to moment dostepnosci maszyn + czas dostarczenia ostatniego
        if (d<e) d=e; //przypisuje o ile bedzie dluzej trwalo
    }
    cout << endl;
    cout << d << endl;
    return 0;
}

