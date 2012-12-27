#include "User.h"

User::User(boost::shared_ptr<PartieIA> partie, boost::shared_ptr<GobanIA> goban, std::string couleur)
    : Joueur("ia","ia",couleur), m_partie(partie), m_goban(goban)
{
    std::cout << "User créé\n";
}
