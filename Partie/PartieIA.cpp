#include "PartieIA.h"
#include "Partie.h"
#include "../IA/IA.h"
#include "../IA/User.h"
#include "Joueur.h"
#include "../GobanFiles/GobanIA.h"

PartieIA::PartieIA(boost::shared_ptr<GobanIA> goban, std::string couleurIA,int handicap,double komi)
    : Partie(), m_couleurIA(couleurIA), m_handicap(handicap), m_komi(komi)
{
    std::cout << "PartieIA créée" << std::endl;
    if (m_couleurIA=="noir")
    {
        m_joueurNoir.reset(new IA(sharedFromThis(),goban,"noir"));
        m_joueurBlanc.reset(new User(sharedFromThis(),goban,"blanc"));
    }
    else
    {
        m_joueurBlanc.reset(new IA(sharedFromThis(),goban,"blanc"));
        m_joueurNoir.reset(new User(sharedFromThis(),goban,"noir"));
    }
}

boost::shared_ptr<IA> PartieIA::getIA()
{
    if (m_couleurIA=="noir")
    {
        return boost::dynamic_pointer_cast<IA>(m_joueurNoir);
    }
    else return boost::dynamic_pointer_cast<IA>(m_joueurBlanc);
}

boost::shared_ptr<User> PartieIA::getUser()
{
    if (m_couleurIA=="noir")
    {
        return boost::dynamic_pointer_cast<User>(m_joueurBlanc);
    }
    else return boost::dynamic_pointer_cast<User>(m_joueurNoir);
}


std::string PartieIA::couleurAJouer()
{
    if (m_coups.size()%2==0)
    {
        //un nombre pair de coups a été joué
        if (m_handicap==0)
        {
            return "noir";
        }
        else return "blanc";
    }
    else
    {
        if (m_handicap==0)
        {
            return "blanc";
        }
        else return "noir";
    }
}
