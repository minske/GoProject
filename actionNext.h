#ifndef ACTIONNEXT_H
#define ACTIONNEXT_H

#include "FP.h"


/***
D�claration de la classe actionNext : h�rite de QUndoCommand, permet d'annuler des coups
Garde en m�moire la pierre qui a �t� ajout�e, les pierres qui ont �t� supprim�es, et un
pointeur vers la fen�tre principale pour modifier le goban.
Lorsqu'une instance d'actionNext est ajout�e � la pile UndoStack, sa m�thode redo() est appel�e
et modifie le goban.
****/

class actionNext : public QUndoCommand
{
public :
    actionNext(FP* f);
    ~actionNext() {}
    int id() const {return 42;}
    bool mergeWith(const QUndoCommand* other);
    void redo();
    void undo();
    //QString getLogMsg() const { return logMsg;}

private :
    Pierre* m_pierre;
    set<Pierre*> pierresSupprimees;
    FP* fp;
    //QString logMsg;

};

#endif // ACTIONNEXT_H
