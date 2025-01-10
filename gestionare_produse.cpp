#include "Gestionare_produse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> // Pentru setarea preciziei in calcule
#include "Verificari.h"
using namespace std;

// initializez cheltuielile pt sesiunea curenta, asociate cafenelelor
Gestionare_Produse::Gestionare_Produse()
{
	// Inițializez cheltuieli cu 0.0 pentru fiecare oraș
	cheltuieli_oras["Bucuresti"] = 0.0;
	cheltuieli_oras["Cluj-Napoca"] = 0.0;
	cheltuieli_oras["Timisoara"] = 0.0;
	cheltuieli_oras["Iasi"] = 0.0;
	cheltuieli_oras["Brasov"] = 0.0;
}

double Gestionare_Produse::get_cheltuieli_totale()
{
	return cheltuieli_totale;
}
// sunt reproduse, pe modelul de la "gestionare_angajati" si aceste metode pentru gestionarea produselor - cu adaptarile de rigoare
void Gestionare_Produse::citeste_date_produs(vector<string> fisiere_CSV)
{
	for (const string &nume_fisier : fisiere_CSV) // Parcurge fiecare fisier destinat produselor (pt Bucuresti, Iasi, Cluj, etc)
	{
		string oras = nume_fisier.substr(5, nume_fisier.find('.') - 5); // Extrag orasul din numele fisierului
		ifstream fisier(nume_fisier.c_str());
		string linie;
		getline(fisier, linie); // Sar peste antet

		while (getline(fisier, linie))
		{
			stringstream extragere_linie_fisier(linie);
			string nume_produs, cost_prod_str, cant_prod_str;
			double cost_produs;
			int cantitate_produs;
			getline(extragere_linie_fisier, nume_produs, ',');
			getline(extragere_linie_fisier, cost_prod_str, ',');
			getline(extragere_linie_fisier, cant_prod_str, ',');

			// Convertesc string-urile in valori numerice
			cost_produs = stod(cost_prod_str);		// string -> double
			cantitate_produs = stoi(cant_prod_str); // string -> int

			// Adaug produsul in vector
			produse.push_back(new Produs(nume_produs, cost_produs, cantitate_produs, oras));
		}
		fisier.close();
	}
}

void Gestionare_Produse::adauga_produs_CSV(string oras, Produs *produs)
{
    produse_orase[produs] = oras; // Adaug produsul in map-ul de asociere cu orasele (am explicat in hpp-ul aferent de ce il utilizez)
	string fisier_CSV = "stoc_" + oras + ".csv";
	ifstream fisier(fisier_CSV);

	// Verific daca fisierul este gol (nu are nici macar antet!!!)
	string linie;
	if (!getline(fisier, linie))
	{
		ofstream fisier_out(fisier_CSV, ios::app); // Deschid fisierul pentru a adauga antetul, in modul append
		if (fisier_out.is_open())
		{
			fisier_out << "DENUMIRE PRODUS,PRET PER BUCATA,NR BUCATI\n";
			fisier_out << produs->lipire_date_CSV() << endl; // Adaug primul produs
		}
		fisier_out.close();
	}
	else
	{
		ofstream fisier_out(fisier_CSV, ios::app); // Deschid fisierul pentru a adauga produsul, direct
		if (fisier_out.is_open())
		{
			fisier_out << produs->lipire_date_CSV() << endl;
		}
		fisier_out.close();
	}
}

void Gestionare_Produse::inregistrare_produs()
{
	string nume_produs;
	double cost_produs;
	int cantitate_produs;
	string oras_repartizare;
	fflush(stdin);
	cout << "Introduceti numele produsului: ";
	getline(cin, nume_produs);
	fflush(stdin);
	cout << "Introduceti costul produsului: ";
	cin >> cost_produs;
	cout << "Introduceti cantitatea disponibila: ";
	cin >> cantitate_produs;

	// Validare oras
	do
	{
		cout << "Introduceti orasul pentru stoc (Bucuresti, Cluj-Napoca, Timisoara, Iasi, Brasov): ";
		cin >> oras_repartizare;
		if (!este_oras_valid(oras_repartizare))
		{
			cout << "Orasul introdus nu este valid! Va rugam sa alegeti un oras corect.\n";
		}
	} while (!este_oras_valid(oras_repartizare));

	Produs *produs_nou = new Produs(nume_produs, cost_produs, cantitate_produs, oras_repartizare);
	produse.push_back(produs_nou);
	adauga_produs_CSV(oras_repartizare, produs_nou);
	actualizeaza_cheltuieli(produs_nou, cantitate_produs, oras_repartizare);
	cout << "Produsul a fost adaugat cu succes!" << endl;
}

void Gestionare_Produse::sterge_produs_CSV()
{
	string oras, nume_produs;
	fflush(stdin);
	cout << "***MENIUL DE ELIMINARE PRODUSE***" << endl;
	cout << "Introduceti numele produsului: ";
	getline(cin, nume_produs);

	// Validare oras
	do
	{
		cout << "Introduceti orasul in care se afla produsul (Bucuresti / Cluj-Napoca / Timisoara / Iasi / Brasov): ";
		cin >> oras;
		if (!este_oras_valid(oras))
		{
			cout << "Orasul introdus nu este valid! Va rugam sa alegeti un oras corect.\n";
		}
	} while (!este_oras_valid(oras));

	string fisier_CSV = "stoc_" + oras + ".csv";
	ifstream fisier_original(fisier_CSV.c_str());
	ofstream temp("temp_stoc.csv");
	string linie;
	string nume_produs_fisier;
	bool gasit = false;

	while (getline(fisier_original, linie))
	{
		stringstream extragere_linie_fisier(linie);
		double cost_produs;
		int cantitate_produs;
		getline(extragere_linie_fisier, nume_produs_fisier, ',');
		extragere_linie_fisier >> cost_produs;
		extragere_linie_fisier >> cantitate_produs;

		if (nume_produs_fisier != nume_produs)
		{
			temp << linie << endl;
		}
		else
		{
			gasit = true;
		}
	}

	fisier_original.close();
	temp.close();

	if (gasit)
	{
		remove(fisier_CSV.c_str());
		rename("temp_stoc.csv", fisier_CSV.c_str());
		cout << "Produsul a fost sters cu succes din stoc!\n";

	// Elimin produsul si din lista de `produse` (pt a actualiza viitoarele afisari de produse)
    for (auto index = produse.begin(); index != produse.end();)
    {
        if ((*index)->nume_produs == nume_produs && (*index)->oras == oras)
        {
            // Sterg produsul din lista + eliberez memoria
            delete *index;
            index = produse.erase(index);
        }
        else
        {
            ++index;
        }
    }
	}
	else
	{
		cout << "Produsul nu a fost gasit!\n";
		remove("temp_stoc.csv");
	}
}

void Gestionare_Produse::afiseaza_produse()
{
	// Parcurg fiecare oras din lista cheltuielilor
    for (const auto& pair : cheltuieli_oras)
    {
        string oras = pair.first;
        string fisier_CSV = "stoc_" + oras + ".csv";
        ifstream fisier(fisier_CSV.c_str());

        if (fisier.is_open())
        {
            string linie;
            getline(fisier, linie);  // Sar de antet

            cout << "\nProduse in stoc pentru orasul: " << oras << endl;
            while (getline(fisier, linie))
            {
                stringstream extragere_linie_fisier(linie);
                string nume_produs, cost_prod_str, cant_prod_str;
                double cost_produs;
                int cantitate_produs;

                getline(extragere_linie_fisier, nume_produs, ',');
                getline(extragere_linie_fisier, cost_prod_str, ',');
                getline(extragere_linie_fisier, cant_prod_str, ',');

                cost_produs = stod(cost_prod_str);
                cantitate_produs = stoi(cant_prod_str);

                // Afisare produsul
                cout << "Produs: " << nume_produs << " | Cost / buc: " << cost_produs
                     << " | Cantitate (buc): " << cantitate_produs << endl;
            }
            fisier.close();
        }
        else
        {
            cout << "Nu s-a putut deschide fisierul pentru orasul: " << oras << endl;
        }
    }

	//Am renuntat la varianta de afisare de mai jos pt vector produse, pt ca am avut probleme la scaderea stocului pt produsele comandate
	/*for (auto produs : produse)
    {
        produs->afiseaza();
    }*/
}

// metoda ce actualizeaza cheltuielile pe baza produsului si a cantitatii modificate
void Gestionare_Produse::actualizeaza_cheltuieli(Produs *produs, int cantitate_modificata, string oras)
{
	double cheltuiala_actualizare = (0.85 * produs->cost_per_unitate) * cantitate_modificata; // PRESUPUN CA AM LUAT PRODUSELE CU 15% MAI IEFTIN DE LA UN DEPOZIT
	cheltuieli_totale += cheltuiala_actualizare;											  // Cheltuielile totale la nivelul retelei

	if (cheltuieli_oras.find(oras) == cheltuieli_oras.end())
	{
		cheltuieli_oras[oras] = 0.0; // Init cheltuielile pentru acest oras - daca nu exista, fireste
	}

	cheltuieli_oras[oras] += cheltuiala_actualizare;
}

void Gestionare_Produse::afiseaza_cheltuieli_oras(bool scrie_in_fisier)
{
	ofstream raport;
	if (scrie_in_fisier)
	{
		raport.open("raport_activitate.csv");
		if (!raport.is_open())
		{
			cout << "Eroare la deschiderea fisierului raport_activitate.csv!" << endl;
			return;
		}
		raport << "TIP CHELTUIELI,ORAS,COST,RON\n";
	}
	// Range-based for pentru map (oras -> cheltuiala)
	for (auto pair : cheltuieli_oras)
	{
		const string &oras = pair.first; // Cheia din map este numele orasului
		double cheltuiala = pair.second; // Valoarea din map este cheltuiala
		if (scrie_in_fisier)
		{
			raport << "(cheltuieli) Produse - aprovizionare/adaugare stoc)," << oras << "," << fixed << setprecision(2) << cheltuiala << ",RON\n";
		}
		else
		{
			cout << "(cheltuieli) Produse - aprovizionare/adaugare stoc) | " << oras << " | " << fixed << setprecision(2) << cheltuiala << " | RON" << endl;
		}
	}

	if (scrie_in_fisier)
	{
		raport.close();
		cout << "Raportul de cheltuieli a fost scris in fisierul raport_activitate.csv!" << endl;
	}
}

void Gestionare_Produse::modifica_cantitate_produs()
{
	string linie;
	string oras, nume_prod;
	int modificare_cantitate;
	try
	{
		fflush(stdin);
		// Validare oras
		do
		{
			cout << "Din ce cafenea vrei sa modifici cantitatea unui produs? (Bucuresti / Iasi / Cluj-Napoca / Timisoara / Brasov): ";
			cin >> oras;
			if (!este_oras_valid(oras))
			{
				cout << "Orasul introdus nu este valid! Va rugam sa alegeti un oras corect.\n";
			}
		} while (!este_oras_valid(oras));
		fflush(stdin);

		cout << "La ce produs faci referire?\n";
		getline(cin, nume_prod);
		if (oras.empty() || nume_prod.empty())
		{
			throw runtime_error("Orasul sau numele produsului nu pot fi goale! Incearca din nou!");
		}

		string fisier_CSV = "stoc_" + oras + ".csv";
		ifstream fisier_original(fisier_CSV.c_str());
		if (!fisier_original.is_open())
		{
			throw runtime_error("Nu s-a putut deschide fisierul pentru citire: " + fisier_CSV + "! Incearca din nou!");
		}

		ofstream temp("temp_stoc.csv");
		if (!temp.is_open())
		{
			throw runtime_error("Nu s-a putut crea fisierul temporar! Verifica cu atentie datele!");
		}

		// Scriu antetul in fisierul temporar
		string antet;
		getline(fisier_original, antet);							 // Sar peste antet din original
		temp << "DENUMIRE PRODUS,PRET PER BUCATA,NR BUCATI" << endl; // Antetul care va fi scris in temp

		bool gasit = false;
		while (getline(fisier_original, linie))
		{
			stringstream extragere_linie_fisier(linie);
			string nume_produs_fisier, cost_prod_str, cant_prod_str;
			double cost_produs;
			int cantitate_produs, cantitate_diferenta;

			// Extrag datele produsului din fisier
			getline(extragere_linie_fisier, nume_produs_fisier, ',');
			getline(extragere_linie_fisier, cost_prod_str, ',');
			getline(extragere_linie_fisier, cant_prod_str, ',');

			try
			{
				cost_produs = stod(cost_prod_str);		// Convertire la double
				cantitate_produs = stoi(cant_prod_str); // Convertire la int
			}
			catch (const invalid_argument &e)
			{
				cout << "Eroare la conversia datelor din fisier!" << endl;
				continue;
			}

			if (nume_produs_fisier == nume_prod)
			{
				gasit = true;
				cout << "Am gasit produsul in cafeneaua din " << oras << "!\n";
				cout << "Introdu noua cantitate pentru produsul selectat: ";
				cin >> modificare_cantitate;

				if (modificare_cantitate < 0)
				{
					throw invalid_argument("Cantitatea nu poate fi negativa!");
				}

				if (modificare_cantitate == 0)
				{
					cout << "S-a eliminat intreaga cantitate! Poti sterge, ulterior, produsul din evidenta!";
				}

				if (modificare_cantitate > cantitate_produs)
				{
					cantitate_diferenta = modificare_cantitate - cantitate_produs;
					Produs temp_produs(nume_produs_fisier, cost_produs, modificare_cantitate, oras);
					actualizeaza_cheltuieli(&temp_produs, cantitate_diferenta, oras);
				}

				cantitate_produs = modificare_cantitate;
				temp << nume_produs_fisier << "," << cost_produs << "," << cantitate_produs << endl;
			}
			else
			{
				temp << linie << endl;
			}
		}

		fisier_original.close();
		temp.close();

		if (gasit)
		{
			remove(fisier_CSV.c_str());
			rename("temp_stoc.csv", fisier_CSV.c_str());
			cout << "Cantitatea produsului " << nume_prod << " a fost actualizata cu succes!\n";

			// Actualizez lista locala de produse (pt a se reflecta modificarile in afisarea tuturor produselor)
            for (auto& produs : produse)
            {
                if (produs->nume_produs == nume_prod && produs->oras == oras)
                {
                    produs->cantitate = modificare_cantitate;
                    break;
                }
            }
		}
		else
		{
			cout << "Produsul nu a fost gasit!\n";
			remove("temp_stoc.csv");
		}
	}
	catch (const exception &e)
	{
		cerr << "Eroare: " << e.what() << endl;
	}
}

Gestionare_Produse::~Gestionare_Produse()
{
	for (auto produs : produse)
	{
		delete produs;
	}
}