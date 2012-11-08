#ifndef PIERRE_H
#define PIERRE_H
#include <QtGUI>
#include <QRect>
#include <boost/shared_ptr.hpp>
#include "groupe.h"
#include "partie.h"
//#define E 31
#define R 0.98


class Pierre
{
public :
    Pierre(Coup c);

    Coup getCoup() const {return m_coup;}
    Coup getCoup() {return m_coup;}
    const QRect& getRect() const {return rect;}
    boost::shared_ptr<QGraphicsPixmapItem> getEllipse() const {return ellipse;}
    boost::shared_ptr<Groupe> getGroupe() {return m_groupe.lock();}
    std::string getName() const {return m_name;}

    int libertes() const;
    vector<pair<int,int> > intersectionsAdjacentes() const;
    //vector<boost::shared_ptr<Pierre> > pierresAutourAdversaire() const;
    vector<boost::shared_ptr<Pierre> > pierresAutourMemeCouleur() const;

    void setEllipse(boost::shared_ptr<QGraphicsPixmapItem> el);
    void setGroupe(boost::shared_ptr<Groupe> groupePtr) {m_groupe = boost::weak_ptr<Groupe>(groupePtr);}
    ~Pierre() {}

    std::string couleur() const {return m_coup.getJoueur()->couleur().toStdString();}

private :
    QRect rect;
    Coup m_coup;
    boost::shared_ptr<QGraphicsPixmapItem> ellipse;
    boost::weak_ptr<Groupe> m_groupe;

    std::string m_name;
};

#endif // PIERRE_H
