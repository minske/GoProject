#ifndef ACTIONNEXT_H
#define ACTIONNEXT_H

#include "FP.h"


class actionNext : public QUndoCommand
{
public :
    actionNext(FP* f);
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
    Pierre* m_pierre;
    bool Suppression;
    vector<Groupe*> groupesSupprimes;
    unsigned int AnbPierresCaptNoir;
    unsigned int NnbPierresCaptNoir;
    unsigned int AnbPierresCaptBlanc;
    unsigned int NnbPierresCaptBlanc;
    FP* fp;

};

#endif // ACTIONNEXT_H
