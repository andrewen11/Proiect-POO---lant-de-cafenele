#include "Gestionare_evenimente.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
using namespace std;

// adaugare eveniment (in fisier si vector, pt a retine baza respectiva de date)
template <typename T1, typename T2>
void Gestionare_Evenimente<T1, T2>::adauga_eveniment(string eveniment, T1 cost, string oras)
{
	// Verif daca evenimentul exista deja în vector (pt a evita duplicarile potentiale)
	for (const auto &ev : evenimente)
	{
		if (get<0>(ev) == eveniment && get<1>(ev) == cost && get<2>(ev) == oras)
		{
			cout << "Evenimentul exista deja!" << endl;
			return;
		}
	}

	// Adaug evenimentul in fisier
	ofstream fisier_out("evenimente.csv", ios::app); // Folosesc ios::out pentru a rescrie fisierul
	if (fisier_out.is_open())
	{
		// Verific daca fisierul este gol si scriu antetul DOAR in acest caz
    fisier_out.seekp(0, ios::end);
    if (fisier_out.tellp() == 0)
    {
        fisier_out << "EVENIMENT,COST,CAFENEA (ORAS)\n";
    }

    // Adaug evenimentul nou
    fisier_out << eveniment << "," << cost << "," << oras << endl;
	}
	fisier_out.close();

	// Adaug evenimentul in vector
	evenimente.push_back(make_tuple(eveniment, cost, oras));
}

// Implementarea functiei de citire evenimente din fisier + popularea vectorului
template <typename T1, typename T2>
void Gestionare_Evenimente<T1, T2>::citeste_evenimente()
{
	ifstream fisier("evenimente.csv");
	if (!fisier.is_open())
	{
		cout << "Eroare la deschiderea fisierului pentru citire!" << endl;
		return;
	}

	string linie;
	getline(fisier, linie); // sar de antet
	while (getline(fisier, linie))
	{
		stringstream extragere_linie_fisier(linie);
		string eveniment, oras, cost_str;
		T1 cost;
		// Citirea fiecarei linii: eveniment, cost, oras
		getline(extragere_linie_fisier, eveniment, ',');
		getline(extragere_linie_fisier, cost_str, ','); // Citesc costul ca string
		getline(extragere_linie_fisier, oras);

		// Convert costul din string la tipul T1
		stringstream convertor_cost(cost_str);
		convertor_cost >> cost;

		// Verific dacă evenimentul exista deja in vectorul evenimente (evit duplicarea!)
		bool existent = false;
		for (const auto &ev : evenimente)
		{
			if (get<0>(ev) == eveniment && get<1>(ev) == cost && get<2>(ev) == oras)
			{
				existent = true;
				break;
			}
		}

		// Daca NU exista deja în vector, adaug evenimentul
		if (!existent)
		{
			evenimente.push_back(make_tuple(eveniment, cost, oras)); // creez o "legatura" tripla: eveniment - cost - oras pt fiecare eveniment din vectorul STL
		}
	}

	fisier.close();
}

// Implementarea functiei de afisare a evenimentelor
template <typename T1, typename T2>
void Gestionare_Evenimente<T1, T2>::afiseaza_evenimente() const
{
	cout << "***** Evenimente Speciale *****" << endl;
	for (const auto &eveniment : evenimente)
	{

		cout << "Eveniment: " << get<0>(eveniment)
			 << ", Cost: " << get<1>(eveniment)
			 << " RON, Oras: " << get<2>(eveniment) << endl;
	}
}

template <typename T1, typename T2>
void Gestionare_Evenimente<T1, T2>::calculeaza_costuri_evenimente(bool scrie_in_fisier) const
{
	// Creez un "map" pentru a stoca costurile totale per oras
	map<string, tuple<T1, string>> costuri_per_oras;

	// Parcurg evenimentele si adun costurile pentru fiecare oras
	for (const auto &eveniment : evenimente)
	{
		string tip_eveniment = get<0>(eveniment); // Tipul evenimentului
		T1 cost = get<1>(eveniment);			  // Costul evenimentului
		string oras = get<2>(eveniment);		  // Orasul

		// Adaugam costul si tipul cheltuielii pentru orasul respectiv
		if (costuri_per_oras.find(oras) != costuri_per_oras.end())
		{
			get<0>(costuri_per_oras[oras]) += cost; // Adaugăm costul la cel existent
		}
		else
		{
			costuri_per_oras[oras] = make_tuple(cost, tip_eveniment); // Init un nou "tuplu"
		}
	}

	if (scrie_in_fisier)
	{
		// Scriu costurile totale per oras intr-un fisier CSV, incluzand si tipul cheltuielii
		ofstream fisier_out("raport_activitate.csv", ios::app); // Deschidem fisierul in modul append
		if (fisier_out.is_open())
		{
			for (const auto &oras_cost : costuri_per_oras)
			{
				fisier_out << "(cheltuieli) " << get<1>(oras_cost.second) << "," // Tipul evenimentului
						   << oras_cost.first << ","							 // Orasul
						   << get<0>(oras_cost.second) << ",RON\n";				 // Costul total
			}
			fisier_out.close();
			cout << "Costurile au fost salvate in fisierul costuri_evenimente.csv." << endl;
		}
		else
		{
			cout << "Eroare la deschiderea fisierului pentru scriere!" << endl;
		}
	}
	else
	{
		// Afisam costurile pe ecran, incluzand si tipul cheltuielii
		cout << "***** Costuri Totale Evenimente per Oras *****" << endl;
		for (const auto &oras_cost : costuri_per_oras)
		{
			cout << "Oras: " << oras_cost.first
				 << ", Tip Eveniment: " << get<1>(oras_cost.second)
				 << ", Cost Total: " << get<0>(oras_cost.second) << " RON" << endl;
		}
	}
}
// includ template
template class Gestionare_Evenimente<double, string>; //  pentru tipurile specifice (cost - int / double)
