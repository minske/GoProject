#include "bouton.h"

void BoutonGoban::envoyerSignalClicked()
{
    emit clickedBouton(abs,ord);
}
