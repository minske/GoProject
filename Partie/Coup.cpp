#include <sstream>

#include "Coup.h"
#include "../Tools/CoupException.h"

using namespace std;

Coup::Coup(std::string const& s, std::string com) :  m_numero(0)
{

    // un coup donné dans le fichier est normalement de la forme B[mc]BL[151.84]OB[11]C[commentaires ...]
    /*
    On cherche à extraire :
    l'abscisse et l'ordonnée du coup
    on ne s'occupe pas du temps et des commentaires pour le moment
    */
    if (s.size() < 5) throw coup_exception("Fichier invalide !\n");
    //les abscisses et ordonnées vont de 0 à 18 en commençant en haut à gauche

    int p=com.size();
    for (int n=0; n<p; n++)
    {
        if (com[n]=='&') com.replace(n,1,1,'\n');
    }
    m_commentaires=QString::fromStdString(com);
    m_abscisse = s[2]-'a';
    m_ordonnee = s[3]-'a';
    m_joueur.reset();
}

string Coup::print() const
{
    std::ostringstream r;
    r << "Coup n°" << m_numero << " - " <<m_joueur->couleur() << " : " << m_abscisse+1 << "-" << m_ordonnee+1;
    return r.str();
}

Coup::~Coup(){}

void Coup::addComm(QString const& s)
{
    m_commentaires.append(s);
}

