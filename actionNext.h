#ifndef ACTIONNEXT_H
#define ACTIONNEXT_H

#include "FP.h"


class actionNext : public QUndoCommand
{
public :
    actionNext(FP* f);
    ~actionNext() {}
    int id() const {return 42;}
    bool mergeWith(const QUndoCommand* other);
    void redo();
    void undo();

private :
    //A pour Ancien, N pour Nouveau
    Pierre* m_pierre;
   set<Pierre*> pierresSupprimees;
    FP* fp;

};

#endif // ACTIONNEXT_H
