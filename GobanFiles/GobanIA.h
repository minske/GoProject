#ifndef GOBANIA_H
#define GOBANIA_H

#include "Goban.h"
#include "../Partie/PartieIA.h"

class IA;

class GobanIA : public Goban
{
public :
    GobanIA(double ecart, int size=9);
    bool coupPossible(int abs, int ord);
    boost::shared_ptr<PartieIA> getPartieIA() {return m_partieIA;}


    boost::shared_ptr<GobanIA> sharedFromThis()
    {
        return boost::dynamic_pointer_cast<GobanIA>(shared_from_this());
    }

private :
    boost::shared_ptr<PartieIA> m_partieIA;
};

#endif // GOBANIA_H
