#include <iostream>
#include <vector>
#include "Gestionare_angajati.h"
#include "Gestionare_produse.h"
#include "Gestionare_comenzi.h"
#include "Gestionare_evenimente.h"
#include "Gestionare_profit.h"
#include "Verificari.h"
using namespace std;

int main()
{
	// std::vector<Angajat> angajati; // Vector de obiecte, nu pointeri
	Gestionare_Angajati gestiune_administrare;
	Gestionare_Produse gestiune_produse;
	Gestionare_Comenzi gestiune_comenzi;
	GestionareProfituri gestiune_profit;
	vector<string> fisiere_CSV = {
		"angajati_Bucuresti.csv",
		"angajati_Iasi.csv",
		"angajati_Cluj-Napoca.csv",
		"angajati_Timisoara.csv",
		"angajati_Brasov.csv"};
	vector<string> fisiere2_CSV = {
		"stoc_Bucuresti.csv",
		"stoc_Iasi.csv",
		"stoc_Cluj-Napoca.csv",
		"stoc_Timisoara.csv",
		"stoc_Brasov.csv"};

	gestiune_administrare.citeste_date_angajat(fisiere_CSV);
	gestiune_produse.citeste_date_produs(fisiere2_CSV);
	int optiune, optiune2, rap_optiune;
	bool ruleaza = true, ruleaza2;
	Gestionare_Evenimente<double, string> gestiune_evenimente;
	cout << "BUN VENIT LA CUP'O'COFFEE SHOPS!" << endl;
	while (ruleaza)
	{
		// Afisez meniul
		cout << "\n===== Meniu Cafenea (valabil pt intreaga retea de cafenele) =====" << endl;
		cout << "1. Gestionare angajati\n";
		cout << "2. Produse si stoc\n";
		cout << "3. Comenzi\n";
		cout << "4. Evenimente speciale\n";
		cout << "5. Raport zi de lucru\n";
		cout << "0. Iesire\n";
		cout << "Selecteaza o optiune (cifra corespunzatoare optiunii, urmata de ENTER): ";
		cin >> optiune;

		// Gestionez optiunile utilizatorului
		switch (optiune)
		{
		case 1:
			cout << endl
				 << "BUN VENIT LA SECTIUNEA <<ANGAJATI>> DIN LANTUL DE CAFENELE!" << endl;
			ruleaza2 = true;

			while (ruleaza2)
			{
				cout << "\n***** OPTIUNI GESTIONARE ANGAJATI *****" << endl;
				cout << "1. Inregistreaza angajat\n";
				cout << "2. Sterge angajat\n";
				cout << "3. Afisare date angajati\n";
				cout << "4. Cauta angajat\n";
				cout << "0. Iesire\n";
				cout << "Selecteaza o optiune (cifra corespunzatoare optiunii, urmata de ENTER): ";
				cin >> optiune2;

				switch (optiune2)
				{
				case 1:
					gestiune_administrare.inregistrare_angajat();
					break;
				case 2:
					gestiune_administrare.sterge_angajat_CSV();
					break;
				case 3:
					gestiune_administrare.afiseaza_angajati();
					break;
				case 0:
					ruleaza2 = false;
					break;
				default:
					cout << "Optiune invalida. Incearca din nou.\n";
					break;
				}
			}
			break;

		case 2:
			cout << endl
				 << "BUN VENIT LA SECTIUNEA <<PRODUSE>> DIN LANTUL DE CAFENELE!" << endl;
			ruleaza2 = true;

			while (ruleaza2)
			{
				cout << "\n***** OPTIUNI GESTIONARE PRODUSE *****" << endl;
				cout << "1. Adaugare produs(e)\n";
				cout << "2. Afisare produs(e)\n";
				cout << "3. Stergere produs(e) - definitiva\n";
				cout << "4. Ajustari cantitati produse\n";
				cout << "0. Inapoi la meniul principal\n";
				cout << "Selecteaza o optiune: ";
				cin >> optiune2;

				switch (optiune2)
				{
				case 1:
					gestiune_produse.inregistrare_produs();
					break;
				case 2:
					gestiune_produse.afiseaza_produse();
					break;
				case 3:
					gestiune_produse.sterge_produs_CSV();
					break;
				case 4:
					gestiune_produse.modifica_cantitate_produs();
					break;
				case 0:
					ruleaza2 = false;
					break;
				default:
					cout << "Optiune invalida. Incearca din nou.\n";
				}
			}
			break;

		case 3:
			cout << endl
				 << "BUN VENIT LA SECTIUNEA <<COMENZI>> DIN LANTUL DE CAFENELE!" << endl;
			ruleaza2 = true;

			while (ruleaza2)
			{
				cout << "\n***** OPTIUNI GESTIONARE COMENZI *****" << endl;
				cout << "1. Adauga comanda noua\n";
				cout << "2. Afisare comenzi\n";
				cout << "0. Inapoi la meniul principal\n";
				cout << "Selecteaza o optiune: ";
				cin >> optiune2;

				switch (optiune2)
				{
				case 1:
					gestiune_comenzi.creare_comanda();
					break;
				case 2:
					gestiune_comenzi.afiseaza_comenzi();
					break;
				case 0:
					ruleaza2 = false;
					break;
				default:
					cout << "Optiune invalida. Incearca din nou.\n";
				}
			}
			break;

		case 4:
		{
			string eveniment, oras;
			double cost;
			cout << endl
				 << "BUN VENIT LA SECTIUNEA <<EVENIMENTE>> DIN LANTUL DE CAFENELE!" << endl;
			ruleaza2 = true;

			while (ruleaza2)
			{
				cout << "\n***** OPTIUNI GESTIONARE EVENIMENTE *****" << endl;
				cout << "1. Adauga eveniment(e)\n";
				cout << "2. Afisare evenimente\n";
				cout << "0. Inapoi la meniul principal\n";
				cout << "Selecteaza o optiune: ";
				cin >> optiune2;

				switch (optiune2)
				{
				case 1:
				{
					char optiune_eveniment = 'y'; // Redenumim variabila pentru a evita conflictul;
					while (optiune_eveniment == 'y' || optiune_eveniment == 'Y')
					{
						cout << "Introduceti numele evenimentului: ";
						getline(cin >> ws, eveniment);

						cout << "Introduceti costul evenimentului: ";
						cin >> cost;

						// Citirea orasului pentru care se face comanda + validare
						do
						{
							cout << "Introduceti orasul cafenelei (Bucuresti / Cluj-Napoca / Iasi / Timisoara / Brasov etc): ";
							cin >> oras;
							if (!este_oras_valid(oras))
							{
								cout << "Orasul introdus nu este valid! Va rugam sa alegeti un oras corect.\n";
							}
						} while (!este_oras_valid(oras));

						gestiune_evenimente.adauga_eveniment(eveniment, cost, oras);

						cout << "Doriti sa adaugati un alt eveniment? (y/n): ";
						cin >> optiune_eveniment;
					}
					break;
				}
				case 2:
					gestiune_evenimente.citeste_evenimente();
					gestiune_evenimente.afiseaza_evenimente();
					break;
				case 0:
					ruleaza2 = false;
					break;
				default:
					cout << "Optiune invalida. Incearca din nou.\n";
				}
			}
			break;
		}

		case 5:
{
    cout << endl
         << "BUN VENIT LA SECTIUNEA <<RAPORT>> DIN LANTUL DE CAFENELE!" << endl;
    ruleaza2 = true;

    while (ruleaza2)
    {
        cout << "\n***** OPTIUNI RAPORT ACTIVITATE *****" << endl;
        cout << "1. Inchidere sesiune activitate + generare raport final (zi) complet\n";
        cout << "0. Inapoi la meniul principal\n";
        cout << "Selecteaza o optiune: ";
        cin >> optiune2;

        if (optiune2 == 1)
        {
            cout << "ATENTIE! ODATA CE ATI SELECTAT ACEASTA OPTIUNE, NU VA MAI PUTETI INTOARCE SA FACETI VREO MODIFICARE IN ACEASTA SESIUNE!";
            cout << "\nEvident, se poate deschide o sesiune noua, dar evenimentele si modificarile vechi de stoc nu vor mai fi tinute in evidenta!";
            cout << "\nEsti SIGUR ca vrei sa inchizi ACUM sesiunea? (1 - DA, 0 - NU) -> ";
            cin >> rap_optiune;

            if (rap_optiune == 1)
            {
                cout << "Se analizeaza datele colectate pentru generarea raportului final..." << endl;

				gestiune_produse.afiseaza_cheltuieli_oras(true);
                gestiune_administrare.genereaza_raport_pentru_toate_orasele(fisiere_CSV, true);
                gestiune_comenzi.genereaza_raport_profit(true);
                gestiune_evenimente.calculeaza_costuri_evenimente(true);
				gestiune_profit.adauga_totaluri_si_mesaj("raport_activitate.csv", "total_profituri.csv");
                // Afisarea mesajului final
                cout << "\n***** RAPORTUL COMPLET DETALIAT PENTRU INTREG LANTUL DE CAFENELE A FOST CREAT "
                     << "; POATE FI VIZUALIZAT IN raport_activitate.csv si total_profituri.csv! *****" << endl;
				exit(0); // Inchid imediat programul

            }
            else
            {
                cout << "Ai selectat NU. Se revine la meniu..." << endl;
            }
        }
        else if (optiune2 == 0)
        {
            ruleaza2 = false;
        }
        else
        {
            cout << "Optiune inexistenta!" << endl;
        }
    }
    break;
}

		case 0:
			ruleaza = false;
			break;

		default:
			cout << "Optiune invalida. Incearca din nou!\n";
			break;
		}
	}

	remove("evenimente.csv");
	return 0;
}