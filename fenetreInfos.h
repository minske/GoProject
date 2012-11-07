#ifndef FENETREINFOS_H
#define FENETREINFOS_H
#include "FP.h"
#include "boost/enable_shared_from_this.hpp"

class FP;

class FenetreInfos : public QWidget, public boost::enable_shared_from_this<FenetreInfos>
{
    Q_OBJECT

    boost::shared_ptr<QLineEdit> nomNoir;
    boost::shared_ptr<QLineEdit> nomBlanc;
    boost::shared_ptr<QLineEdit> niveauNoir;
    boost::shared_ptr<QLineEdit> niveauBlanc;
    boost::shared_ptr<QLineEdit> datePartie;
    boost::shared_ptr<FP> fenPrincipale;

public :
    FenetreInfos(boost::shared_ptr<FP> f);
    boost::shared_ptr<FenetreInfos> f()
   {
       return shared_from_this();
   }

    boost::shared_ptr<QLineEdit> getNomNoir() const {return nomNoir;}
    boost::shared_ptr<QLineEdit> getNomBlanc() const {return nomBlanc;}
    boost::shared_ptr<QLineEdit> getNiveauNoir() const {return niveauNoir;}
    boost::shared_ptr<QLineEdit> getNiveauBlanc() const {return niveauBlanc;}
    boost::shared_ptr<QLineEdit> getDatePartie() const {return datePartie;}
    ~FenetreInfos();


public slots :
    void valider();
    void annuler();

};

#endif // FENETREINFOS_H
