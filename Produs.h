#ifndef PRODUS_H
#define PRODUS_H

#include <string>

class Produs
{
public:
	std::string nume_produs;
	double cost_per_unitate;
	std::string oras;
	int cantitate;

	Produs(std::string product_name, double price_per_unit, int quantity, std::string city);
	void afiseaza();
	std::string lipire_date_CSV();
};

#endif // PRODUS_H