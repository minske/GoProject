#include "actionNext.h"
#include "debug.h"

actionNext::actionNext() : QUndoCommand()
{
    SGF::Debug::getInstance()->add(SGF::Normal,"actionNext::constructeur\n");
}

actionNext::actionNext(boost::shared_ptr<FP> f) : QUndoCommand(), fp(f)
{
    SGF::Debug::getInstance()->add(SGF::Normal,"actionNext::constructeur\n");
}

void actionNext::undo()
{
    Goban* g = Goban::getInstance();
    SGF::Debug* dbg = SGF::Debug::getInstance();
    //dbg->add(SGF::Normal,"\n == Undo ==");
    *dbg << "\n == Undo ==";

    //partie* Partie = fp->getPartie();

    /* Il faut supprimer la pierre m_pierre qui a été ajoutée,
    et remettre sur le plateau les pierres qui ont été supprimées*/
    try
    {
        g->supprimerPierre(m_pierre);
        dbg->add(SGF::Normal,"Suppression de la pierre : "+m_pierre->getCoup()->print());

        if (pierresSupprimees.size()!=0)
        {
            for (vector<boost::shared_ptr<Pierre> >::iterator it = pierresSupprimees.begin(); it!=pierresSupprimees.end(); ++it)
            {
                dbg->add(SGF::Normal,"Rajout de la pierre : "+(*it)->getCoup()->print());
                g->ajouterPierre(*it);
            }
        }
        g->reculer();
        int abs = g->getCourant()->getAbs();
        int ord = g->getCourant()->getOrd();


        ostringstream os;
        os << g->getCourant()->getNum();
        fp->getComm()->setText("Coup numéro "+QString::fromStdString(os.str())+"\n "+g->getCourant()->getComm());

        g->removeItem(&(g->getCoupCourant().operator *()));
        QRect rect((abs+1)*FP::ECART_T()-(FP::ECART_T()*0.31),(ord+1)*FP::ECART_T()-(FP::ECART_T()*0.31),FP::ECART_T()*0.6,FP::ECART_T()*0.6);
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

void actionNext::redo()
{
    SGF::Debug* dbg = SGF::Debug::getInstance();

    try {

        dbg->add(SGF::Normal, "\n-- (Re)do --");
        boost::shared_ptr<partie> Partie = fp->getPartie();
        Goban* goban = Goban::getInstance();
        ostringstream os;
        if (Partie!=0) //s'il y a bien une partie ouverte
        {
            if(goban->getCourant()!=Partie->fin()) //et que la partie n'est pas terminée
            {
                Coup c = goban->getCourant().operator *();
                dbg->add(SGF::Normal, c.print());
                boost::shared_ptr<Pierre> p (new Pierre(boost::shared_ptr<Coup>(&c))); //on récupère l'itérateur courant
                m_pierre = p;

                std::ostringstream pierreCreee;
                pierreCreee << "Création de la pierre : " << p->getCoup()->getJoueur()->couleur().toStdString()
                            << " en " << p->getCoup()->getAbs()
                            << "-" << p->getCoup()->getOrd() << "\n";
                SGF::Debug::getInstance()->add(SGF::Normal,pierreCreee.str());
                os << p->getCoup()->getNum();
                fp->getComm()->setText("Coup numéro "+QString::fromStdString(os.str())+"\n "+p->getCoup()->getComm());

                int abs = p->getCoup()->getAbs(); int ord = p->getCoup()->getOrd();
                pierresSupprimees = goban->ajouterPierre(p);
                //on ajoute la pierre au goban, ce qui renvoie les pierres capturées
                unsigned int nbCapt = pierresSupprimees.size();
                if (nbCapt>0) //si ce nombre est positif, il faut les ajouter aux joueurs
                {
                    ostringstream os;
                    os <<  nbCapt << " pierre(s) à supprimer.";
                    setText(QString::fromStdString(os.str()));

                    if (p->getCoup()->getJoueur()->couleur()=="Blanc")
                    {
                        ostringstream oss;
                        oss << Partie->getBlanc()->getCapt() + nbCapt;
                        Partie->getBlanc()->addCapt(nbCapt);
                        fp->getInfosBlanc()->setCapt(QString::fromStdString(oss.str()));

                    }
                    else
                    {
                        ostringstream oss;
                        oss << Partie->getNoir()->getCapt() + nbCapt;
                        Partie->getNoir()->addCapt(nbCapt);
                        fp->getInfosNoir()->setCapt(QString::fromStdString(oss.str()));

                    }
                }


                goban->avancer();

                goban->removeItem(goban->getCoupCourant().get());
                QRect rect((abs+1)*FP::ECART_T()-(FP::ECART_T()*0.31),(ord+1)*FP::ECART_T()-(FP::ECART_T()*0.31),FP::ECART_T()*0.6,FP::ECART_T()*0.6);
                goban->setCoupCourant(boost::shared_ptr<QGraphicsEllipseItem>(goban->addEllipse(rect,Goban::getRouge())));
                //goban->printGroupes();


            }
            if (goban->getCourant()==Partie->fin()) fp->getComm()->setText("Fin de la partie. Résultat : " + Partie->getResultat());
        }

        //os << "\n" << goban->getLogMsg().toStdString();
        setText(QString::fromStdString(os.str()));
        dbg->add(SGF::Normal,Goban::getInstance()->printPlateau());

        dbg->add(SGF::Normal,"--Fin Redo\n");
    }
    catch (std::exception const& e)
    {
        std::ostringstream os;
        os << "Fichier " << __FILE__ << " ligne " << __LINE__ << " : " << e.what();
        dbg->add(SGF::Exception,os.str());
    }
}

bool actionNext::mergeWith(const boost::shared_ptr<QUndoCommand> other)
{
    return false;
}

