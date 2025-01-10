#ifndef COMENZI_H
#define COMENZI_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Produs.h"

// Observer Pattern: O cls care observa schimbarile de stoc (si le alerteaza, in gestiune)
class Observer
{
public:
	virtual void actualizeaza(std::string mesaj) = 0;
};

// Clasa de notificare pentru logistica - derivata din "observer"
class Logistica : public Observer
{
public:
	void actualizeaza(std::string mesaj) override
	{
		std::cout << "[Logistica] " << mesaj << std::endl;
	}
};

//Comanda - cls centrala din acest header file
class Comanda
{
	//private
	std::string client_nume;
	std::string client_prenume;
	std::map<Produs *, int> produse_comandate; // Produs si cantitatea comandata
	double cost_total;
	std::vector<Observer *> observatori;
	std::string oras_comanda;

public:
	Comanda(std::string nume, std::string prenume, std::string oras)
	{
		client_prenume = prenume;
		client_nume = nume;
		oras_comanda = oras;
		cost_total = 0;
	}

	void adauga_produs(Produs *produs, int cantitate, double reducere);
	void notifica_observatori(std::string);
	void adauga_observator(Observer *observer);
	double calculeaza_cost_total();
	void salveaza_in_fisier(std::string oras, double reducere);
	std::string get_client_nume();
	std::string get_client_prenume();
	std::string get_oras();
};

// Factory Pattern - pentru crearea comenzilor, prin metoda statica de creeaza_comanda de tipul Comanda
class ComandaFactory
{
public:
	static Comanda *creeaza_comanda(std::string nume, std::string prenume, std::string oras);
};
#endif // COMENZI_H