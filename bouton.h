#ifndef BOUTON_H
#define BOUTON_H
#include "FP.h"

class BoutonGoban : public QPushButton
{
    Q_OBJECT


    int abs;
    int ord;

public :
    BoutonGoban(int a, int o);
    int getAbs() const {return abs;}
    int getOrd() const {return ord;}

signals :
    void clickedBouton(int a, int o);

public slots :
    void envoyerSignalClicked();

};

#endif // BOUTON_H
