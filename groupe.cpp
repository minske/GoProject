#include "groupe.h"

Groupe::Groupe() : statut(0), libertes(0), appartient(list<Pierre*>())
{}

bool Groupe::faitPartie(const Pierre* p) const
{
    if (find(appartient.begin(), appartient.end(),p)==appartient.end())
        return false;
    else return true;
}

void Groupe::ajouterPierre(Pierre* p)
{
    if (faitPartie(p)) throw coup_exception("La pierre appartient déjà au groupe");
    appartient.push_back(p);
}

void Groupe::capture()
{
    if (!appartient.empty())
    {
        unsigned int nb = appartient.size();
        appartient.back()->getCoup()->getJoueur()->addCapt(nb);
        for (unsigned int i = 0; i<appartient.size() ; i++)
        {

           // *(it).enleverGoban();
        }
    }
}

Groupe& Groupe::operator+=(Groupe const& g)
{
    if (g!=*this)
    {
        for (list<Pierre*>::iterator it = appartient.begin() ;it != appartient.end() ; ++it)
        {
            ajouterPierre(*it);
        }
    }

    return *this;
}

bool Groupe::operator==(Groupe const& g) const
{
    if (faitPartie(g.appartient.back()))
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
    list<Pierre*> appartient;*/

    statut = g.statut;
    libertes = g.libertes;
    for (list<Pierre*>::const_iterator it=g.appartient.begin(); it!=g.appartient.end() ; ++it)
    {
        appartient.push_back(*it);
    }
}
