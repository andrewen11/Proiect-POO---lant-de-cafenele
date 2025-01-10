#ifndef GESTIONARE_PROFITURI_H
#define GESTIONARE_PROFITURI_H

#include <string>
#include <map>

class GestionareProfituri {
public:
    void citeste_raportul_din_fisier(std::string fisier, std::map<std::string, double>& totaluri_pe_orase);
    void scrie_totaluri_si_mesaj_in_fisier(std::string fisier, std::map<std::string, double>& totaluri_pe_orase);
    void adauga_totaluri_si_mesaj(std::string fisier_raport, std::string fisier_profituri);
};

#endif // GESTIONARE_PROFITURI_H
