#ifndef ACTIONNEXT_H
#define ACTIONNEXT_H

#include "FP.h"

class actionNext : public QUndoCommand
{
public :
    actionNext(set<Groupe*> grp, map<pair<int,int>,Pierre*> plt, Goban* gb, Pierre* pr)
        : QUndoCommand(), Ngroupes(grp), Nplateau(plt), g(gb), p(pr)
    { setText("Next"); }
    ~actionNext() {}
    int id() const {return 42;}
    bool mergeWith(const QUndoCommand* other)
    {
        return false;
    }

    void redo()
    {

    }

    void undo()
    {
        g->setPlateau(Nplateau); g->setGroupes(Ngroupes);
        g->supprimerPierre(p);
        g->reculer();
    }

private :
    set<Groupe*> Ngroupes;
    map<pair<int,int>,Pierre*> Nplateau;
    Goban* g;
    Pierre* p;

};

#endif // ACTIONNEXT_H
