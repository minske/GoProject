#ifndef FENETREINFOS_H
#define FENETREINFOS_H
#include "FP.h"

class FP;

class FenetreInfos : public QWidget
{
    Q_OBJECT

    QLineEdit* nomNoir;
    QLineEdit* nomBlanc;
    QLineEdit* niveauNoir;
    QLineEdit* niveauBlanc;
    QLineEdit* datePartie;
    FP* fenPrincipale;

public :
    FenetreInfos(FP* f);
    QLineEdit* getNomNoir() const {return nomNoir;}
    QLineEdit* getNomBlanc() const {return nomBlanc;}
    QLineEdit* getNiveauNoir() const {return niveauNoir;}
    QLineEdit* getNiveauBlanc() const {return niveauBlanc;}
    QLineEdit* getDatePartie() const {return datePartie;}
    ~FenetreInfos();


public slots :
    void valider();
    void annuler();

};

#endif // FENETREINFOS_H
