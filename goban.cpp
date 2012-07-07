#include "goban.h"

QPen Goban::pen(Qt::black,1);
QBrush Goban::noir(Qt::black);
QBrush Goban::blanc(Qt::white);

Goban::Goban() : QGraphicsScene()
{
    //brush pour la couleur de fond
    QBrush brush(QColor(236,184,82));
    lignes = createItemGroup(QList<QGraphicsItem*>());

    for (unsigned int i = 0; i<19 ; i++)
    {
        lignes->addToGroup(addLine(E*(i+1),E,E*(i+1),19*E));
        lignes->addToGroup(addLine(E,E*(i+1),19*E,E*(i+1)));
    }
    //ajout des hoshi
    QPen penE(Qt::black,5);
    for (unsigned int i = 0; i<3; i++)
    {
        lignes->addToGroup(addEllipse((E*4+(i*E*6))-1,E*4-1,2,2,penE,noir));
        lignes->addToGroup(addEllipse((E*4+(i*E*6))-1,E*10-1,2,2,penE,noir));
        lignes->addToGroup(addEllipse((E*4+(i*E*6))-1,E*16-1,2,2,penE,noir));
    }

    setBackgroundBrush(brush);

}

void Goban::ajouterPierre(Pierre* p)
{
    pierres.push_back(p);

    if (p->getCoup()->getJoueur()->couleur()=="Noir")
    {
        //si le coup est joué par noir, on ajoute une pierre noire sur le goban :
        addEllipse(p->getRect(),pen,noir);
        //et on met à jour la matrice plateau :
        plateau[p->getCoup()->getAbs()][p->getCoup()->getOrd()] = -1;
    }
    else
    {
        addEllipse(p->getRect(),pen,blanc);
        plateau[p->getCoup()->getAbs()][p->getCoup()->getOrd()] = 1;
    }


}
