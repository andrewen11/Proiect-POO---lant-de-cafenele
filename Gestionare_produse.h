#ifndef GESTIONARE_PRODUSE_H
#define GESTIONARE_PRODUSE_H

#include <vector>
#include <map>
#include "Produs.h"

class Gestionare_Produse
{
private:
	std::vector<Produs *> produse; // vector de produse, dinamic
	std::map<std::string, double> cheltuieli_oras; // map intre oras si cheltuielile aferente
	std::map<Produs*, std::string> produse_orase; // map / leg. pt asocierea produselor cu orasele aferente (pt retinerea oraselor in care se gasesc produsele salvate in csv-uri!)
	double cheltuieli_totale = 0.0;

    // pentru actualizarea cheltuielilor
    void actualizeaza_cheltuieli(Produs* produs, int cantitate_modificata, std::string oras);

public:
	Gestionare_Produse();
	void inregistrare_produs();
	void adauga_produs_CSV(std::string oras, Produs *produs);
	void sterge_produs_CSV();
	void citeste_date_produs(std::vector<std::string> fisiere_CSV);
	void afiseaza_produse();
	void modifica_cantitate_produs();
	Produs* cauta_produs_in_stoc(std::string nume_produs, std::string oras);
	void scade_stoc(std::string nume_produs, std::string oras, int cantitate);
    double get_cheltuieli_totale(); // Getter pentru cheltuieli totale
	void afiseaza_cheltuieli_oras(bool scrie_in_fisier);
	~Gestionare_Produse();
};

#endif // GESTIONARE_PRODUSE_H