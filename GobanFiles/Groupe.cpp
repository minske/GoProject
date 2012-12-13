#include "groupe.h"
#include "pierre.h"
#include "../Tools/CoupException.h"

using namespace std;

Groupe::Groupe() : statut(0)
{}


unsigned int Groupe::nbLibertes() const
{
    unsigned int result=0;

    for (vector<boost::shared_ptr<Pierre> >::const_iterator it = m_pierres.begin(); it != m_pierres.end() ; ++it)
    {
        result += (*it)->libertes();
    }

    return result;
}

bool Groupe::faitPartie(const boost::shared_ptr<Pierre> p) const
{
    return (find(m_pierres.begin(), m_pierres.end(),p)!=m_pierres.end());
}

void Groupe::ajouterPierre(boost::shared_ptr<Pierre> p)
{
    if (faitPartie(p))
    {
        ostringstream res;
        res << "La pierre " << p->getCoup().getAbs() << "-" << p->getCoup().getOrd() << " appartient déjà au groupe";
        throw coup_exception(res.str());
    }
    m_pierres.push_back(p);
    p->setGroupe(this->shared_from_this());
}

void Groupe::ajouterGroupe(boost::shared_ptr<Groupe> g)
{
    cout << "Groupe::ajouterGroupe" << endl;
    cout << "this.size()=" << m_pierres.size() << endl;
    if (g.get()!=0)
    {
    cout << g->getPierres().size() << " pierres a ajouter\n";
    int i = 1;
    for (vector<boost::shared_ptr<Pierre> >::iterator it = g->getPierres().begin(); it != g->getPierres().end(); it++)
    {
        cout << "pierre " << i << endl;
        boost::shared_ptr<Pierre> pierrePtr = *it;
        cout << "ajouter pierre : " <<pierrePtr->getCoup().print() << endl;
        ajouterPierre(pierrePtr);
        pierrePtr->setGroupe(this->shared_from_this());
        i++;
    }
    cout << "fin ajouter Groupe\n";
    }
    else
    {
        cout << "Pointeur sur groupe à ajouter nul" << endl;
    }
}

void Groupe::capture()
{}




Groupe::Groupe(Groupe const& g)
{
    /*int statut; //0 pour mort, 1 pour vivant
    unsigned int libertes;
    set<Pierre*> m_pierres;*/

    statut = g.statut;
    for (vector<boost::shared_ptr<Pierre> >::const_iterator it=g.m_pierres.begin(); it!=g.m_pierres.end() ; ++it)
    {
        m_pierres.push_back(*it);
    }
}

void Groupe::print() const
{
    if (m_pierres.size()!=0)
    {
        for (vector<boost::shared_ptr<Pierre> >::const_iterator it=m_pierres.begin(); it!=m_pierres.end(); ++it)
            std::cout << (*it)->getCoup().getAbs() <<"-" << (*it)->getCoup().getOrd() <<" + ";
    }
    else std::cout << "Groupe vide";
}

std::string Groupe::printToString() const
{
    std::ostringstream os;

    if (m_pierres.size()!=0)
    {
        os << couleur() << " - ";
        for (vector<boost::shared_ptr<Pierre> >::const_iterator it=m_pierres.begin(); it!=m_pierres.end(); ++it)
            os << (*it)->getCoup().getAbs() <<"-" << (*it)->getCoup().getOrd() <<" + ";
    }
    else os << "Groupe vide";

    return os.str();
}

std::string Groupe::couleur() const
{
    if (!m_pierres.empty())
    {
        return m_pierres[0]->couleur();
    }
}

boost::shared_ptr<Goban> Groupe::getGoban() const
{
    return m_goban.lock();
}

void Groupe::setGoban(boost::shared_ptr<Goban> gobanPtr)
{
    m_goban = boost::weak_ptr<Goban>(gobanPtr);
}
