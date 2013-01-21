#ifndef PIERRE_H
#define PIERRE_H
#include <QtGUI>
#include <QRect>
#include <boost/shared_ptr.hpp>

#include "groupe.h"
#include "../Partie/partie.h"

#define R 0.98

class Goban;

class Pierre
{
public :
    Pierre(Coup c, int ecartGoban);
    Pierre(Pierre const& p, bool copyGroupe=false);
    Pierre(boost::shared_ptr<Pierre> p, bool copyGroupe=false);

    Coup getCoup() const {return m_coup;}
    Coup getCoup() {return m_coup;}
    const QRect& getRect() const {return rect;}
    boost::shared_ptr<QGraphicsPixmapItem> getEllipse() const {return ellipse;}
    boost::shared_ptr<Groupe> getGroupe() {return m_groupe.lock();}
    std::string getName() const {return m_name;}

    int libertes() const;
    std::vector<std::pair<int,int> > intersectionsAdjacentes() const;
    std::vector<std::pair<int,int> > intersectionsAdjacentes(int gobanSize) const;
    std::vector<boost::shared_ptr<Pierre> > pierresAutourMemeCouleur() const;

    int libertes(boost::shared_ptr<Goban> gobanPtr) const;
    std::vector<std::pair<int,int> > intersectionsAdjacentes(boost::shared_ptr<Goban> gobanPtr) const;
    std::vector<boost::shared_ptr<Pierre> > pierresAutourMemeCouleur(boost::shared_ptr<Goban> gobanPtr) const;

    void setEllipse(boost::shared_ptr<QGraphicsPixmapItem> el);
    void setGroupe(boost::shared_ptr<Groupe> groupePtr) {m_groupe = boost::weak_ptr<Groupe>(groupePtr);}
    ~Pierre() {}

    std::string couleur() const {return m_coup.getJoueur()->couleur();}


private :
    QRect rect;
    Coup m_coup;
    boost::shared_ptr<QGraphicsPixmapItem> ellipse;
    boost::weak_ptr<Groupe> m_groupe;
    std::string m_name;
};

#endif // PIERRE_H
