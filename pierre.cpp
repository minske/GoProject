#include "pierre.h"

Pierre::Pierre(const coup* c)
{
    if (c->getJoueur()->getNom()=="Kiral")
        ellipse = new QGraphicsPixmapItem(QPixmap("pierreRose.png"));

    else if (c->getJoueur()->couleur()=="Noir")
    ellipse = new QGraphicsPixmapItem(QPixmap("pierreNoire.png"));

    else ellipse = new QGraphicsPixmapItem(QPixmap("pierreBlanche.png"));
    corres = c;

    ellipse->setX((c->getAbs()+1)*E-(E*0.45));
    ellipse->setY((c->getOrd()+1)*E-(E*0.45));

    //rect.setWidth(E*0.9);
    //rect.setHeight(E*0.9);
    //les pierres font 100 pixels de diamètre, on veut qu'elles fassent E*0,9
    ellipse->setScale(E*0.9/100);

}


void Pierre::setEllipse(QGraphicsPixmapItem* el)
{
    ellipse = el;
}
