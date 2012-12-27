#include "Joueur.h"

#include <iostream>
#include <sstream>

Joueur::~Joueur()
{
}

Joueur::Joueur(QString const& Nom, QString const& rank, std::string couleur)
    : nom(Nom), niveau(rank), pierresCapturees(0), territoire(0), m_couleur(couleur)
{}

void Joueur::setHandicap(int handi) {handicap = handi;}
void Joueur::setKomi(double k) {komi = k;}
QString Joueur::getNom() const {return nom;}
QString Joueur::getRank() const {return niveau;}
unsigned int Joueur::getCapt() const {return pierresCapturees;}
unsigned int Joueur::getTerr() const {return territoire;}
void Joueur::addCapt(unsigned int i) {pierresCapturees+=i;}
std::string Joueur::couleur() {return m_couleur;}
void Joueur::setCapt(unsigned int i) {pierresCapturees=i;}
