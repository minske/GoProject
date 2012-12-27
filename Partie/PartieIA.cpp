#include "PartieIA.h"
#include "Partie.h"
#include "../IA/IA.h"
#include "../IA/User.h"
#include "Joueur.h"
#include "../GobanFiles/GobanIA.h"
#include "../Tools/CoupException.h"

PartieIA::PartieIA(std::string couleurIA,int handicap,double komi)
    : Partie(), m_couleurIA(couleurIA), m_handicap(handicap), m_komi(komi)
{
}

void PartieIA::init(boost::shared_ptr<GobanIA> gobanPtr)
{
    std::cout << "init partie ia\n";

    try
    {
        std::cout << "hmhmhm\n";
        if (m_couleurIA=="noir")
        {
            m_joueurNoir.reset(new IA(sharedFromThis(),gobanPtr,"noir"));
            m_joueurBlanc.reset(new User(sharedFromThis(),gobanPtr,"blanc"));
        }
        else
        {
            std::cout << "hmmmm" << std::endl;

            m_joueurBlanc.reset(new IA(sharedFromThis(),gobanPtr,"blanc"));
            m_joueurNoir.reset(new User(sharedFromThis(),gobanPtr,"noir"));
        }
    }
    catch(std::exception e)
    {
        std::cout << "Impossible d'initialiser la partieIA : " << e.what();
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
    std::cout << "couleur a jouer, nb coups = " << m_coups.size() << std::endl;
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


boost::shared_ptr<PartieIA> PartieIA::sharedFromThis()
{
    return boost::shared_dynamic_cast<PartieIA,Partie>(shared_from_this());
}
