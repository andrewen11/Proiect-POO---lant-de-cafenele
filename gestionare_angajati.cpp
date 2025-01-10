#include "Gestionare_angajati.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> // Pentru setarea nr de zecimale corespunzatoare
#include "Verificari.h"
using namespace std;

void Gestionare_Angajati::citeste_date_angajat(const vector<string> &fisiere_CSV)
{
	for (const string &nume_fisier : fisiere_CSV) // Parcurge fiecare fisier
	{
		string oras = nume_fisier.substr(9, nume_fisier.find('.') - 9); // Extrag orasul din numele fisierului (formatul e: angajati_oras.csv - preiau de la string-ul orasului - tocmai, de aceea, caut sa extrag de la pozitia 9 pana la "."!)
		ifstream fisier(nume_fisier.c_str());
		string linie;
		getline(fisier, linie); // Sar peste antet

		while (getline(fisier, linie)) // Citim fiecare linie
		{
			stringstream extragere_linie_fisier(linie);
			string prenume, nume, functie, ora_inceput, ora_sfarsit;
			getline(extragere_linie_fisier, prenume, ',');
			getline(extragere_linie_fisier, nume, ',');
			getline(extragere_linie_fisier, functie, ',');
			getline(extragere_linie_fisier, ora_inceput, ',');
			getline(extragere_linie_fisier, ora_sfarsit, ',');

			// Adăugăm angajatul în funcție de funcție
			if (functie == "Ospatar")
			{
				angajati.push_back(new Ospatar(prenume, nume, ora_inceput, ora_sfarsit, oras)); // adaug angajatul
			}
			else if (functie == "Barista")
			{
				angajati.push_back(new Barista(prenume, nume, ora_inceput, ora_sfarsit, oras)); // adaug angajatul
			}
			else if (functie == "Manager")
			{
				angajati.push_back(new Manager(prenume, nume, ora_inceput, ora_sfarsit, oras)); // adaug angajatul
			}
		}
		fisier.close();
	}
}

void Gestionare_Angajati::sterge_angajat_CSV()
{
	string oras, nume_angajat, prenume_angajat;
	cout << "***MENIUL DE ELIMINARE ANGAJATI***" << endl;
	cout << "Introduceti prenumele angajatului: ";
	cin >> prenume_angajat;
	cout << "Introduceti numele angajatului: ";
	cin >> nume_angajat;
	// Validare oras
	do
	{
		cout << "Introduceti orasul in care activeaza angajatul (Bucuresti / Cluj-Napoca / Timisoara / Iasi / Brasov): ";
		cin >> oras;
		if (!este_oras_valid(oras))
		{
			cout << "Orasul introdus nu este valid! Va rugam sa alegeti un oras corect.\n";
		}
	} while (!este_oras_valid(oras));
	fflush(stdin);

	string fisier_CSV = "angajati_" + oras + ".csv";
	ifstream fisier_original(fisier_CSV.c_str());
	ofstream temp("temp_angajati.csv"); // Fisier temporar pentru a salva datele fara angajatul sters
	string linie;

	bool gasit = false;
	while (getline(fisier_original, linie))
	{
		stringstream extragere_linie_fisier(linie);
		string prenume, nume, functie, ora_inceput, ora_sfarsit;
		getline(extragere_linie_fisier, prenume, ',');
		getline(extragere_linie_fisier, nume, ',');
		getline(extragere_linie_fisier, functie, ',');
		getline(extragere_linie_fisier, ora_inceput, ',');
		getline(extragere_linie_fisier, ora_sfarsit, ',');

		if (nume != nume_angajat) // Dacă numele nu este cel cautat, se va copia in fisierul temporar
		{
			temp << linie << endl;
		}
		else
		{
			gasit = true;
			for (auto indexare = angajati.begin(); indexare != angajati.end(); indexare++)
			{
				if ((*indexare)->nume == nume_angajat && (*indexare)->prenume == prenume_angajat)
				{
					delete *indexare;		  // Sterg obiectul
					angajati.erase(indexare); // Elimin pointerul din vector
					break;
				}
			}
		}
	}

	fisier_original.close();
	temp.close();

	if (gasit)
	{
		remove(fisier_CSV.c_str());						 // Sterg fisierul original
		rename("temp_angajati.csv", fisier_CSV.c_str()); // Redenumesc fisierul temporar pentru a deveni, de fapt, fisierul original
		cout << "Angajatul a fost sters cu succes din baza de date!\n";
	}
	else
	{
		cout << "Angajatul nu a fost gasit!\n";
		remove("temp_angajati.csv"); // Sterg fisierul temporar daca nu s-a gasit angajatul (scriere gresita sau nu exista, de fapt, in baza de date)
	}
}

void Gestionare_Angajati::inregistrare_angajat()
{
	string prenume, nume, functie, ora_inceput, ora_sfarsit, oras_repartizare;
	cout << "Introduceti prenumele angajatului: ";
	cin >> prenume;
	cout << "Introduceti numele angajatului: ";
	cin >> nume;
	cout << "Introduceti functia (Ospatar/Barista/Manager): ";
	cin >> functie;

	// Validare ora de inceput
    do
    {
        cout << "Introduceti ora de inceput a turei (format HH:MM): ";
        cin >> ora_inceput;
        if (!este_ora_valida(ora_inceput))
        {
            cout << "Ora introdusa nu este valida! Va rugam sa alegeti un format corect (HH:MM).\n";
        }
    } while (!este_ora_valida(ora_inceput));

    // Validare ora de sfarsit
    do
    {
        cout << "Introduceti ora de sfarsit a turei (format HH:MM): ";
        cin >> ora_sfarsit;
        if (!este_ora_valida(ora_sfarsit))
        {
            cout << "Ora introdusa nu este valida! Va rugam sa alegeti un format corect (HH:MM).\n";
        }
    } while (!este_ora_valida(ora_sfarsit));

	//Validare oras
	do
	{
		cout << "Introduceti orasul in care va activa viitorul angajat (Bucuresti / Cluj-Napoca / Timisoara / Iasi / Brasov): ";
		cin >> oras_repartizare;
		if (!este_oras_valid(oras_repartizare))
		{
			cout << "Orasul introdus nu este valid! Va rugam sa alegeti un oras corect.\n";
		}
	} while (!este_oras_valid(oras_repartizare));
	fflush(stdin);

	Angajat *angajat_nou = nullptr;
	if (functie == "Ospatar")
	{
		angajat_nou = new Ospatar(prenume, nume, ora_inceput, ora_sfarsit, oras_repartizare);
	}
	else if (functie == "Barista")
	{
		angajat_nou = new Barista(prenume, nume, ora_inceput, ora_sfarsit, oras_repartizare);
	}
	else if (functie == "Manager")
	{
		angajat_nou = new Manager(prenume, nume, ora_inceput, ora_sfarsit, oras_repartizare);
	}

	if (angajat_nou != nullptr)
	{
		angajati.push_back(angajat_nou);
		adauga_angajat_CSV(oras_repartizare, angajat_nou);
		cout << "Angajatul a fost adaugat cu succes in <<angajati_" << oras_repartizare << ".csv>> !" << endl;
		angajat_nou->afiseaza();
	}
}

void Gestionare_Angajati::adauga_angajat_CSV(string oras, Angajat *angajat)
{
	string fisier_CSV = "angajati_" + oras + ".csv";
	ifstream fisier(fisier_CSV);

	// Verific daca fisierul este gol (nu are nici macar antet!!!)
	string linie;
	if (!getline(fisier, linie))
	{
		ofstream fisier_out(fisier_CSV, ios::app); // Deschid fisierul pentru a adauga antetul, in modul append
		if (fisier_out.is_open())
		{
			fisier_out << "PRENUME,NUME,TIP ANGAJAT,INCEPERE TURA,FINALIZARE TURA\n";
			fisier_out << angajat->lipire_date_CSV() << endl; // Adaug primul angajat
		}
		fisier_out.close();
	}
	else
	{
		ofstream fisier_out(fisier_CSV, ios::app); // Deschid fisierul pentru a adauga angajatul, direct
		if (fisier_out.is_open())
		{
			fisier_out << angajat->lipire_date_CSV() << endl;
		}
		fisier_out.close();
	}
}

void Gestionare_Angajati::adauga_in_raport_salariu(string fisier_CSV, bool scrie_in_fisier)
{
    string oras;
    if (fisier_CSV == "angajati_Bucuresti.csv")
    {
        oras = "Bucuresti";
    }
    else if (fisier_CSV == "angajati_Iasi.csv")
    {
        oras = "Iasi";
    }
    else if (fisier_CSV == "angajati_Cluj-Napoca.csv")
    {
        oras = "Cluj-Napoca";
    }
    else if (fisier_CSV == "angajati_Timisoara.csv")
    {
        oras = "Timisoara";
    }
    else if (fisier_CSV == "angajati_Brasov.csv")
    {
        oras = "Brasov";
    }
    else
    {
        cout << "Oras necunoscut pentru fisierul: " << fisier_CSV << endl;
        return;
    }

    ofstream raport;
    if (scrie_in_fisier)
    {
        raport.open("raport_activitate.csv", ios::app);
        if (!raport.is_open())
        {
            cout << "Nu s-a putut deschide fisierul-raport." << endl;
            return;
        }
    }

    // Citire date angajati (pt extragere + calculare timp lucru)
    ifstream fisier(fisier_CSV);
    if (!fisier.is_open())
    {
        cout << "Nu s-a putut deschide fisierul-angajati: " << fisier_CSV << endl;
        return;
    }

    string linie;
    getline(fisier, linie); // Sar de antet
    while (getline(fisier, linie))
    {
        stringstream extragere_linie_fisier(linie);
        string prenume, nume, functie, ora_inceput, ora_sfarsit;

        getline(extragere_linie_fisier, prenume, ',');
        getline(extragere_linie_fisier, nume, ',');
        getline(extragere_linie_fisier, functie, ',');
        getline(extragere_linie_fisier, ora_inceput, ',');
        getline(extragere_linie_fisier, ora_sfarsit, ',');

        // Convertire in minute -> ora_inceput & ora_sfarsit
        int ora_in, minut_in, ora_out, minut_out;
        char sep; // preiau ":"
        stringstream extragere_linie_fisier_in(ora_inceput);
        stringstream extragere_linie_fisier_out(ora_sfarsit);
        extragere_linie_fisier_in >> ora_in >> sep >> minut_in;
        extragere_linie_fisier_out >> ora_out >> sep >> minut_out;

        int minute_in = ora_in * 60 + minut_in;
        int minute_out = ora_out * 60 + minut_out;
        int minute_lucrate = minute_out - minute_in;

        // Calcul salariu / zi pe baza functiei detinute
        double tarif = 0.0;
        if (functie == "Ospatar")
        {
            tarif = 0.27;
        }
        else if (functie == "Barista")
        {
            tarif = 0.33;
        }
        else if (functie == "Manager")
        {
            tarif = 0.40;
        }

        double cost = minute_lucrate * tarif;

        // Scriere in raport sau afisare pe ecran
        if (scrie_in_fisier)
        {
            raport << "(salariu) " << prenume << " " << nume << ","
                   << oras << "," << fixed << setprecision(2) << cost << ",RON" << endl;
        }
        else
        {
            cout << "(salariu) " << prenume << " " << nume << " | "
                 << oras << " | " << fixed << setprecision(2) << cost << " | RON" << endl;
        }
    }

    fisier.close();
    if (scrie_in_fisier)
    {
        raport.close();
        cout << "Raportul a fost scris in fisierul raport_activitate.csv!" << endl;
    }
}

void Gestionare_Angajati::genereaza_raport_pentru_toate_orasele(vector<string> fisiere_CSV, bool scrie_in_fisier)
{
    for (string fisier : fisiere_CSV)
    {
        adauga_in_raport_salariu(fisier, scrie_in_fisier);
    }

    if (!scrie_in_fisier)
    {
        cout << "Raportul cheltuielilor salariale a fost generat pentru toate orasele!" << endl;
    }
}


void Gestionare_Angajati::afiseaza_angajati()
{
	for (const auto &angajat : angajati)
	{
		angajat->afiseaza();
	}
}

Gestionare_Angajati::~Gestionare_Angajati()
{
	for (auto angajat : angajati)
	{
		delete angajat;
	}
}