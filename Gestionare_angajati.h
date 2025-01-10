#ifndef GESTIONARE_ANGAJATI_H
#define GESTIONARE_ANGAJATI_H

#include <vector>
#include "Angajat.h"

class Gestionare_Angajati
{
private:
    std::vector<Angajat*> angajati;

public:
	void inregistrare_angajat();
    void adauga_angajat_CSV(std::string oras, Angajat* angajat);
    void sterge_angajat_CSV();
    void citeste_date_angajat(const std::vector<std::string>& fisiere_CSV);
    void afiseaza_angajati();
	void adauga_in_raport_salariu(std::string fisier_CSV, bool scrie_in_fisier);
	void genereaza_raport_pentru_toate_orasele(std::vector<std::string> fisiere_CSV, bool scrie_in_fisier);
    ~Gestionare_Angajati();
};

#endif // GESTIONARE_ANGAJATI_H