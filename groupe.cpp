#include "groupe.h"

Groupe::Groupe() : statut(0), appartient(set<Pierre*>())
{}

bool Groupe::faitPartie(const Pierre* p) const
{
    return (find(appartient.begin(), appartient.end(),p)!=appartient.end());
}

void Groupe::ajouterPierre(Pierre* p)
{
    if (faitPartie(p)) throw coup_exception("La pierre appartient déjà au groupe");
    appartient.insert(p);
}

void Groupe::capture()
{}

Groupe& Groupe::operator+=(Groupe const& g)
{
    for (set<Pierre*>::const_iterator it = g.appartient.begin() ;it != g.appartient.end() ; ++it)
    {
        if (!faitPartie(*it))
            appartient.insert(*it);
    }

    return *this;
}

bool Groupe::operator==(Groupe const& g) const
{
    if (faitPartie(*g.appartient.begin()))
        return true;
    else return false;
}

bool Groupe::operator!=(Groupe const& g) const
{
    return !(*this==g);
}

Groupe operator+(Groupe const& a, Groupe const& b)
{
    Groupe c(a);
    c+=b;
    return c;
}


Groupe::Groupe(Groupe const& g)
{
    /*int statut; //0 pour mort, 1 pour vivant
    unsigned int libertes;
    set<Pierre*> appartient;*/

    statut = g.statut;
    for (set<Pierre*>::const_iterator it=g.appartient.begin(); it!=g.appartient.end() ; ++it)
    {
        appartient.insert(*it);
    }
}
