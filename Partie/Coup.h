#ifndef COUP_H
#define COUP_H

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <qstring.h>

#include "../Partie/Joueur.h"

class Coup : public boost::enable_shared_from_this<Coup>
{
    int m_abscisse;
    int m_ordonnee;
    boost::shared_ptr<Joueur> m_joueur;
    int m_numero;
    QString m_commentaires;


public :
    Coup() : m_abscisse(0), m_ordonnee(0), m_numero(0), m_commentaires(QString()) {}
    Coup(int abs, int ord, boost::shared_ptr<Joueur> jou) : m_abscisse(abs), m_ordonnee(ord), m_joueur(jou) {}
    Coup(int abs, int ord) : m_abscisse(abs), m_ordonnee(ord) {}
    Coup(std::string const& s, std::string com=std::string());


    std::string print() const;

    int getAbs() const {return m_abscisse;}
    int getOrd() const {return m_ordonnee;}
    boost::shared_ptr<Joueur> getJoueur() const {return m_joueur;}

    void setJoueur(boost::shared_ptr<Joueur> J) {m_joueur = J;}
    void setNum(int n) {m_numero = n;}
    int getNum() const {return m_numero;}
    QString getComm() const {return m_commentaires;}
    void addComm(QString const& s);

    std::string couleur() const {return m_joueur->couleur();}

    ~Coup();
};

#endif // COUP_H
