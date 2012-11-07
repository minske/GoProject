#include "bouton.h"

BoutonGoban::BoutonGoban(int a, int o) : QPushButton(), abs(a), ord(o)
{
    setFixedSize(FP::ECART_T(),FP::ECART_T()); setFlat(true);
}

void BoutonGoban::envoyerSignalClicked()
{
    emit clickedBouton(abs,ord);
}
