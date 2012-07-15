#ifndef ACTIONNEXT_H
#define ACTIONNEXT_H

#include "FP.h"

class actionNext : public QUndoCommand
{
public :
    actionNext(set<Groupe*> Agrp, set<Groupe*> Ngrp,
               map<pair<int,int>,Pierre*> Aplt, map<pair<int,int>,Pierre*> Nplt,
               Goban* gb, Pierre* pr, partie* part,
               bool suppr, vector<Groupe*> grpSuppr,
               unsigned int ApNoir, unsigned int NpNoir,
               unsigned int ApBlanc, unsigned int NpBlanc);
    ~actionNext() {}
    int id() const {return 42;}
    bool mergeWith(const QUndoCommand* other);
    void redo();
    void undo();

private :
    //A pour Ancien, N pour Nouveau
    vector<Groupe> Agroupes;
    vector<Groupe> Ngroupes;
    map<pair<int,int>,Pierre*> Aplateau;
    map<pair<int,int>,Pierre*> Nplateau;
    Goban* g;
    Pierre* p;
    partie* Partie;
    bool Suppression;
    vector<Groupe*> groupesSupprimes;
    unsigned int AnbPierresCaptNoir;
    unsigned int NnbPierresCaptNoir;
    unsigned int AnbPierresCaptBlanc;
    unsigned int NnbPierresCaptBlanc;


};

#endif // ACTIONNEXT_H
