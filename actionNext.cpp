#include "actionNext.h"

actionNext::actionNext(FP* f) : QUndoCommand(), fp(f)
{}

void actionNext::undo()
{
    Goban* g = fp->getGoban();
    partie* Partie = fp->getPartie();
    /* On remplace le plateau par le nouveau plateau */
    g->setPlateau(Aplateau);
    g->removeItem(m_pierre->getEllipse());

    /* On trouve le groupe auquel appartient la pile qu'on vient de poser et on enlève la pierre du groupe
    Groupe* grpe = g->trouverGroupe(p);
    grpe->getPierres().erase(p);*/

    /* On rétablit les anciens groupes */
    set<Groupe*> ancienGroupes;
    for (vector<Groupe>::iterator it=Agroupes.begin() ; it!=Agroupes.end(); ++it)
        ancienGroupes.insert(&(*it));
    g->setGroupes(ancienGroupes);

    //g->supprimerPierre(p);
    g->reculer();
    Partie->getNoir()->setCapt(NnbPierresCaptNoir); Partie->getBlanc()->setCapt(NnbPierresCaptBlanc);
}

void actionNext::redo()
{
    partie* Partie = fp->getPartie();
    Goban* goban = fp->getGoban();
    bool suppr = false;
    set<Groupe*> OldGroupes = goban->getGroupes();
    map<pair<int,int>,Pierre*> OldPlateau = goban->getPlateau();
    unsigned int OldCaptNoir = Partie->getNoir()->getCapt();
    unsigned int OldCaptBlanc = Partie->getBlanc()->getCapt();
    vector<Groupe*> GrpSuppr;

    if (Partie!=0)
    {
        if(goban->getCourant()!=Partie->fin())
        {
            Pierre* p = new Pierre(goban->getCourant().getPtr());
            unsigned int nbCapt = goban->ajouterPierre(p);
            if (nbCapt>0)
            {
                suppr=true; std::cout << nbCapt << " pierre(s) à supprimer." << std::endl;
                if (p->getCoup()->getJoueur()->couleur()=="Blanc")
                {
                    ostringstream os;
                    os << Partie->getBlanc()->getCapt() + nbCapt;
                    Partie->getBlanc()->addCapt(nbCapt);
                    fp->getInfosBlanc()->setCapt(QString::fromStdString(os.str()));

                }
                else
                {
                    ostringstream os;
                    os << Partie->getNoir()->getCapt() + nbCapt;
                    Partie->getNoir()->addCapt(nbCapt);
                    fp->getInfosNoir()->setCapt(QString::fromStdString(os.str()));

                }
            }

            ostringstream os;
            os << goban->getCourant().getPtr()->getNum();
            fp->getComm()->setText("Coup numéro "+QString::fromStdString(os.str())+"\n "+goban->getCourant().getPtr()->getComm());

            goban->avancer();

           for (set<Groupe*>::const_iterator it = OldGroupes.begin(); it!=OldGroupes.end(); ++it)
           {
               const Groupe gpe = **it;
               Agroupes.push_back(gpe);
           }

            for (set<Groupe*>::const_iterator it=goban->getGroupes().begin(); it!=goban->getGroupes().end(); ++it)
            {
                const Groupe gpe = **it;
                Ngroupes.push_back(gpe);
            }

            Aplateau = OldPlateau; Nplateau = goban->getPlateau();
            m_pierre = p; Suppression = suppr; groupesSupprimes = GrpSuppr;
            AnbPierresCaptNoir = OldCaptNoir; AnbPierresCaptBlanc = OldCaptBlanc;
            NnbPierresCaptNoir = Partie->getNoir()->getCapt();
            NnbPierresCaptBlanc = Partie->getBlanc()->getCapt();

            //return new actionNext(OldGroupes, goban->getGroupes(),OldPlateau, goban->getPlateau(),goban,p,Partie,suppr,GrpSuppr,OldCaptNoir,Partie->getNoir()->getCapt(),OldCaptBlanc,Partie->getBlanc()->getCapt());

        }
        else fp->getComm()->setText("Fin de la partie. Résultat : " + Partie->getResultat());
    }
}

bool actionNext::mergeWith(const QUndoCommand* other)
{
    return false;
}

