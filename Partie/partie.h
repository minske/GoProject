#ifndef PARTIE_H_INCLUDED
#define PARTIE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/enable_shared_from_this.hpp>

#include "Joueur.h"
#include "Coup.h"



class Partie
{
public :
    Partie() : m_date(QString()), m_resultat(QString()), m_contenuFichier(std::string())  {}
    void init(QString const& noirNom, QString const& blancNom, QString const& noirNiveau, QString const& blancNiveau, QString const& PartieDate);

    std::vector<Coup> getListeCoups() const {return m_coups;}
    void ajouterCoup(Coup const& c);
    Coup getCoup(int numero) const {return m_coups.at(numero);}

    boost::shared_ptr<Blanc> getBlanc() const {return m_joueurBlanc;}
    boost::shared_ptr<Noir> getNoir() const {return m_joueurNoir;}


    std::string infos() const;
    ~Partie();

    QString getDate() const {return m_date;}
    QString getResultat() const {return m_resultat;}

    void chargerFichier(std::string const& nomFichier);
    void enregistrerFichier(QString nomFich);


private :
    std::vector<Coup> m_coups;
    boost::shared_ptr<Blanc> m_joueurBlanc;
    boost::shared_ptr<Noir> m_joueurNoir;
    QString m_date;
    QString m_resultat;
    std::string m_contenuFichier;
};

std::ostream& operator<<(std::ostream& f, Coup const& c);


#endif // PARTIE_H_INCLUDED
