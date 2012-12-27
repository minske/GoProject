#include "GobanIA.h"
#include "../IA/IA.h"
#include "../Tools/CoupException.h"

GobanIA::GobanIA(double ecart, int size) : Goban(ecart,size)
{
    std::cout << "GobanIA en cours de création\n";
    boost::shared_ptr<PartieIA> partiePtr (new PartieIA());
    std::cout << "eeeeeeeeeeeee";
//    partiePtr->init(sharedFromThis());
    std::cout << "oooo";
    m_partie = partiePtr;
    std::cout << "okokokok\n";
}

void GobanIA::init()
{
    try
    {
        std::cout << "init goban ia";
        boost::shared_ptr<PartieIA> partiePtr = boost::dynamic_pointer_cast<PartieIA>(m_partie);
        if (partiePtr.get()==0) throw coup_exception("\n!!!!!! partie NULL !!!!!\n"); ///ERREUR
        boost::shared_ptr<GobanIA> shrdThis = sharedFromThis();
        std::cout << "hahahahahahahahahah\n";
        if (shrdThis.get()!=0)
            partiePtr->init(shrdThis);
        else throw coup_exception("sharedFromThis de GobanIA = NULL");
    }
    catch(std::exception const& e)
    {
        std::cout << e.what();
        throw std::exception(e);
    }
}

bool GobanIA::coupPossible(int abs, int ord)
{
    std::pair<int,int> coord (abs,ord);
    std::map<std::pair<int,int>,boost::shared_ptr<Pierre> > plateau = getPlateau();

    if ((plateau.find(coord)!=plateau.end()) || (abs < 0) || (abs > 9) || (ord < 0) || (ord > 9))
    {
        return false;
    }
    else return true;
    ///TODO à terminer
}

boost::shared_ptr<GobanIA> GobanIA::sharedFromThis()
{
    try
    {
        std::cout << "NOOOOOOOOOOOOOOOOOOOOOOOOOO\n";
        return boost::shared_dynamic_cast<GobanIA,Goban>(shared_from_this());
    }
    catch(std::exception const& e)
    {
        std::cout << "Impossible de créer un shared ptr à partir de GobanIA : " << e.what();
        throw std::exception(e);
    }
}
