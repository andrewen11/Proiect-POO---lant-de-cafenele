#include "Produs.h"
#include <iostream>
using namespace std;

Produs::Produs(string product_name, double price_per_unit, int quantity, string city)
{
	nume_produs = product_name;
	cost_per_unitate = price_per_unit;
	cantitate = quantity;
	oras = city;
}

// afisez informatii despre produs (unde e cazul)
void Produs::afiseaza()
{
	cout << "Produs: " << nume_produs << " | Cost / buc: " << cost_per_unitate << " | Cantitate (buc): " << cantitate << " | Oras: " << oras << endl;
}

// salvez produsul in CSV
string Produs::lipire_date_CSV()
{
	return nume_produs + "," + to_string(cost_per_unitate) + "," + to_string(cantitate);
}
