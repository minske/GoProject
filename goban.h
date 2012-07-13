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
    Goban(Goban const& g);
    unsigned int ajouterPierre(Pierre* p);
    vector<Pierre*> pierresAutour(Pierre* p) const;
    vector<Pierre*> pierresAutourMemeCouleur(Pierre* p) const;
    vector<Pierre*> pierresAutourAdversaire(Pierre* p) const;
    Groupe* trouverGroupe(Pierre* p) const;
    set<Groupe*> getGroupes() const {return groupes;}
    map<pair<int,int>,Pierre*> getPlateau() const {return plateau;}
    ~Goban() {delete lignes;}
    void supprimerGroupe(Groupe* g);
    void supprimerPierre(Pierre* p);
    unsigned int nbLibertes(Groupe* g) const;
    bool estSurPlateau(Pierre* p) const;
    void init();
    partie::iterateur getCourant() const {return courant;}
    void avancer() {++courant;}
    void reculer() {--courant;}
    void setCourant(partie::iterateur it) {courant=it;}

private :
    static QBrush noir;
    static QBrush blanc;
    static QPen pen;
    static QPen rouge;
    QGraphicsItemGroup* lignes;
    set<Groupe*> groupes;
    map<pair<int,int>,Pierre*> plateau;
    QGraphicsEllipseItem* coupCourant;
    partie::iterateur courant;

    //plateau : pierres avec pair<abs,ord> comme clé (pair<int,int>)
};

#endif // GOBAN_H
