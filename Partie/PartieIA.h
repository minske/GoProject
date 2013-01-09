#ifndef PARTIEIA_H
#define PARTIEIA_H
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Partie.h"

class IA;
class User;
class GobanIA;
class Joueur;
//class Partie;

class PartieIA : public Partie
{
public :
    PartieIA(std::string couleurIA="blanc",int handicap=0,double komi=6.5);
    bool partieFinie() {std::cout << "partie finie ?\n"; return false; } ///TODO
    std::string couleurAJouer();
    void init(boost::shared_ptr<GobanIA> gobanPtr);
    std::string getCouleurIA() {return m_couleurIA;}
    int getHandicap() {return m_handicap;}
    double getKomi() {return m_komi;}

    boost::shared_ptr<IA> getIA();
    boost::shared_ptr<User> getUser();


    boost::shared_ptr<PartieIA> sharedFromThis();


private :
    int m_handicap;
    double m_komi;
    std::string m_couleurIA;
};


#endif // PARTIEIA_H
