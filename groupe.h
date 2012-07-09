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
    Groupe(Groupe const& g);
    int getStatut() const {return statut;}
    unsigned int nbLibertes() const {return libertes;}
    list<Pierre*> getPierres() const {return appartient;}
    void capture();
    void ajouterPierre(Pierre* p);
    bool faitPartie(const Pierre* p) const;
    Groupe& operator+=(Groupe const& g);
    bool operator!=(Groupe const& g) const;
    bool operator==(Groupe const& g) const;
    //void supprimerDoublons();

};

Groupe operator+(Groupe const& a, Groupe const& b);

#endif // GROUPE_H
