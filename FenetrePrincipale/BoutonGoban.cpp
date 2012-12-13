#include "BoutonGoban.h"

BoutonGoban::BoutonGoban(int a, int o, boost::shared_ptr<Goban> gobanPtr)
    : QPushButton(), abs(a), ord(o), m_goban(boost::weak_ptr<Goban>(gobanPtr))
{
    setFixedSize(gobanPtr->ECART(),gobanPtr->ECART()); setFlat(true);
}

void BoutonGoban::envoyerSignalClicked()
{
    emit clickedBouton(abs,ord);
}
