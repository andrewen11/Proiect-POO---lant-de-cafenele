#include "comenzi.h"
#include <fstream>
using namespace std;
// adaugarea produselor la comanda
void Comanda::adauga_produs(Produs *produs, int cantitate, double reducere)
{
	produse_comandate[produs] += cantitate;
	cost_total = (cost_total + (produs->cost_per_unitate * cantitate)) * reducere;

	// se trimite alerta / notificare daca stocul va scadea sub 5 bucati in urma comenzilor
	if (produs->cantitate - cantitate < 5 && produs->cantitate - cantitate > 0)
	{
		string mesaj = "Stocul pentru produsul " + produs->nume_produs + " este scazut (sub 5 unitati)!";
		notifica_observatori(mesaj);
	}
	if (produs->cantitate - cantitate == 0)
	{
		string mesaj = "Stocul pentru produsul " + produs->nume_produs + " s-a epuizat! Reaprovizioneaza sau sterge produsul definitiv din cafenea!";
		notifica_observatori(mesaj);
	}

	produs->cantitate -= cantitate;
}

// Adaug observator pentru notificari
void Comanda::adauga_observator(Observer *observer)
{
	observatori.push_back(observer);
}

void Comanda::notifica_observatori(std::string mesaj)
{
	for (Observer *observer : observatori)
	{
		observer->actualizeaza(mesaj);
	}
}

// Calcul costul total al comenzii
double Comanda::calculeaza_cost_total()
{
	return cost_total;
}

// Salvez comanda in CSV-ul specific orasului de la care s-a efectuat comanda
void Comanda::salveaza_in_fisier(std::string oras, double reducere)
{
	string fisier_CSV = "comenzi_" + oras + ".csv";
	ifstream fisier(fisier_CSV);
	string linie;

	bool este_gol = !getline(fisier, linie); // Verificăm dacă fișierul este gol
	fisier.close();							 // Închidem fișierul pentru a-l deschide în modul de scriere

	ofstream fisier_out(fisier_CSV, ios::app); // Deschidem fișierul în modul append (adăugare)
	if (fisier_out.is_open())
	{
		// Adăugăm antetul doar dacă fișierul este gol
		if (este_gol)
		{
			fisier_out << "NUME,PRENUME,NUME COMANDA (PRODUS),CANTITATE,PRET TOTAL PRODUS\n";
		}

		// Pentru fiecare produs comandat, salvăm informațiile în formatul corect
		for (auto &p : produse_comandate)
		{
			// Salvăm numele și prenumele pentru fiecare produs
			fisier_out << client_nume << "," << client_prenume << ",";
			fisier_out << p.first->nume_produs << "," << p.second << ","
					   << (p.first->cost_per_unitate * p.second) * reducere << "\n";
		}
	}
	fisier_out.close();
}

string Comanda::get_client_nume()
{
	return client_nume;
}

string Comanda::get_client_prenume()
{
	return client_prenume;
}
string Comanda::get_oras()
{
    return oras_comanda;
}

// Factory Pattern pentru crearea comenzilor
Comanda *ComandaFactory::creeaza_comanda(std::string nume, std::string prenume, std::string oras)
{
	return new Comanda(nume, prenume, oras);
}