#ifndef GOBAN_H
#define GOBAN_H
#include <QtGui>
#include "pierre.h"
#include "groupe.h"
#include <set>
#include <map>

class Goban : public QGraphicsScene
{
public :
    Goban();
    void ajouterPierre(Pierre* p);
    vector<Pierre*> pierresAutour(Pierre* p) const;
    Groupe* trouverGroupe(Pierre* p) const;
    set<Groupe*> getGroupes() const {return groupes;}
    map<pair<int,int>,Pierre*> getPlateau() const {return plateau;}

private :
    static QBrush noir;
    static QBrush blanc;
    static QPen pen;
    QGraphicsItemGroup* lignes;
    set<Groupe*> groupes;
    map<pair<int,int>,Pierre*> plateau;
    //plateau : pierres avec pair<abs,ord> comme clé (pair<int,int>)
};

#endif // GOBAN_H
