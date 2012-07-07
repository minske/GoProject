#ifndef GOBAN_H
#define GOBAN_H
#include <QtGui>
#include "pierre.h"

class Goban : public QGraphicsScene
{
public :
    Goban();
    void ajouterPierre(Pierre* p);

private :
    static QBrush noir;
    static QBrush blanc;
    static QPen pen;
    QGraphicsItemGroup* lignes;
    QList<Pierre*> pierres;
    //plateau = matrice 19x19 de integer : 0 pour rien, -1 pour Noir, 1 pour Blanc
    int plateau[19][19];
};

#endif // GOBAN_H
