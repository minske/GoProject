#ifndef GROUPE_H
#define GROUPE_H

#include <boost/enable_shared_from_this.hpp>
#include <set>
#include <vector>

class Pierre;
class Goban;

class Groupe : public boost::enable_shared_from_this<Groupe>
{
    int statut; //0 pour mort, 1 pour vivant
    //unsigned int libertes;
    std::vector<boost::shared_ptr<Pierre> > m_pierres;
    boost::weak_ptr<Goban> m_goban;

public :
    Groupe();
    Groupe(Groupe const& g);
    int getStatut() const {return statut;}
    unsigned int nbLibertes() const;
    std::vector<boost::shared_ptr<Pierre> >& getPierres() {return m_pierres;}
    void capture();
    void ajouterPierre(boost::shared_ptr<Pierre> p);
    void ajouterGroupe(boost::shared_ptr<Groupe> g);
    bool faitPartie(const boost::shared_ptr<Pierre> p) const;
    Groupe& operator+=(Groupe const& g);
    bool operator!=(Groupe const& g) const;
    bool operator==(Groupe const& g) const;
    void supprimerPierre(boost::shared_ptr<Pierre> p);
    void print() const;
    std::string printToString() const;
    std::string couleur() const;

    boost::shared_ptr<Goban> getGoban() const;
    void setGoban(boost::shared_ptr<Goban> gobanPtr);
};

Groupe operator+(Groupe const& a, Groupe const& b);

#endif // GROUPE_H
