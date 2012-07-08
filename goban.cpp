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
    /* On ajoute la nouvelle pierre au map de pierres référencées par leurs coordonnées */
    const int ord = p->getCoup()->getOrd();
    const int abs = p->getCoup()->getAbs();
    pair<int,int> coord = make_pair(abs,ord);
    plateau.insert(pair<pair<int,int>,Pierre*>(coord,p));
    std::cout << "Ajout de la pierre à la map plateau : ok" << std::endl;

    /* On regarde s'il y a des pierres autour de la pierre qu'on pose
    Si oui, on doit ajouter cette pierre à un groupe déjà existant
    Si non, on doit créer un nouveau groupe avec juste cette nouvelle pierre*/
    vector<Pierre*> autour = pierresAutour(p);

    if (autour.size()==0) // la nouvelle pierre n'appartient à aucun groupe
    {
        std::cout << "Pierre isolée, création d'un nouveau groupe" << std::endl;
        Groupe* g = new Groupe();
        g->ajouterPierre(p);
        groupes.insert(g);
    }
    else if (autour.size()==1)
    {
        //il faut trouver le groupe auquel la pierre va être ajoutée
        std::cout << "Une pierre à côté" << std::endl;
        trouverGroupe(autour[0])->ajouterPierre(p);
    }
    else if (autour.size()==2)
    {
        //on doit fusionner les deux groupes
        Groupe* g1 = new Groupe();
        g1 = trouverGroupe(autour[0]);
        Groupe* g2 = new Groupe();
        g2 = trouverGroupe(autour[1]);

        Groupe* g3 = new Groupe();
        g3 = &(*g1 + *g2);
        groupes.erase(g1);
        delete g1;
        groupes.erase(g2);
        delete g2;
        groupes.insert(g3);
    }

    /* Affichage */
    if (p->getCoup()->getJoueur()->couleur()=="Noir")
    {
        //si le coup est joué par noir, on ajoute une pierre noire sur le goban :
        p->setEllipse(this->addEllipse(p->getRect(),pen,noir));
        //et on met à jour la matrice plateau :
       // plateau[abs][ord] = -1;
    }
    else
    {
        p->setEllipse(this->addEllipse(p->getRect(),pen,blanc));
        //plateau[abs][ord] = 1;
    }



}


vector<Pierre*> Goban::pierresAutour(Pierre* p) const
{
    int abs = p->getCoup()->getAbs();
    int ord = p->getCoup()->getOrd();
    vector<Pierre*> resultat;

    /* On cherche à récupérer les pierres qui sont juste autour de la pierre p
    On utilise l'attribut plateau = map<pair<abs,ord>,Pierre*> */
    if (plateau.find(pair<int,int>(abs,ord-1))!=plateau.end())
    {
        //il existe une pierre en abs,ord-1
        resultat.push_back(plateau.find(pair<int,int>(abs,ord-1))->second);
    }
    if (plateau.find(pair<int,int>(abs,ord+1))!=plateau.end())
    {
        //il existe une pierre en abs,ord+1
        resultat.push_back(plateau.find(pair<int,int>(abs,ord+1))->second);
    }
    if (plateau.find(pair<int,int>(abs+1,ord))!=plateau.end())
    {
        //il existe une pierre en abs+1,ord
        resultat.push_back(plateau.find(pair<int,int>(abs+1,ord))->second);
    }
    if (plateau.find(pair<int,int>(abs-1,ord))!=plateau.end())
    {
        //il existe une pierre en abs-1,ord
        resultat.push_back(plateau.find(pair<int,int>(abs-1,ord))->second);
    }

    return resultat;
}


Groupe* Goban::trouverGroupe(Pierre* p) const
{
    for(set<Groupe*>::iterator it=groupes.begin() ; it!=groupes.end() ; ++it)
    {
        if ((*it)->faitPartie(p)) return *it;
    }
    throw coup_exception("Cette pierre ne fait partie d'aucun groupe");
}
