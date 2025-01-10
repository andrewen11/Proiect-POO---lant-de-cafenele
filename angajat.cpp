#include "Angajat.h"
#include <iostream>
using namespace std;

Angajat::Angajat(string name, string surname, string occupation, string start, string finish, string city)
{
	prenume = name;
	nume = surname;
	functie = occupation;
	ora_inceput = start;
	ora_sfarsit = finish;
	oras = city;
}

Angajat::~Angajat() {}

void Angajat::afiseaza()
{
    cout  << "Prenume: " << prenume << " | Nume: " << nume << " | Inceput tura: " << ora_inceput << " | Sfarsit tura: " << ora_sfarsit << "| Oras: "<< oras << endl;
}
string Angajat::lipire_date_CSV()
{
	return prenume + "," + nume + "," + functie + "," + ora_inceput + "," + ora_sfarsit;
}
Ospatar::Ospatar(string name, string surname, string start, string finish, string city)
	: Angajat(name, surname, "Ospatar", start, finish, city) {}


void Ospatar::afiseaza()
{
    cout << "Ospatar: " << endl;
    Angajat::afiseaza();
}

Barista::Barista(string name, string surname, string start, string finish, string city)
    : Angajat(name, surname, "Barista", start, finish, city) {}

void Barista::afiseaza()
{
    cout << "Barista: " << endl;
    Angajat::afiseaza();
}

Manager::Manager(string name, string surname, string start, string finish, string city)
    : Angajat(name, surname, "Manager", start, finish, city) {}

void Manager::afiseaza()
{
    cout << "Manager: " << endl;
    Angajat::afiseaza();
}