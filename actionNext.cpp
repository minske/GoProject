#include "actionNext.h"

actionNext::actionNext(set<Groupe*> Agrp, set<Groupe*> Ngrp,
           map<pair<int,int>,Pierre*> Aplt, map<pair<int,int>,Pierre*> Nplt,
           Goban* gb, Pierre* pr, partie* part,
           bool suppr, vector<Groupe*> grpSuppr,
           unsigned int ApNoir, unsigned int NpNoir,
           unsigned int ApBlanc, unsigned int NpBlanc)
    : QUndoCommand(), Aplateau(Aplt),Nplateau(Nplt),
      g(gb), p(pr),  Partie(part), Suppression(suppr), groupesSupprimes(grpSuppr),
      AnbPierresCaptNoir(ApNoir), NnbPierresCaptNoir(NpNoir),
      AnbPierresCaptBlanc(ApBlanc), NnbPierresCaptBlanc(NpBlanc), Agroupes(vector<Groupe>()), Ngroupes(vector<Groupe>())
{
    //il faut copier les groupes
    for (set<Groupe*>::iterator it = Agrp.begin() ; it!=Agrp.end(); ++it)
        Agroupes.push_back(*(*it));
    for(set<Groupe*>::iterator it = Ngrp.begin(); it!= Ngrp.end(); ++it)
        Ngroupes.push_back(*(*it));
    setText("Next");
}

void actionNext::undo()
{
    /* On remplace le plateau par le nouveau plateau */
    g->setPlateau(Aplateau);
    g->removeItem(p->getEllipse());

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
    std::cout << "redo" << std::endl;
   /* g->setPlateau(Aplateau);// g->setGroupes(Agroupes);
    //g->ajouterPierre(p);
    g->addItem(p->getEllipse());*/
}

bool actionNext::mergeWith(const QUndoCommand* other)
{
    return false;
}

