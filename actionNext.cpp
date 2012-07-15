#include "actionNext.h"

void actionNext::undo()
{
    g->setPlateau(Nplateau); //g->setGroupes(Ngroupes);
    Groupe* grpe = g->trouverGroupe(p);
    grpe->getPierres().erase(p);

    set<Groupe*> ancienGroupes;
    for (vector<Groupe>::iterator it=Ngroupes.begin() ; it!=Ngroupes.end(); ++it)
        ancienGroupes.insert(&(*it));
    g->setGroupes(ancienGroupes);

    g->supprimerPierre(p);
    g->reculer();
    Partie->getNoir()->setCapt(NnbPierresCaptNoir); Partie->getBlanc()->setCapt(NnbPierresCaptBlanc);
}

void actionNext::redo()
{
    g->setPlateau(Aplateau);// g->setGroupes(Agroupes);
    //g->ajouterPierre(p);
    g->addItem(p->getEllipse());
}

bool actionNext::mergeWith(const QUndoCommand* other)
{
    return false;
}

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
