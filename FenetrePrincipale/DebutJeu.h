#ifndef DEBUTJEU_H
#define DEBUTJEU_H

#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QButtonGroup>

class Lanceur;

class DebutJeu : public QWidget
{
    Q_OBJECT

public :
    DebutJeu(Lanceur* l);

public slots :
    void valider();
    void choixNoir();
    void choixBlanc();

private :
    Lanceur* m_lanceur;
    QSpinBox* handicap;
    QDoubleSpinBox* komi;
    QButtonGroup* couleur;
};


#endif // DEBUTJEU_H
