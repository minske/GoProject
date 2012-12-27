#ifndef JOUEUR_H
#define JOUEUR_H
#include <QtGui>

#include <boost/shared_ptr.hpp>

class Joueur
{
protected :
    unsigned int territoire;
    QString nom;
    QString niveau;
    unsigned int pierresCapturees;
    unsigned int handicap;
    double komi;
    std::string m_couleur;

public :
    Joueur(QString const& Nom, QString const& rank, std::string couleur);
    void setHandicap(int handi);
    void setKomi(double k);
    QString getNom() const;
    QString getRank() const;
    unsigned int getCapt() const;
    unsigned int getTerr() const;
    void addCapt(unsigned int i);
    std::string couleur();
    virtual ~Joueur();
    void setCapt(unsigned int i);
};

#endif
