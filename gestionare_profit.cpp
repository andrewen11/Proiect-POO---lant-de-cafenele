#include "Gestionare_profit.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
//CITESC DATELE DIN raport_activitate.csv
void GestionareProfituri::citeste_raportul_din_fisier(string fisier, map<string, double>& totaluri_pe_orase) {
    ifstream raport_in(fisier);
    if (raport_in.is_open()) {
        string linie;
        while (getline(raport_in, linie)) {
            istringstream ss(linie);
            string tip, oras, suma_str;
            double suma;

            getline(ss, tip, ',');
            getline(ss, oras, ',');
            getline(ss, suma_str, ',');

            try {
                suma = stod(suma_str);
            } catch (const std::invalid_argument& e) {
                cerr << "Eroare la conversia valorii " << suma_str << " in numar! Ignor linia: " << linie << endl;
                continue;
            }

            if (tip.find("cheltuieli") != string::npos || tip.find("salariu") != string::npos)
                totaluri_pe_orase[oras] -= suma;
            else if (tip.find("Profit") != string::npos)
                totaluri_pe_orase[oras] += suma;
        }
        raport_in.close();
    } else {
        cerr << "Eroare la deschiderea fisierului: " << fisier << endl;
    }
}

void GestionareProfituri::scrie_totaluri_si_mesaj_in_fisier(string fisier, map<string, double>& totaluri_pe_orase) {
    ofstream raport_out(fisier, ios::app);
    if (raport_out.is_open()) {
        for (auto& oras : totaluri_pe_orase) {
            double total = oras.second;
            raport_out << "TOTAL," << oras.first << "," << total << ",RON\n";
        }
        raport_out << "Raport zilnic finalizat\n";
        raport_out.close();
    } else {
        cerr << "Eroare la deschiderea fisierului pentru scriere: " << fisier << endl;
    }
}

void GestionareProfituri::adauga_totaluri_si_mesaj(string fisier_raport, string fisier_profituri) {
    map<string, double> totaluri_pe_orase;
    citeste_raportul_din_fisier(fisier_raport, totaluri_pe_orase);
    scrie_totaluri_si_mesaj_in_fisier(fisier_profituri, totaluri_pe_orase);

    ofstream raport_out(fisier_raport, ios::app);
    if (raport_out.is_open()) {
        for (auto& oras : totaluri_pe_orase) {
            double total = oras.second;
            raport_out << "TOTAL," << oras.first << "," << total << ",RON\n";
        }
        raport_out.close();
    } else {
        cerr << "Eroare la deschiderea fisierului pentru actualizare: " << fisier_raport << endl;
    }
}