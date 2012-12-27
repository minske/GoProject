#ifndef IA_H
#define IA_H
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "../Partie/Joueur.h"
#include "../Partie/PartieIA.h"

class IA : public Joueur
{
private :
//    std::string m_couleur;
    boost::weak_ptr<PartieIA> m_partie;
    boost::weak_ptr<GobanIA> m_goban;

public :
    IA(boost::shared_ptr<PartieIA> partie, boost::shared_ptr<GobanIA> goban, std::string couleur="blanc");
    std::pair<int,int> choixCoup();
};

#endif // IA_H
