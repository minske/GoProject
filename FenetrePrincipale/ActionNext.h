#ifndef ACTIONNEXT_H
#define ACTIONNEXT_H

#include "FenetreSGF.h"


/***
Déclaration de la classe actionNext : hérite de QUndoCommand, permet d'annuler des coups
Garde en mémoire la pierre qui a été ajoutée, les pierres qui ont été supprimées, et un
pointeur vers la fenêtre principale pour modifier le goban.
Lorsqu'une instance d'actionNext est ajoutée à la pile UndoStack, sa méthode redo() est appelée
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
