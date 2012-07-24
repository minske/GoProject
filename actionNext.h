#ifndef ACTIONNEXT_H
#define ACTIONNEXT_H

#include "FP.h"


/***
Déclaration de la classe actionNext : hérite de QUndoCommand, permet d'annuler des coups
Garde en mémoire la pierre qui a été ajoutée, les pierres qui ont été supprimées, et un
pointeur vers la fenêtre principale pour modifier le goban.
Lorsqu'une instance d'actionNext est ajoutée à la pile UndoStack, sa méthode redo() est appelée
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
