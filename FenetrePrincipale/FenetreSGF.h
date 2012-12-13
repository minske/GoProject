#ifndef FENETRESGF_H
#define FENETRESGF_H

#include "FenetrePrincipale.h"

class FenetreSGF : public FenetrePrincipale
{
    Q_OBJECT

public :
    FenetreSGF();
    boost::shared_ptr<FenetreSGF> sharedFromThis() {return boost::dynamic_pointer_cast<FenetreSGF>(shared_from_this());}

public slots :
    void ouvrirFichier();
    void enregistrerFichier();
    void nouveauFichier();
    void nextMove();
    void prevMove();
    void next5Moves();
    void prev5Moves();
    void debutPartie();
    void finPartie();
    void fermerFichier();
};

#endif // FENETRESGF_H
