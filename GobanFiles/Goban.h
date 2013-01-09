#ifndef GOBAN_H
#define GOBAN_H

#include <QtGui>
#include <set>
#include <map>

#include "pierre.h"
#include "groupe.h"
#include "../Partie/Partie.h"

class Goban : public QGraphicsScene, public boost::enable_shared_from_this<Goban>
{
public :
    Goban(double ecart, int size=19);
    Goban(Goban const& g);

    std::vector<boost::shared_ptr<Pierre> > ajouterPierre(boost::shared_ptr<Pierre> p, bool trueGoban=true); //renvoie les pierres qui ont été supprimées du goban
    std::vector<boost::shared_ptr<Pierre> > pierresSansLibertes() const;
    std::set<boost::shared_ptr<Groupe> > groupesSansLiberte() const;

    // GETTERS
    std::map<std::pair<int,int>,boost::shared_ptr<Pierre> > getPlateau() const;
    QBrush getBrushFonce() const {return Goban::fondFonce;}
    QBrush getBrushMoyen() const {return fondMoyen;}
    QBrush getBrushClair() const {return fondClair;}
    QBrush getBrushSansMotif() const {return sansMotif;}
    static QPen getRouge();
    QGraphicsItemGroup* getLignes() const {return lignes;}
    //partie::iterateur getCourant() {return courant;}
    int getCourant() {return m_courant;}
    boost::shared_ptr<QGraphicsEllipseItem> getCoupCourant() {return coupCourant;}
    std::set<boost::shared_ptr<Groupe> >& getGroupes() {return m_groupes;}

    ~Goban() {/*delete lignes;*/}


    void supprimerPierre(boost::shared_ptr<Pierre> p);
    bool estSurPlateau(boost::shared_ptr<Pierre> p) const;
    void init();
    void avancer() {++m_courant;}
    void reculer() {--m_courant;}
    bool partieNonCommencee() const {return (m_courant==-1);}
    bool partieTerminee() const {return (m_courant!=-1) && (m_courant==m_partie->getListeCoups().size());}

    // SETTERS
    void setCourant(int num) {m_courant=num;}
    void setCoupCourant(boost::shared_ptr<QGraphicsEllipseItem> q) {coupCourant=q;}


    std::string printPlateau() const;

    unsigned int SIZE() {return M_SIZE;}
    double ECART() {return M_ECART;}

    boost::shared_ptr<Partie> getPartie() const {return m_partie;}
    void setPartie(boost::shared_ptr<Partie> p) {m_partie = p;}

    bool hasPartie() const {return m_hasPartie;}
    void setHasPartie(bool p) {m_hasPartie=p;}

protected :
    std::string m_name; //"SGF" ou "JEU"
    bool m_hasPartie;

    static QBrush noir;
    static QBrush blanc;
    static QPen pen;
    static QPen rouge;
    QGraphicsItemGroup* lignes;

    std::set<boost::shared_ptr<Groupe> > m_groupes;

    boost::shared_ptr<QGraphicsEllipseItem> coupCourant;
    int m_courant;
    QBrush fondClair;
    QBrush fondMoyen;
    QBrush fondFonce;
    QBrush sansMotif;
    QString logMsg;

    unsigned int M_SIZE;
    double M_ECART;

    boost::shared_ptr<Partie> m_partie;
};

#endif // GOBAN_H
