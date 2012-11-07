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
    actionNext();
    actionNext(boost::shared_ptr<FP> f);
    ~actionNext() {}
    int id() const {return 42;}
    bool mergeWith(const boost::shared_ptr<QUndoCommand> other);
    void redo();
    void undo();
    //QString getLogMsg() const { return logMsg;}
    void setFpPtr(boost::shared_ptr<FP> fpPtr) {fp = fpPtr;}

private :
    boost::shared_ptr<Pierre> m_pierre;
    vector<boost::shared_ptr<Pierre> > pierresSupprimees;
    boost::shared_ptr<FP> fp;
    //QString logMsg;

};

#endif // ACTIONNEXT_H
