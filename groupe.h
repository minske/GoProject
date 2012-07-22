#ifndef GROUPE_H
#define GROUPE_H
#include "pierre.h"
#include <set>

class Groupe
{
    int statut; //0 pour mort, 1 pour vivant
    //unsigned int libertes;
    set<Pierre*> appartient;

public :
    Groupe();
    Groupe(Groupe const& g);
    int getStatut() const {return statut;}
    unsigned int nbLibertes() const;
    set<Pierre*>& getPierres() {return appartient;}
    void capture();
    void ajouterPierre(Pierre* p);
    bool faitPartie(const Pierre* p) const;
    Groupe& operator+=(Groupe const& g);
    bool operator!=(Groupe const& g) const;
    bool operator==(Groupe const& g) const;
    void supprimerPierre(Pierre* p) { appartient.erase(p);}
    void print() const;
};

Groupe operator+(Groupe const& a, Groupe const& b);

#endif // GROUPE_H
