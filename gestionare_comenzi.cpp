#include "Gestionare_comenzi.h"
#include "Gestionare_produse.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Verificari.h"
using namespace std;

// metoda reprodusa in mare parte, pe modelul de la "gestionare_angajati & produse" pentru gestionarea recompenselor - cu adaptarile de rigoare
int Gestionare_Comenzi::identifica_client_fidel(string nume_client, string prenume_client)
{
	vector<string> fisiere3_CSV = {
		"comenzi_Bucuresti.csv",
		"comenzi_Iasi.csv",
		"comenzi_Cluj-Napoca.csv",
		"comenzi_Timisoara.csv",
		"comenzi_Brasov.csv"};

	int count = 0, count_de_acoperit;
	double cheltuieli = 0.0, cheltuieli_de_acoperit;
	for (const string &nume_fisier : fisiere3_CSV) // Parcurge fiecare fisier destinat comenzilor (pt Bucuresti, Iasi, Cluj, etc)
	{
		ifstream fisier(nume_fisier.c_str());
		string linie;
		getline(fisier, linie); // Sar peste antet
		while (getline(fisier, linie))
		{
			stringstream extragere_linie_fisier(linie);
			string nume, prenume, produs, cantitate, pret;
			double cost_produs;
			int cantitate_produs;
			getline(extragere_linie_fisier, nume, ',');
			getline(extragere_linie_fisier, prenume, ',');
			getline(extragere_linie_fisier, produs, ',');
			getline(extragere_linie_fisier, cantitate, ',');
			getline(extragere_linie_fisier, pret, ',');

			// Convertesc string-urile in valori numerice
			cost_produs = stod(pret);			// string -> double
			cantitate_produs = stoi(cantitate); // string -> int

			if (nume_client == nume && prenume_client == prenume)
			{
				count++;
				cheltuieli = cheltuieli + cost_produs;
			}
		}
		fisier.close();
	}

	if ((count >= 7 && count < 10) || (cheltuieli > 250.00 && cheltuieli < 500.00))
	{
		cout << "Sunteti deja un client fidel RANG 2 in reteaua noastra! Beneficiati de 10 % reducere la viitoarele produse achizitionat!" << endl;
		cout << "Ati cheltuit pana acum in reteaua noastra de cafenele " << cheltuieli << "RON!" << endl;
		return 2;
	}
	else if (count >= 10 || cheltuieli > 500.00)
	{
		cout << "Sunteti deja un client fidel RANG 1 in reteaua noastra! FELICITARI! Beneficiati de 20 % reducere la viitoarele produse achizitionate!" << endl;
		cout << "Ati cheltuit pana acum in reteaua noastra de cafenele " << cheltuieli << "RON!" << endl;
		return 1;
	}
	else
	{
		count_de_acoperit = 7 - count;
		cheltuieli_de_acoperit = double(250.00 - cheltuieli);
		cout << "Momentan NU sunteti membru fidel! Mai aveti de achizitionat produse in valoare de cel putin "
			 << cheltuieli_de_acoperit << " RON sau de cumparat din reteaua noastra timp de "
			 << count_de_acoperit << " dati pentru a ajunge la RANG 2 MEMBRU!" << endl;
		return 0;
	}
}
// Afisare - toate comenzile (derulate de la momentul deschiderii sesiunii!!!)
void Gestionare_Comenzi::afiseaza_comenzi()
{
	if (comenzi.empty()) // Verif. daca lista / vect de comenzi este gol
	{
		cout << "Nu exista comenzi inregistrate pentru aceasta sesiune (zi)!" << endl;
		return;
	}
	for (Comanda *comanda : comenzi)
	{
		cout << "Comanda pentru: " << comanda->get_client_nume() << " " << comanda->get_client_prenume()
			 << " - ORAS: " << comanda->get_oras() << " - Cost total: " << comanda->calculeaza_cost_total() << " RON" << endl;
	}

}

void Gestionare_Comenzi::creare_comanda()
{
	string nume_client, prenume_client, oras;
	double reducere;

	// Citirea datelor clientului
	fflush(stdin);
	cout << "Introduceti numele clientului: ";
	getline(cin, nume_client);

	fflush(stdin);
	cout << "Introduceti prenumele clientului: ";
	getline(cin, prenume_client);

	// Citirea orasului pentru care se face comanda + validare
	do
	{
		cout << "Din ce cafenea doriti sa faceti achizitia? (Bucuresti / Cluj-Napoca / Iasi / Timisoara / Brasov etc.): ";
		cin >> oras;
		if (!este_oras_valid(oras))
		{
			cout << "Orasul introdus nu este valid! Va rugam sa alegeti un oras corect.\n";
		}
	} while (!este_oras_valid(oras));

	// Crearea comenzii folosind Factory Pattern
	Comanda *comanda_noua = ComandaFactory::creeaza_comanda(nume_client, prenume_client, oras);


	// Adaug observatorul pentru logistica
	comanda_noua->adauga_observator(&logistica_alerta);

	int input = identifica_client_fidel(nume_client, prenume_client);

		if (input == 2)
		{
			reducere = 0.9;
		}
		else if (input == 1)
		{
			reducere = 0.8;
		}
		else
		{
			reducere = 1.0; // nu se aplica reducerea (trunchierea la 90 - 80%)
		}

	// Introducerea produselor in comanda
	bool adauga_produs = true;
	bool comanda_valida = true;
	while (adauga_produs)
	{
		string nume_produs;
		int cant;
		fflush(stdin);
		cout << "Introduceti numele produsului: ";
		getline(cin, nume_produs);

		cout << "Introduceti cantitatea dorita: ";
		cin >> cant;

		// Verif. produsului in stoc (din CSV-ul asociat)
		try
		{
			Produs *produs_in_stoc = gestionare_produse->cauta_produs_in_stoc(nume_produs, oras);
			if (produs_in_stoc->cantitate < cant)
			{
				throw runtime_error("Cantitate insuficienta in stoc!");
			}

			// Scaderea cantitatii din stoc si actualizarea CSV-ului ("indirect", folosindu-ma de gestionare_produse)
			gestionare_produse->scade_stoc(nume_produs, oras, cant);
			// Adaug produsul in comanda, activez eventualele notificari, alerte din Observer daca stocul ce va ramane e mic sau nul! (sub 5 unitati setata)
			comanda_noua->adauga_produs(produs_in_stoc, cant, reducere);
			cout << "Produsul (COMANDA) " << nume_produs << " a fost adaugat si urmeaza sa fie salvat in fisier." << endl;
		}
		catch (const exception &e)
		{
			cout << "Eroare: " << e.what() << endl;
			comanda_valida = false; // Daca prod nu e valid, comanda e invalida
			break;
		}

		cout << "Vrei sa mai adaugi vreun produs? (1 - Da, 0 - Nu): ";
		cin >> adauga_produs;
	}

	// Adaug comanda in lista de comenzi - daca e valida, fireste!
	if (comanda_valida)
	{
		comenzi.push_back(comanda_noua);
		// Salvez produsul comandat in CSV-ul pt locatia de la care s a comandat
		comanda_noua->salveaza_in_fisier(oras, reducere);
		cout << "Comanda totala a fost adaugata cu succes pentru clientul " << nume_client << " " << prenume_client << endl;
	}
	else
	{
		delete comanda_noua;
		cout << "Comanda nu a fost salvata din cauza erorilor." << endl;
	}
}

Produs *Gestionare_Produse::cauta_produs_in_stoc(string nume_produs, string oras)
{
	string fisier_CSV = "stoc_" + oras + ".csv";
	ifstream fisier(fisier_CSV.c_str());
	string linie;
	getline(fisier, linie); // sar de eventualul antet (unde s-a creat deja)
	while (getline(fisier, linie))
	{
		stringstream extragere_linie_fisier(linie);
		string nume_produs_fisier, cost_prod_str, cant_prod_str;
		double cost_produs;
		int cantitate_produs;

		getline(extragere_linie_fisier, nume_produs_fisier, ',');
		getline(extragere_linie_fisier, cost_prod_str, ',');
		getline(extragere_linie_fisier, cant_prod_str, ',');

		try
		{
			cost_produs = stod(cost_prod_str);
			cantitate_produs = stoi(cant_prod_str);
		}
		catch (const invalid_argument &e)
		{
			cerr << e.what() << endl;
			continue; // Trecem la următoarea linie
		}
		if (nume_produs_fisier == nume_produs)
		{
			fisier.close();
			return new Produs(nume_produs, cost_produs, cantitate_produs, oras);
		}
	}

	fisier.close();
	throw runtime_error("Produsul nu a fost gasit in stoc!");
}

void Gestionare_Produse::scade_stoc(string nume_produs, string oras, int cantitate)
{
	string fisier_CSV = "stoc_" + oras + ".csv";
	ifstream fisier_original(fisier_CSV.c_str());
	ofstream temp("temp_stoc.csv");
	string linie;
	bool gasit = false;
	getline(fisier_original, linie);					   // sar de eventualul antet (unde s-a creat deja)
	temp << "DENUMIRE PRODUS,PRET PER BUCATA,NR BUCATI\n"; // pastrez antetul, evident, pt a evita ulterior, erori de conversie (stoi)
	while (getline(fisier_original, linie))
	{
		stringstream extragere_linie_fisier(linie);
		string nume_produs_fisier, cost_prod_str, cant_prod_str;
		double cost_produs;
		int cantitate_produs;

		getline(extragere_linie_fisier, nume_produs_fisier, ',');
		getline(extragere_linie_fisier, cost_prod_str, ',');
		getline(extragere_linie_fisier, cant_prod_str, ',');

		try
		{
			cost_produs = stod(cost_prod_str);
			cantitate_produs = stoi(cant_prod_str);
		}
		catch (const invalid_argument &e)
		{
			cout << "EROARE la conversia liniei: " << linie << " - " << e.what() << endl;
			continue; // Trecem la următoarea linie
		}

		if (nume_produs_fisier == nume_produs)
		{
			cantitate_produs -= cantitate;
			if (cantitate_produs < 0)
			{
				throw runtime_error("Stoc insuficient!");
			}
			gasit = true;
		}

		temp << nume_produs_fisier << "," << cost_produs << fixed << setprecision(2) << "," << cantitate_produs << endl;
	}

	fisier_original.close();
	temp.close();

	if (gasit)
	{
		remove(fisier_CSV.c_str());
		rename("temp_stoc.csv", fisier_CSV.c_str());
	}
	else
	{
		remove("temp_stoc.csv");
		throw runtime_error("Produsul nu a fost gasit in stoc!");
	}
}

void Gestionare_Comenzi::genereaza_raport_profit(bool scrie_in_fisier)
{
    // Resetăm profitul pe oraș
    //profit_per_oras.clear();

    // Calc profit / oras
    for (Comanda *comanda : comenzi)
    {
        string oras_comanda = comanda->get_oras();
        profit_per_oras[oras_comanda] += comanda->calculeaza_cost_total();
    }
    if (scrie_in_fisier)
    {
        ofstream fisier_raport("raport_activitate.csv", ios::app);

        if (fisier_raport.is_open())
        {
            // Verificăm dacă fișierul este gol și adăugăm antetul doar la început
            ifstream fisier_citire("raport_activitate.csv");
            string linie;
            bool este_gol = !getline(fisier_citire, linie);
            fisier_citire.close();

            if (este_gol)
            {
                fisier_raport << "TIP CHELTUIELI,ORAS,COST,RON\n";
            }

            // Scriem profitul pe fiecare oraș
            for (auto intermediar : profit_per_oras)
            {
                string oras = intermediar.first;
                double profit = intermediar.second;
                fisier_raport << "Profit (vanzari produse)," << oras << "," << profit << ",RON\n";
            }

            fisier_raport.close();
        }
    }
    else
    {
        cout << "Raport Profit Vanzari:\n";
        cout << "===========================\n";

        for (auto intermediar : profit_per_oras)
        {
            string oras = intermediar.first;
            double profit = intermediar.second;
            cout << "Oras: " << oras << ", Profit: " << profit << " RON\n";
        }
    }
}
