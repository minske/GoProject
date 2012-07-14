#define R 0.98

#include "pierre.h"
/*
QPixmap Pierre::pierreBlanche(QPixmap("pierreBlanche.png").scaled(E*0.9,E*0.9,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
QPixmap Pierre::pierreNoire(QPixmap("pierreNoir.png").scaled(E*0.9,E*0.9,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
QPixmap Pierre::pierreRose(QPixmap("pierreRose.png").scaled(E*0.9,E*0.9,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
*/
Pierre::Pierre(const coup* c)
{
    if (c->getJoueur()->getNom()=="Kiral")
        ellipse = new QGraphicsPixmapItem(QPixmap("pierreRose.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    else if (c->getJoueur()->couleur()=="Noir")
    ellipse = new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));


    else ellipse = new QGraphicsPixmapItem(QPixmap("pierreBlanche.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    corres = c;

    ellipse->setX((c->getAbs()+1)*E-(E*R/2));
    ellipse->setY((c->getOrd()+1)*E-(E*R/2));

    //rect.setWidth(E*0.9);
    //rect.setHeight(E*0.9);
    //les pierres font 100 pixels de diam�tre, on veut qu'elles fassent E*0,9
    //ellipse->setScale(E*0.9/100);

}


void Pierre::setEllipse(QGraphicsPixmapItem* el)
{
    ellipse = el;
}
