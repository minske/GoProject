#ifndef GOBAN_H
#define GOBAN_H
#include <QtGui>
#include "pierre.h"
#include "groupe.h"
#include <set>
#include <map>

class Goban : public QGraphicsScene
{
public :
    static Goban* getInstance();
    static void deleteInstance();

    vector<boost::shared_ptr<Pierre> > ajouterPierre(boost::shared_ptr<Pierre> p); //renvoie les pierres qui ont été supprimées du goban
    vector<boost::shared_ptr<Pierre> > pierresSansLibertes() const;
    set<boost::shared_ptr<Groupe> > groupesSansLiberte() const;

    // GETTERS
    map<pair<int,int>,boost::shared_ptr<Pierre> >& getPlateau()  {return plateau;}
    QBrush getBrushFonce() const {return Goban::fondFonce;}
    QBrush getBrushMoyen() const {return fondMoyen;}
    QBrush getBrushClair() const {return fondClair;}
    QBrush getBrushSansMotif() const {return sansMotif;}
    static QPen getRouge();
    QGraphicsItemGroup* getLignes() const {return lignes;}
    partie::iterateur getCourant() {return courant;}
    boost::shared_ptr<QGraphicsEllipseItem> getCoupCourant() {return coupCourant;}
    set<boost::shared_ptr<Groupe> >& getGroupes() {return m_groupes;}

    ~Goban() {/*delete lignes;*/}


    void supprimerPierre(boost::shared_ptr<Pierre> p);
    bool estSurPlateau(boost::shared_ptr<Pierre> p) const;
    void init();
    void avancer() {++courant;}
    void reculer() {--courant;}

    // SETTERS
    void setCourant(partie::iterateur it) {courant=it;}
    void setPlateau(map<pair<int,int>,boost::shared_ptr<Pierre> > pl) {plateau=pl;}
    void setCoupCourant(boost::shared_ptr<QGraphicsEllipseItem> q) {coupCourant=q;}


    std::string printPlateau() const;

    static unsigned int SIZE() {return M_SIZE;}

private :
    Goban();
    Goban(Goban const& g);

    static QBrush noir;
    static QBrush blanc;
    static QPen pen;
    static QPen rouge;
    QGraphicsItemGroup* lignes;

    map<pair<int,int>,boost::shared_ptr<Pierre> > plateau;
    set<boost::shared_ptr<Groupe> > m_groupes;

    boost::shared_ptr<QGraphicsEllipseItem> coupCourant;
    partie::iterateur courant;
    QBrush fondClair;
    QBrush fondMoyen;
    QBrush fondFonce;
    QBrush sansMotif;
    QString logMsg;

    static unsigned int M_SIZE;
    static Goban* m_instance;

    //plateau : pierres avec pair<abs,ord> comme clé (pair<int,int>)
};

#endif // GOBAN_H
