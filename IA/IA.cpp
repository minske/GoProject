#include "IA.h"
#include "../Partie/PartieIA.h"
#include "../GobanFiles/GobanIA.h"

IA::IA(boost::shared_ptr<PartieIA> partie, boost::shared_ptr<GobanIA> goban, std::string couleur)
    : Joueur("ia","ia",couleur), m_partie(partie), m_goban(goban)
{
    std::cout << "IA créée\n";
}

std::pair<int,int> IA::choixCoup()
{
    int abs = -1, ord = -1;

    while (!(m_goban.lock()->coupPossible(abs,ord)))
    {
        std::cout << "o\n";
        abs = rand() % 9;
        ord = rand() % 9;
    }

    return std::pair<int,int>(abs,ord);
}
