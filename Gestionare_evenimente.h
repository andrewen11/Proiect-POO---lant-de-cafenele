#ifndef GESTIONARE_EVENIMENTE_H
#define GESTIONARE_EVENIMENTE_H

#include <string>
#include <vector>
#include <tuple>

// Def. clasa GestionareEvenimente
template <typename T1, typename T2>
class Gestionare_Evenimente {
private:
    std::vector<std::tuple<std::string, T1, std::string>> evenimente;  // Vector pentru stocarea evenimentelor

public:

    void adauga_eveniment(std::string eveniment, T1 cost, std::string oras);
    void citeste_evenimente();
    void afiseaza_evenimente() const;
    void calculeaza_costuri_evenimente(bool scrie_in_fisier = false) const;
};

#endif // GESTIONARE_EVENIMENTE_H