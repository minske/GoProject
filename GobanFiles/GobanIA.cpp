#include "GobanIA.h"
#include "../IA/IA.h"

GobanIA::GobanIA(double ecart, int size) : Goban(ecart,size)
{
    std::cout << "GobanIA créé\n";
    m_partieIA.reset(new PartieIA(sharedFromThis()));
}

bool GobanIA::coupPossible(int abs, int ord)
{
    std::pair<int,int> coord (abs,ord);
    std::map<std::pair<int,int>,boost::shared_ptr<Pierre> > plateau = getPlateau();

    if (plateau.find(coord)!=plateau.end())
    {
        return false;
    }
    else return true;
    ///TODO à terminer
}
