#ifndef GESTIONARE_COMENZI_H
#define GESTIONARE_COMENZI_H

#include "comenzi.h"
#include "Gestionare_produse.h"

class Gestionare_Comenzi
{
	std::vector<Comanda *> comenzi;
	std::map<std::string, double> profit_per_oras; // Din STL, pt a face leg oras - comanda (cost)
	Logistica logistica_alerta;
	Gestionare_Produse *gestionare_produse;

public:
	void creare_comanda();
	void afiseaza_comenzi();
	int identifica_client_fidel(std::string nume_client, std::string prenume_client);
	void genereaza_raport_profit(bool scrie_in_fisier);
};

#endif // GESTIONARE_COMENZI_H