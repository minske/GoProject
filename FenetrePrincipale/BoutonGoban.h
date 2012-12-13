#ifndef BOUTON_H
#define BOUTON_H
#include "FenetrePrincipale.h"

class BoutonGoban : public QPushButton
{
    Q_OBJECT


    int abs;
    int ord;
    boost::weak_ptr<Goban> m_goban;

public :
    BoutonGoban(int a, int o, boost::shared_ptr<Goban> gobanPtr);

    int getAbs() const {return abs;}
    int getOrd() const {return ord;}
    boost::shared_ptr<Goban> getGoban() {return m_goban.lock();}

signals :
    void clickedBouton(int a, int o);

public slots :
    void envoyerSignalClicked();

};

#endif // BOUTON_H
