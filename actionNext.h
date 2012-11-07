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
