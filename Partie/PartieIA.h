#ifndef PARTIEIA_H
#define PARTIEIA_H
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Partie.h"

class IA;
class User;
class GobanIA;

class PartieIA : public Partie
{
public :
    PartieIA(boost::shared_ptr<GobanIA> goban, std::string couleurIA="noir",int handicap=0,double komi=6.5);
    bool partieFinie() {return false; } ///TODO
    std::string couleurAJouer();

    std::string getCouleurIA() {return m_couleurIA;}
    int getHandicap() {return m_handicap;}
    double getKomi() {return m_komi;}

    boost::shared_ptr<IA> getIA();
    boost::shared_ptr<User> getUser();

    boost::shared_ptr<PartieIA> sharedFromThis()
    {
        return boost::dynamic_pointer_cast<PartieIA>(Partie::shared_from_this());
    }

private :
    int m_handicap;
    double m_komi;
    std::string m_couleurIA;
};


#endif // PARTIEIA_H
