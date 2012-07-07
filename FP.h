#ifndef FORM_H
#define FORM_H

#include <QApplication>
#include <QtGui>
#include "goban.h"

class FP : public QMainWindow
{
    Q_OBJECT
private :
    Goban* goban;
    partie* Partie;
    QWidget* infosJoueurs;

public :
    FP();

public slots :
    void ouvrirFichier();
    void nextMove();
    //void prevMove();
};

#endif // FORM_H
