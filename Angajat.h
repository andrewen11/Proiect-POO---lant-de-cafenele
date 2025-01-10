#ifndef ANGAJAT_H
#define ANGAJAT_H

#include <string>

class Angajat
{
public:
    std::string prenume;
	std::string nume;
    std::string functie;
    std::string ora_inceput;
    std::string ora_sfarsit;
	std::string oras;

    Angajat(std::string name, std::string surname, std::string occupation, std::string start, std::string finish, std::string city);
	virtual double getTarif() = 0;
    virtual void afiseaza();
	virtual std::string lipire_date_CSV();
    virtual ~Angajat();
};

class Ospatar : public Angajat
{
public:
    Ospatar(std::string name, std::string surname, std::string start, std::string finish, std::string city);
    void afiseaza() override;
	double getTarif() override
	{
        return 0.27;
    }
};

class Barista : public Angajat
{
public:
    Barista(std::string name, std::string surname, std::string start, std::string finish, std::string city);
    void afiseaza() override;
	double getTarif() override
	{
        return 0.33;
    }
};

class Manager : public Angajat
{
public:
    Manager(std::string name, std::string surname, std::string start, std::string finish, std::string city);
    void afiseaza() override;
	double getTarif() override
	{
        return 0.40;
    }
};

#endif // ANGAJAT_H