#define R 0.98

#include "pierre.h"


Pierre::Pierre(const Coup* c)
{
    if (c->getJoueur()->getNom()=="Kiral")
        ellipse = new QGraphicsPixmapItem(QPixmap("pierreRose.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    else if (c->getJoueur()->couleur()=="Noir")
    ellipse = new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));


    else
    {
       /* unsigned int nb = rand() % 6 +1;
        ostringstream os;
        os << "pierreBlanche" << nb << ".png";
        QString nomFichier = QString::fromStdString(os.str());*/
        ellipse = new QGraphicsPixmapItem(QPixmap("pierreBlanche.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    }
    corres = c;

    ellipse->setX((c->getAbs()+1)*E-(E*R/2));
    ellipse->setY((c->getOrd()+1)*E-(E*R/2));

    //rect.setWidth(E*0.9);
    //rect.setHeight(E*0.9);
    //les pierres font 100 pixels de diamètre, on veut qu'elles fassent E*0,9
    //ellipse->setScale(E*0.9/100);

}


void Pierre::setEllipse(QGraphicsPixmapItem* el)
{
    ellipse = el;
}
