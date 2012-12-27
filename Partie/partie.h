#ifndef PARTIE_H_INCLUDED
#define PARTIE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/enable_shared_from_this.hpp>

#include "Joueur.h"
#include "Coup.h"
#include <boost/enable_shared_from_this.hpp>


class Partie : public boost::enable_shared_from_this<Partie>
{
public :
    Partie() : m_date(QString()), m_resultat(QString()), m_contenuFichier(std::string())
    {
        std::cout << "Partie créée\n";
    }
    void init(QString const& noirNom, QString const& blancNom, QString const& noirNiveau, QString const& blancNiveau, QString const& PartieDate);

    std::vector<Coup> getListeCoups() const {return m_coups;}
    void ajouterCoup(Coup const& c);
    Coup getCoup(int numero) const {return m_coups.at(numero);}

    boost::shared_ptr<Joueur> getBlanc() const {return m_joueurBlanc;}
    boost::shared_ptr<Joueur> getNoir() const {return m_joueurNoir;}
    boost::shared_ptr<Joueur> getJoueur(std::string couleur) const
    {
        if (couleur=="noir") return m_joueurNoir;
        if (couleur=="blanc") return m_joueurBlanc;
    }

    std::string infos() const;
    ~Partie();

    QString getDate() const {return m_date;}
    virtual QString getResultat() const {return m_resultat;}

    void chargerFichier(std::string const& nomFichier);
    void enregistrerFichier(QString nomFich);


protected :
    std::vector<Coup> m_coups;
    boost::shared_ptr<Joueur> m_joueurBlanc;
    boost::shared_ptr<Joueur> m_joueurNoir;
    QString m_date;
    QString m_resultat;
    std::string m_contenuFichier;
};

std::ostream& operator<<(std::ostream& f, Coup const& c);


#endif // PARTIE_H_INCLUDED
