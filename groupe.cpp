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
