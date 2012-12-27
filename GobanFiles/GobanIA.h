#ifndef GOBANIA_H
#define GOBANIA_H

#include "Goban.h"
#include "../Partie/PartieIA.h"

class IA;

class GobanIA : public Goban//, boost::enable_shared_from_this<GobanIA>
{
public :
    GobanIA(double ecart, int size=9);
    bool coupPossible(int abs, int ord);
    boost::shared_ptr<PartieIA> getPartieIA() {return boost::dynamic_pointer_cast<PartieIA>(m_partie);}

    void init();
    boost::shared_ptr<GobanIA> sharedFromThis();

private :
//    boost::shared_ptr<PartieIA> m_partieIA;
};

#endif // GOBANIA_H
