#include "ActionNext.h"
#include "../Tools/debug.h"
#include "../GobanFiles/GobanManager.h"

using namespace std;

ActionNext::ActionNext() : QUndoCommand()
{
    SGF::Debug::getInstance()->add(SGF::Normal,"ActionNext::constructeur\n");
}

ActionNext::ActionNext(boost::shared_ptr<FenetreSGF> f) : QUndoCommand()
{
    fp = boost::weak_ptr<FenetreSGF>(f);
    SGF::Debug::getInstance()->add(SGF::Normal,"ActionNext::constructeur\n");
}

void ActionNext::undo()
{
    boost::shared_ptr<Goban> g = fp.lock()->getGoban();
    boost::shared_ptr<Partie> p = fp.lock()->getGoban()->getPartie();
    SGF::Debug* dbg = SGF::Debug::getInstance();
    //dbg->add(SGF::Normal,"\n == Undo ==");
    *dbg << "\n == Undo ==";

    //partie* Partie = fp->getPartie();

    /* Il faut supprimer la pierre m_pierre qui a été ajoutée,
    et remettre sur le plateau les pierres qui ont été supprimées*/
    try
    {
        g->supprimerPierre(m_pierre);
        dbg->add(SGF::Normal,"Suppression de la pierre : "+m_pierre->getCoup().print());

        if (pierresSupprimees.size()!=0)
        {
            for (vector<boost::shared_ptr<Pierre> >::iterator it = pierresSupprimees.begin(); it!=pierresSupprimees.end(); ++it)
            {
                dbg->add(SGF::Normal,"Rajout de la pierre : "+(*it)->getCoup().print());
                g->ajouterPierre(*it);
            }
        }
        g->reculer();
        int abs = p->getCoup(g->getCourant()).getAbs();
        int ord = p->getCoup(g->getCourant()).getOrd();


        ostringstream os;
        os << p->getCoup(g->getCourant()).getNum();
        fp.lock()->getComm()->setText("Coup numéro "+QString::fromStdString(os.str())+"\n "+p->getCoup(g->getCourant()).getComm());

        g->removeItem(&(g->getCoupCourant().operator *()));
        QRect rect((abs+1)*g->ECART()-(g->ECART()*0.31),(ord+1)*g->ECART()-(g->ECART()*0.31),g->ECART()*0.6,g->ECART()*0.6);
        g->setCoupCourant(boost::shared_ptr<QGraphicsEllipseItem>(g->addEllipse(rect,Goban::getRouge())));
        //g->setCoupCourant((abs+1)*E-(E*0.31),(ord+1)*E-(E*0.31));
    }
    catch (std::exception const& e)
    {
        std::ostringstream os;
        os << "Fichier " << __FILE__ << " ligne " << __LINE__ << " : " << e.what();
        dbg->add(SGF::Exception,os.str());
    }

    dbg->add(SGF::Normal,"== fin Undo ==\n");
}

void ActionNext::redo()
{
    SGF::Debug* dbg = SGF::Debug::getInstance();

    try
    {
        cout << "REDO\n";
        dbg->add(SGF::Normal, "\n-- (Re)do --");
        //boost::shared_ptr<partie> Partie = fp->getPartie();
        boost::shared_ptr<Goban> goban = fp.lock()->getGoban();
        ostringstream os;
        cout << "nb de coups : " << goban->getPartie()->getListeCoups().size() << endl;
        if(!goban->partieTerminee()) //et que la partie n'est pas terminée
        {
            std::cout << "partie non terminée" << std::endl;
                goban->avancer();
                boost::shared_ptr<Partie> partie = goban->getPartie();
                Coup c = partie->getCoup(goban->getCourant());
                cout << "Coup : "<< c.print() << endl;
                dbg->add(SGF::Normal, c.print());
                int ecartGoban = goban->ECART();
                boost::shared_ptr<Pierre> p (new Pierre(c, ecartGoban)); //on récupère l'itérateur courant
                cout << "Pierre créée\n";
                m_pierre = p;

                std::ostringstream pierreCreee;
                pierreCreee << "Création de la pierre : " << p->getCoup().getJoueur()->couleur()
                            << " en " << p->getCoup().getAbs()
                            << "-" << p->getCoup().getOrd() << "\n";
                SGF::Debug::getInstance()->add(SGF::Normal,pierreCreee.str());
                cout << pierreCreee.str();
                os << p->getCoup().getNum();
                fp.lock()->getComm()->setText("Coup numéro "+QString::fromStdString(os.str())+"\n "+p->getCoup().getComm());

                int abs = p->getCoup().getAbs(); int ord = p->getCoup().getOrd();
                pierresSupprimees = goban->ajouterPierre(p);
                cout << "Ajout au goban : ok" << endl;
                //on ajoute la pierre au goban, ce qui renvoie les pierres capturées
                unsigned int nbCapt = pierresSupprimees.size();
                if (nbCapt>0) //si ce nombre est positif, il faut les ajouter aux joueurs
                {
                    ostringstream os;
                    os <<  nbCapt << " pierre(s) à supprimer.";
                    setText(QString::fromStdString(os.str()));

                    if (p->getCoup().getJoueur()->couleur()=="Blanc")
                    {
                        ostringstream oss;
                        oss << partie->getBlanc()->getCapt() + nbCapt;
                        partie->getBlanc()->addCapt(nbCapt);
                        fp.lock()->getInfosBlanc()->setCapt(QString::fromStdString(oss.str()));

                    }
                    else
                    {
                        ostringstream oss;
                        oss << goban->getPartie()->getNoir()->getCapt() + nbCapt;
                        goban->getPartie()->getNoir()->addCapt(nbCapt);
                        fp.lock()->getInfosNoir()->setCapt(QString::fromStdString(oss.str()));

                    }
                }


                goban->removeItem(goban->getCoupCourant().get());
                QRect rect((abs+1)*goban->ECART()-(goban->ECART()*0.31),(ord+1)*goban->ECART()-(goban->ECART()*0.31),goban->ECART()*0.6,goban->ECART()*0.6);
                goban->setCoupCourant(boost::shared_ptr<QGraphicsEllipseItem>(goban->addEllipse(rect,Goban::getRouge())));
                //goban->printGroupes();


            }
            if (goban->getCourant()==goban->getPartie()->getListeCoups().size()) fp.lock()->getComm()->setText("Fin de la partie. Résultat : " + goban->getPartie()->getResultat());


        //os << "\n" << goban->getLogMsg().toStdString();
        setText(QString::fromStdString(os.str()));
        dbg->add(SGF::Normal,fp.lock()->getGoban()->printPlateau());

        dbg->add(SGF::Normal,"--Fin Redo\n");
    }
    catch (std::exception const& e)
    {
        std::ostringstream os;
        os << "Fichier " << __FILE__ << " ligne " << __LINE__ << " : " << e.what();
        dbg->add(SGF::Exception,os.str());
    }
}

bool ActionNext::mergeWith(const boost::shared_ptr<QUndoCommand> other)
{
    return false;
}

