#ifndef GROUPE_H
#define GROUPE_H
#include "pierre.h"
#include <list>

class Groupe
{
    int statut; //0 pour mort, 1 pour vivant
    unsigned int libertes;
    list<Pierre*> appartient;

public :
    Groupe();
    int getStatut() const {return statut;}
    unsigned int nbLibertes() const {return libertes;}
    void capture();
    void ajouterPierre(Pierre* p);
    bool faitPartie(const Pierre* p) const;

};

#endif // GROUPE_H
