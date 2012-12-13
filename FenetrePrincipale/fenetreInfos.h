#ifndef FENETREINFOS_H
#define FENETREINFOS_H
#include "FenetrePrincipale.h"
#include "boost/enable_shared_from_this.hpp"


class FenetreInfos : public QWidget, public boost::enable_shared_from_this<FenetreInfos>
{
    Q_OBJECT

    QLineEdit* nomNoir;
    QLineEdit* nomBlanc;
    QLineEdit* niveauNoir;
    QLineEdit* niveauBlanc;
    QLineEdit* datePartie;
    FenetrePrincipale* fenPrincipale;

public :
    FenetreInfos(FenetrePrincipale* f);

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
