#include "Verificari.h"
#include <sstream>
#include <vector>
using namespace std;

// functie pt validarea oraselor (general)
bool este_oras_valid(string oras)
{
	vector<string> orase_valide = {"Bucuresti", "Cluj-Napoca", "Timisoara", "Iasi", "Brasov"};
	for (auto oras_valid : orase_valide)
	{
		if (oras == oras_valid)
		{
			return true;
		}
	}
	return false;
}

bool este_ora_valida(string ora)
{
	if (ora.length() != 5 || ora[2] != ':')
	{
		return false;
	}

	int ora_int, minute_int;
	char separator;
	stringstream extragere_string(ora);

	extragere_string >> ora_int >> separator >> minute_int; // Extragem orele și minutele

	// Verif dacă orele + minutele sunt valide
	if (extragere_string.fail())
		return false;

	if (ora_int < 0 || ora_int > 23)
		return false;

	if (minute_int < 0 || minute_int > 59)
		return false;

	return true;
}
