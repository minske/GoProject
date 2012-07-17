#include "goban.h"

QPen Goban::pen(Qt::black,1);
QPen Goban::rouge(Qt::red,1.5);
QBrush Goban::noir(Qt::black);
QBrush Goban::blanc(Qt::white);

QPen Goban::getRouge()
{
    return rouge;
}


Goban::Goban() : QGraphicsScene(), coupCourant(0), courant(partie::iterateur(0))
{
    //brush pour la couleur de fond
    //QBrush brush(QColor(236,184,82));
    fondClair = QBrush(QPixmap("fondBoisClair.png").scaled(E*20,E*20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    fondMoyen = QBrush(QPixmap("fondBois.png").scaled(E*20,E*20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    fondFonce = QBrush(QPixmap("fondBoisFonce.png").scaled(E*20,E*20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    sansMotif = QBrush(QColor(236,184,82));
    //QBrush brush(QPixmap("fondBois.png").scaled(E*20,E*20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
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

    setBackgroundBrush(fondClair);

}

Goban::Goban(Goban const& g) : courant(0)
{
    Goban::Goban();
    /*QGraphicsItemGroup* lignes;
    set<Groupe*> groupes;
    map<pair<int,int>,Pierre*> plateau;
    QGraphicsEllipseItem* coupCourant;*/

    /*lignes=g.lignes;*/ groupes=g.groupes; plateau=g.plateau;
    coupCourant = g.coupCourant; courant=g.courant;
}


void Goban::init()
{
    removeItem(coupCourant);
    coupCourant=0; courant=0;
    for (map<pair<int,int>,Pierre*>::iterator it = plateau.begin(); it!=plateau.end(); ++it)
    {
        removeItem((*it).second->getEllipse());
    }
    groupes.clear(); plateau.clear();
}

set<Pierre*> Goban::ajouterPierre(Pierre* p)
{
    //if (coupCourant!=0) removeItem(coupCourant);


    std::cout << "\n ---------- \nCoup n°" << p->getCoup()->getNum() << std::endl;

    /* On ajoute la nouvelle pierre au map de pierres référencées par leurs coordonnées */
    const int ord = p->getCoup()->getOrd();
    const int abs = p->getCoup()->getAbs();
    pair<int,int> coord = make_pair(abs,ord);
    if (plateau.insert(pair<pair<int,int>,Pierre*>(coord,p)).second)
        std::cout << "Ajout de la pierre à la map plateau : ok" << std::endl;

    /* On regarde s'il y a des pierres autour de la pierre qu'on pose
    Si oui, on doit ajouter cette pierre à un groupe déjà existant
    Si non, on doit créer un nouveau groupe avec juste cette nouvelle pierre*/
    vector<Pierre*> autourMemeCouleur = pierresAutourMemeCouleur(p);

    switch(autourMemeCouleur.size())
    {
    case 0 :
    {
        std::cout << "Pierre isolée, création d'un nouveau groupe" << std::endl;
        Groupe* g = new Groupe();
        g->ajouterPierre(p);
        groupes.insert(g);
        break;
    }

    case 1 :
    {
        //il faut trouver le groupe auquel la pierre va être ajoutée
        std::cout << "Une pierre à côté" << std::endl;
        Groupe* g1 = trouverGroupe(autourMemeCouleur[0]);
        g1->ajouterPierre(p);
        std::cout << "Groupe de " << g1->getPierres().size() << " pierres avec "<< nbLibertes(trouverGroupe(autourMemeCouleur[0])) << " libertés." << std::endl;
        break;
    }

    case 2 :
    {
        //on doit fusionner les deux groupes
        std::cout << "Deux pierres à côté" << std::endl;
        Groupe* g1 = trouverGroupe(autourMemeCouleur[0]);
        Groupe* g2 = trouverGroupe(autourMemeCouleur[1]);

        std::cout << "Fusion de deux groupes" << std::endl;
        g1->operator +=(*g2);
        groupes.erase(g2);

        g1->ajouterPierre(p);
        //g1->supprimerDoublons();

        std::cout << "Groupe de " << g1->getPierres().size() << " pierres avec "<< nbLibertes(g1) << " libertés." << std::endl;
        break;
    }

    case 3 :
    {
        //on fusionne trois groupes
        std::cout << "Trois pierres à côté" << std::endl;
        Groupe* g1 = trouverGroupe(autourMemeCouleur[0]);
        Groupe* g2 = trouverGroupe(autourMemeCouleur[1]);
        Groupe* g3= trouverGroupe(autourMemeCouleur[2]);


        std::cout << "Fusion de trois groupes" << std::endl;
        g1->operator +=(*g2);
        g1->operator +=(*g3);
        groupes.erase(g2);
        groupes.erase(g3);

        g1->ajouterPierre(p);

        //g1->supprimerDoublons();

        std::cout << "Groupe de " << g1->getPierres().size() << " pierres avec "<< nbLibertes(g1) << " libertés." << std::endl;
        break;
    }

    case 4 :
    {
        //on fusionne quatre groupes max
        std::cout << "Quatre pierres à côté" << std::endl;
        Groupe* g1 = trouverGroupe(autourMemeCouleur[0]);
        Groupe* g2 = trouverGroupe(autourMemeCouleur[1]);
        Groupe* g3= trouverGroupe(autourMemeCouleur[2]);
        Groupe* g4 = trouverGroupe(autourMemeCouleur[3]);

        g1->operator +=(*g2);
        g1->operator +=(*g3);
        g1->operator +=(*g4);
        groupes.erase(g2); groupes.erase(g3); groupes.erase(g4);
        //g1->supprimerDoublons();
        g1->ajouterPierre(p);

        std::cout << "Groupe de " << g1->getPierres().size() << " pierres." << std::endl;
        break;
    }

    default :
        throw coup_exception("Plus de 5 pierres ou moins de 0 pierre autour : impossible.");
        break;
    }


    /****************************************** Affichage ******************************************/
    addItem(p->getEllipse());
    if (coupCourant==0)
    {
        QRect rect((abs+1)*E-(E*0.31),(ord+1)*E-(E*0.31),E*0.6,E*0.6);
        coupCourant=  this->addEllipse(rect,rouge);

    }


    /************************* Suppression des pierres *********************************************/
    //unsigned int nbcapt = 0;
    set<Pierre*> pierresSupprimees;
    vector<Pierre*> autourAdv = pierresAutourAdversaire(p);
    if (autourAdv.size()!=0)
    {
        for (vector<Pierre*>::iterator it = autourAdv.begin() ; it != autourAdv.end() ; ++it)
        {
            if (estSurPlateau(*it))
            {
                Groupe* g = trouverGroupe(*it);
                if (nbLibertes(g)==0)
                {
                    std::cout << "Un groupe à supprimer" << std::endl;
                    //nbcapt+=g->getPierres().size();
                    for (set<Pierre*>::iterator it = g->getPierres().begin(); it!=g->getPierres().end(); ++it)
                    {
                        //pour chaque pierre qui appartient au groupe, on va la stocker dans pierresSupprimees
                        pierresSupprimees.insert(*it);
                    }
                    supprimerGroupe(g);
                }
            }
        }
    }



    //return nbcapt;
    return pierresSupprimees;

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
    ostringstream os;
    os << "La pierre " << p->getCoup()->getAbs() << " - " << p->getCoup()->getAbs() << "ne fait partie d'aucun groupe";
    throw coup_exception(os.str());
}

vector<Pierre*> Goban::pierresAutourMemeCouleur(Pierre* p) const
{
    int abs = p->getCoup()->getAbs();
    int ord = p->getCoup()->getOrd();
    vector<Pierre*> resultat;

    /* On cherche à récupérer les pierres qui sont juste autour de la pierre p
    On utilise l'attribut plateau = map<pair<abs,ord>,Pierre*> */
    if ((plateau.find(pair<int,int>(abs,ord-1))!=plateau.end()))
    {
        //il existe une pierre en abs,ord-1
        Pierre* a = plateau.find(pair<int,int>(abs,ord-1))->second;
        if (a->getCoup()->getJoueur()->couleur() == p->getCoup()->getJoueur()->couleur())
            resultat.push_back(a);
    }
    if (plateau.find(pair<int,int>(abs,ord+1))!=plateau.end())
    {
        //il existe une pierre en abs,ord+1
        Pierre* a = plateau.find(pair<int,int>(abs,ord+1))->second;
        if (a->getCoup()->getJoueur()->couleur() == p->getCoup()->getJoueur()->couleur())
        resultat.push_back(a);
    }
    if (plateau.find(pair<int,int>(abs+1,ord))!=plateau.end())
    {
        //il existe une pierre en abs+1,ord
        Pierre* a = plateau.find(pair<int,int>(abs+1,ord))->second;
        if (a->getCoup()->getJoueur()->couleur() == p->getCoup()->getJoueur()->couleur())
        resultat.push_back(a);
    }
    if (plateau.find(pair<int,int>(abs-1,ord))!=plateau.end())
    {
        //il existe une pierre en abs-1,ord
        Pierre* a = plateau.find(pair<int,int>(abs-1,ord))->second;
        if (a->getCoup()->getJoueur()->couleur() == p->getCoup()->getJoueur()->couleur())
        resultat.push_back(a);
    }

    return resultat;
}


void Goban::supprimerPierre(Pierre* p)
{
    int a = p->getCoup()->getAbs();
    int o = p->getCoup()->getOrd();
    if (plateau.erase(pair<int,int>(a,o))!=1) throw coup_exception("Erreur à la suppression d'une pierre");
    Groupe* g = trouverGroupe(p);
    if (g!=0) g->supprimerPierre(p);
    removeItem(p->getEllipse());
    //delete p;
}

void Goban::supprimerGroupe(Groupe* g)
{
    set<Pierre*> ltmp = g->getPierres();
    for (set<Pierre*>::iterator it = ltmp.begin() ; it != ltmp.end() ; ++it)
    {
        supprimerPierre(*it);
    }
    groupes.erase(g);
    delete g;
}


vector<Pierre*> Goban::pierresAutourAdversaire(Pierre* p) const
{
    int abs = p->getCoup()->getAbs();
    int ord = p->getCoup()->getOrd();
    vector<Pierre*> resultat;

    /* On cherche à récupérer les pierres qui sont juste autour de la pierre p
    On utilise l'attribut plateau = map<pair<abs,ord>,Pierre*> */
    if ((plateau.find(pair<int,int>(abs,ord-1))!=plateau.end()))
    {
        //il existe une pierre en abs,ord-1
        Pierre* a = plateau.find(pair<int,int>(abs,ord-1))->second;
        if (a->getCoup()->getJoueur()->couleur() != p->getCoup()->getJoueur()->couleur())
            resultat.push_back(a);
    }
    if (plateau.find(pair<int,int>(abs,ord+1))!=plateau.end())
    {
        //il existe une pierre en abs,ord+1
        Pierre* a = plateau.find(pair<int,int>(abs,ord+1))->second;
        if (a->getCoup()->getJoueur()->couleur() != p->getCoup()->getJoueur()->couleur())
        resultat.push_back(a);
    }
    if (plateau.find(pair<int,int>(abs+1,ord))!=plateau.end())
    {
        //il existe une pierre en abs+1,ord
        Pierre* a = plateau.find(pair<int,int>(abs+1,ord))->second;
        if (a->getCoup()->getJoueur()->couleur() != p->getCoup()->getJoueur()->couleur())
        resultat.push_back(a);
    }
    if (plateau.find(pair<int,int>(abs-1,ord))!=plateau.end())
    {
        //il existe une pierre en abs-1,ord
        Pierre* a = plateau.find(pair<int,int>(abs-1,ord))->second;
        if (a->getCoup()->getJoueur()->couleur() != p->getCoup()->getJoueur()->couleur())
        resultat.push_back(a);
    }

    return resultat;
}



unsigned int Goban::nbLibertes(Groupe* g) const
{
    vector<pair<int,int> > cases;
    set<Pierre*> pierres = g->getPierres();

    for (set<Pierre*>::const_iterator it = pierres.begin() ; it!=pierres.end() ; ++it)
    {
        /* Pour chaque pierre du groupe, on regarde les quatre cases à côté
        Si elles sont vides et qu'elles ne font pas déjà partie de cases, on les ajoute au vector*/
        int abs = (*it)->getCoup()->getAbs(), ord = (*it)->getCoup()->getOrd();

        if (ord >0)
        {
            if (plateau.find(pair<int,int>(abs,ord-1))==plateau.end())
            {
                //il n'y a pas de pierre en abs,ord-1
                if (find(cases.begin(),cases.end(),pair<int,int>(abs,ord-1))==cases.end())
                    // la case ne fait pas déjà partie de cases
                    cases.push_back(pair<int,int>(abs,ord-1));
            }
        }

        if (ord<18)
        {
            if (plateau.find(pair<int,int>(abs,ord+1))==plateau.end())
            {
                //il n'y a pas de pierre en abs,ord+1
                if (find(cases.begin(),cases.end(),pair<int,int>(abs,ord+1))==cases.end())
                    // la case ne fait pas déjà partie de cases
                    cases.push_back(pair<int,int>(abs,ord+1));
            }
        }

        if (abs>0)
        {
            if (plateau.find(pair<int,int>(abs-1,ord))==plateau.end())
            {
                //il n'y a pas de pierre en abs-1,ord
                if (find(cases.begin(),cases.end(),pair<int,int>(abs-1,ord))==cases.end())
                    // la case ne fait pas déjà partie de cases
                    cases.push_back(pair<int,int>(abs-1,ord));
            }
        }

        if (abs<18)
        {
            if (plateau.find(pair<int,int>(abs+1,ord))==plateau.end())
            {
                //il n'y a pas de pierre en abs+1,ord
                if (find(cases.begin(),cases.end(),pair<int,int>(abs+1,ord))==cases.end())
                    // la case ne fait pas déjà partie de cases
                    cases.push_back(pair<int,int>(abs+1,ord));
            }
        }

    }

    return cases.size();
}


bool Goban::estSurPlateau(Pierre* p) const
{
    int abs = p->getCoup()->getAbs(), ord = p->getCoup()->getOrd();
    return (plateau.find(pair<int,int>(abs,ord))!=plateau.end());
}
