#include "goban.h"
#include "debug.h"
#include "FP.h"

QPen Goban::pen(Qt::black,1);
QPen Goban::rouge(Qt::red,1.5);
QBrush Goban::noir(Qt::black);
QBrush Goban::blanc(Qt::white);
Goban* Goban::m_instance = 0;

Goban* Goban::getInstance()
{
    if (m_instance==0)
    {
        m_instance =  new Goban();
    }

    return m_instance;
}

void Goban::deleteInstance()
{
    if (m_instance!=0) delete m_instance;
}


unsigned int Goban::M_SIZE = 19;

QPen Goban::getRouge()
{
    return rouge;
}


Goban::Goban() : QGraphicsScene()
{
    //brush pour la couleur de fond
    //QBrush brush(QColor(236,184,82));
    fondClair = QBrush(QPixmap("fondBoisClair.png").scaled(FP::ECART_T()*(M_SIZE+1),FP::ECART_T()*(M_SIZE+1),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    fondMoyen = QBrush(QPixmap("fondBois.png").scaled(FP::ECART_T()*(M_SIZE+1),FP::ECART_T()*(M_SIZE+1),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    fondFonce = QBrush(QPixmap("fondBoisFonce.png").scaled(FP::ECART_T()*(M_SIZE+1),FP::ECART_T()*(M_SIZE+1),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    sansMotif = QBrush(QColor(236,184,82));

    lignes = createItemGroup(QList<QGraphicsItem*>());

    for (unsigned int i = 0; i<M_SIZE ; i++)
    {
        lignes->addToGroup(addLine(FP::ECART_T()*(i+1),FP::ECART_T(),FP::ECART_T()*(i+1),M_SIZE*FP::ECART_T()));
        lignes->addToGroup(addLine(FP::ECART_T(),FP::ECART_T()*(i+1),M_SIZE*FP::ECART_T(),FP::ECART_T()*(i+1)));
    }
    //ajout des hoshi
    QPen penE(Qt::black,5);

    /// HOSHIS à placer en fonction de la taille du goban
    switch (M_SIZE)
    {
        case 19 :
        {
            for (unsigned int i = 0; i<3; i++)
            {
                lignes->addToGroup(addEllipse((FP::ECART_T()*4+(i*FP::ECART_T()*6))-1,FP::ECART_T()*4-1,2,2,penE,noir));
                lignes->addToGroup(addEllipse((FP::ECART_T()*4+(i*FP::ECART_T()*6))-1,FP::ECART_T()*10-1,2,2,penE,noir));
                lignes->addToGroup(addEllipse((FP::ECART_T()*4+(i*FP::ECART_T()*6))-1,FP::ECART_T()*16-1,2,2,penE,noir));
            }

            break;
        }

        case 13 :
        {
            lignes->addToGroup(addEllipse((FP::ECART_T()*4)-1,FP::ECART_T()*4-1,2,2,penE,noir));
            lignes->addToGroup(addEllipse((FP::ECART_T()*4)-1,FP::ECART_T()*10-1,2,2,penE,noir));
            lignes->addToGroup(addEllipse((FP::ECART_T()*10)-1,FP::ECART_T()*4-1,2,2,penE,noir));
            lignes->addToGroup(addEllipse((FP::ECART_T()*10)-1,FP::ECART_T()*10-1,2,2,penE,noir));
            lignes->addToGroup(addEllipse((FP::ECART_T()*7)-1,FP::ECART_T()*7-1,2,2,penE,noir));

            break;
        }

        case 9 :
        {
            lignes->addToGroup(addEllipse(FP::ECART_T()*3-1,FP::ECART_T()*3-1,2,2,penE,noir));
            lignes->addToGroup(addEllipse(FP::ECART_T()*3-1,FP::ECART_T()*7-1,2,2,penE,noir));
            lignes->addToGroup(addEllipse(FP::ECART_T()*7-1,FP::ECART_T()*3-1,2,2,penE,noir));
            lignes->addToGroup(addEllipse(FP::ECART_T()*7-1,FP::ECART_T()*7-1,2,2,penE,noir));
            lignes->addToGroup(addEllipse(FP::ECART_T()*5-1,FP::ECART_T()*5-1,2,2,penE,noir));

            break;
        }
    }

    setBackgroundBrush(fondClair);

}

Goban::Goban(Goban const& g)
{
    Goban::Goban();
    /*QGraphicsItemGroup* lignes;
    set<Groupe*> groupes;
    map<pair<int,int>,Pierre*> plateau;
    QGraphicsEllipseItem* coupCourant;*/

    /*lignes=g.lignes;*/
    //groupes=g.groupes;
    plateau=g.plateau;
    coupCourant = g.coupCourant; courant=g.courant;
    M_SIZE = g.M_SIZE;
}

void Goban::init()
{
    /*if (coupCourant!=0) removeItem(coupCourant.get());
    coupCourant=0; courant=0;*/
    for (map<pair<int,int>,boost::shared_ptr<Pierre> >::iterator it = plateau.begin(); it!=plateau.end(); ++it)
    {
        removeItem((*it).second->getEllipse().get());
    }
    //groupes.clear();
    plateau.clear();
}

vector<boost::shared_ptr<Pierre> > Goban::ajouterPierre(boost::shared_ptr<Pierre> p)
{
    //if (coupCourant!=0) removeItem(coupCourant);

    SGF::Debug* dbg = SGF::Debug::getInstance();
    //os << "\n ---------- \n Coup n°" << p->getCoup().getNum() << std::endl;

    /* On ajoute la nouvelle pierre au map de pierres référencées par leurs coordonnées */
    const int ord = p->getCoup().getOrd();
    const int abs = p->getCoup().getAbs();
    pair<int,int> coord = make_pair(abs,ord);
    plateau.insert(pair<pair<int,int>,boost::shared_ptr<Pierre> >(coord,p));

    ostringstream msg;
    msg << "!!!!!! Ajout de la pierre " << p->couleur() << " en " << abs << "-" << ord << "\n";
    dbg->add(SGF::Normal,msg.str()); /// INFOS CORRECTES


    ostringstream os;
    os << "Nombre de libertés de la pierre ajoutée : " << p->libertes();
    dbg->add(SGF::Normal,os.str());

    /**************************************** FUSION DES GROUPES ***********************************/

    vector<boost::shared_ptr<Pierre> > pierresAutour = p->pierresAutourMemeCouleur();
    vector<boost::shared_ptr<Groupe> > groupesAutour;

    for (vector<boost::shared_ptr<Pierre> >::iterator it = pierresAutour.begin(); it != pierresAutour.end(); it++)
    {
        boost::shared_ptr<Groupe> m_groupe = (*it)->getGroupe();
        if (find(groupesAutour.begin(), groupesAutour.end(), m_groupe)==groupesAutour.end())
        {
            groupesAutour.push_back(m_groupe);
        }
    }

    if (groupesAutour.size()==0)
    {
        //aucun groupe autour, il faut en créer un nouveau
        boost::shared_ptr<Groupe> groupePtr(new Groupe());
        groupePtr->ajouterPierre(p);
        p->setGroupe(groupePtr->shared_from_this());
        m_groupes.insert(groupePtr);
    }
    else
    {
        /*il y a plusieurs groupes à fusionner : on prend le premier, on lui ajoute les autres
          un par un, puis on ajoute la nouvelle pierre créée */
        boost::shared_ptr<Groupe> groupePtr = groupesAutour[0];
        for (vector<boost::shared_ptr<Groupe> >::iterator it = groupesAutour.begin(); it != groupesAutour.end(); it++)
        {
            if (it!=groupesAutour.begin())
            {
                groupePtr->ajouterGroupe(*it);
                m_groupes.erase(*it);
            }
        }

        groupePtr->ajouterPierre(p);
        p->setGroupe(groupePtr->shared_from_this());
    }

    /****************************************** Affichage ******************************************/
    addItem(p->getEllipse().get());
    if (coupCourant.get()==0)
    {
        QRect rect((abs+1)*FP::ECART_T()-(FP::ECART_T()*0.31),(ord+1)*FP::ECART_T()-(FP::ECART_T()*0.31),FP::ECART_T()*0.6,FP::ECART_T()*0.6);
        coupCourant=  boost::shared_ptr<QGraphicsEllipseItem>(this->addEllipse(rect,rouge));

    }


    /************************* Suppression des pierres *********************************************/
    //unsigned int nbcapt = 0;
    vector<boost::shared_ptr<Pierre> > pierresSupprimees;
    set<boost::shared_ptr<Groupe> > sansLibertes = groupesSansLiberte();
    if (sansLibertes.size()!=0)
    {
        std::ostringstream os;
        os << sansLibertes.size() << " groupes sans libertés sur le plateau";
        dbg->add(SGF::Normal,os.str());
        for (set<boost::shared_ptr<Groupe> >::iterator it = sansLibertes.begin() ; it != sansLibertes.end() ; ++it)
        {
            boost::shared_ptr<Groupe> groupePtr = *it;
            //si groupe adversaire, alors on supprime toutes ses pierres une par une, puis on supprime le groupe
            if (groupePtr->couleur()!=p->couleur())
            {
                vector<boost::shared_ptr<Pierre> > pierres = groupePtr->getPierres();
                for (vector<boost::shared_ptr<Pierre> >::iterator it_pierre = pierres.begin(); it_pierre != pierres.end(); it_pierre++)
                {
                    //pour chaque pierre du groupe à supprimer
                    pierresSupprimees.push_back(*it_pierre);
                    int abs, ord;
                    abs = (*it_pierre)->getCoup().getAbs(); ord = (*it_pierre)->getCoup().getOrd();
                    supprimerPierre(*it_pierre);

                }
            }

            m_groupes.erase(groupePtr);

        }
    }


    //return nbcapt;
    //logMsg = QString::fromStdString(os.str());
    //SGF::Debug::getInstance()->add(SGF::Normal,os.str());
    return pierresSupprimees;

}





void Goban::supprimerPierre(boost::shared_ptr<Pierre> p)
{
    int a = p->getCoup().getAbs();
    int o = p->getCoup().getOrd();
    if (plateau.erase(pair<int,int>(a,o))!=1) throw coup_exception("Erreur à la suppression d'une pierre");

    removeItem(p->getEllipse().get());
    //delete p;
}



bool Goban::estSurPlateau(boost::shared_ptr<Pierre> p) const
{
    int abs = p->getCoup().getAbs(), ord = p->getCoup().getOrd();
    return (plateau.find(pair<int,int>(abs,ord))!=plateau.end());
}

vector<boost::shared_ptr<Pierre> > Goban::pierresSansLibertes() const
{
    vector<boost::shared_ptr<Pierre> > result;

    for (map<pair<int,int>,boost::shared_ptr<Pierre> >::const_iterator it = plateau.begin(); it !=plateau.end(); ++it)
    {
        if (it->second->libertes()==0)
        {
            result.push_back(it->second);
        }
    }
    return result;
}

set<boost::shared_ptr<Groupe> > Goban::groupesSansLiberte() const
{
    set<boost::shared_ptr<Groupe> > result;
    for (set<boost::shared_ptr<Groupe> >::iterator it = m_groupes.begin(); it != m_groupes.end(); it++)
    {
        if ((*it)->nbLibertes()==0)
        {
            result.insert(*it);
        }
    }

    return result;
}

std::string Goban::printPlateau() const
{
    std::ostringstream result;
    result << "Etat du plateau : \n";
    for (map<pair<int,int>,boost::shared_ptr<Pierre> >::const_iterator it = plateau.begin(); it != plateau.end(); ++it)
    {
       // result << "Pierre " << it->second->getCoup().getJoueur()->couleur().toStdString() << " en " << it->first.first << "-" << it->first.second << "\n";
        result << "Pierre " << it->second->getCoup().print() << " m_name=" << it->second->getName() <<"\n";

    }

    result << "\n\n";
    return result.str();
}
