#ifndef ACTIONNEXT_H
#define ACTIONNEXT_H

#include "FenetreSGF.h"


/***
D�claration de la classe actionNext : h�rite de QUndoCommand, permet d'annuler des coups
Garde en m�moire la pierre qui a �t� ajout�e, les pierres qui ont �t� supprim�es, et un
pointeur vers la fen�tre principale pour modifier le goban.
Lorsqu'une instance d'actionNext est ajout�e � la pile UndoStack, sa m�thode redo() est appel�e
et modifie le goban.
****/

class ActionNext : public QUndoCommand
{
public :
    ActionNext();
    ActionNext(boost::shared_ptr<FenetreSGF> f);
    ~ActionNext() {}
    int id() const {return 42;}
    bool mergeWith(const boost::shared_ptr<QUndoCommand> other);
    void redo();
    void undo();
    //QString getLogMsg() const { return logMsg;}
    void setFenetrePrincipalePtr(boost::shared_ptr<FenetreSGF> fpPtr) {fp = fpPtr;}

private :
    boost::shared_ptr<Pierre> m_pierre;
    std::vector<boost::shared_ptr<Pierre> > pierresSupprimees;
    boost::weak_ptr<FenetreSGF> fp;
    //QString logMsg;

};

#endif // ActionNext_H
