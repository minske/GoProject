#include "actionNext.h"

actionNext::actionNext(FP* f) : QUndoCommand(), fp(f)
{}

void actionNext::undo()
{
    Goban* g = fp->getGoban();
    //partie* Partie = fp->getPartie();

    /* Il faut supprimer la pierre m_pierre qui a �t� ajout�e,
    et remettre sur le plateau les pierres qui ont �t� supprim�es*/
    g->supprimerPierre(m_pierre);

    if (pierresSupprimees.size()!=0)
    {
        for (set<Pierre*>::iterator it = pierresSupprimees.begin(); it!=pierresSupprimees.end(); ++it)
        {
            g->ajouterPierre(*it);
        }
    }
    g->reculer();
    int abs = (g->getCourant().getPtr()-1)->getAbs();
    int ord = (g->getCourant().getPtr()-1)->getOrd();


    ostringstream os;
    os << (g->getCourant().getPtr()-1)->getNum();
    fp->getComm()->setText("Coup num�ro "+QString::fromStdString(os.str())+"\n "+(g->getCourant().getPtr()-1)->getComm());

    g->removeItem(g->getCoupCourant());
    QRect rect((abs+1)*E-(E*0.31),(ord+1)*E-(E*0.31),E*0.6,E*0.6);
    g->setCoupCourant(g->addEllipse(rect,Goban::getRouge()));
    //g->setCoupCourant((abs+1)*E-(E*0.31),(ord+1)*E-(E*0.31));


}

void actionNext::redo()
{
    partie* Partie = fp->getPartie();
    Goban* goban = fp->getGoban();
    ostringstream os;
    if (Partie!=0) //s'il y a bien une partie ouverte
    {
        if(goban->getCourant()!=Partie->fin()) //et que la partie n'est pas termin�e
        {
            Pierre* p = new Pierre(goban->getCourant().getPtr()); //on r�cup�re l'it�rateur courant
            m_pierre = p;


            os << p->getCoup()->getNum();
            fp->getComm()->setText("Coup num�ro "+QString::fromStdString(os.str())+"\n "+p->getCoup()->getComm());

            int abs = p->getCoup()->getAbs(); int ord = p->getCoup()->getOrd();
            pierresSupprimees = goban->ajouterPierre(p);
            //on ajoute la pierre au goban, ce qui renvoie les pierres captur�es
            unsigned int nbCapt = pierresSupprimees.size();
            if (nbCapt>0) //si ce nombre est positif, il faut les ajouter aux joueurs
            {
                ostringstream os;
                os <<  nbCapt << " pierre(s) � supprimer.";
                //logMsg.append(QString::fromStdString(os.str()));
                setText(QString::fromStdString(os.str()));
                //std::cout << nbCapt << " pierre(s) � supprimer." << std::endl;
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

            goban->removeItem(goban->getCoupCourant());
            QRect rect((abs+1)*E-(E*0.31),(ord+1)*E-(E*0.31),E*0.6,E*0.6);
            goban->setCoupCourant(goban->addEllipse(rect,Goban::getRouge()));
            goban->printGroupes();


        }
        if (goban->getCourant()==Partie->fin()) fp->getComm()->setText("Fin de la partie. R�sultat : " + Partie->getResultat());
    }

    os << "\n" << goban->getLogMsg().toStdString();
    setText(QString::fromStdString(os.str()));

}

bool actionNext::mergeWith(const QUndoCommand* other)
{
    return false;
}

