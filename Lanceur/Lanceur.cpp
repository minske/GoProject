#include "Lanceur.h"

#include <QHBoxLayout>
#include <QVBoxLayout>


Lanceur::Lanceur() : QFrame()
{
    QVBoxLayout* vlayout = new QVBoxLayout;

    QPalette palette(QColor(201, 201, 201, 100));

    boutonSGF = new QPushButton("Editer ou créer un SGF");
    boutonSGF->setFixedSize(250,60);
    boutonSGF->setPalette(palette);
    boutonJeu = new QPushButton("Jouer contre l'ordinateur");
    boutonJeu->setFixedSize(250,60);
    boutonQuitter = new QPushButton("Quitter");
    boutonQuitter->setFixedSize(250,60);

    vlayout->addSpacing(200);
    vlayout->addWidget(boutonSGF);
    vlayout->addWidget(boutonJeu);
    vlayout->addWidget(boutonQuitter);
    vlayout->setAlignment(boutonSGF,Qt::AlignCenter);
    vlayout->setAlignment(boutonJeu,Qt::AlignCenter);
    vlayout->setAlignment(boutonQuitter,Qt::AlignCenter);

    //        connect(next,SIGNAL(clicked()),this,SLOT(nextMove()));

    connect(boutonSGF,SIGNAL(clicked()),this,SLOT(LancerSGF()));
    connect(boutonJeu,SIGNAL(clicked()),this,SLOT(LancerJeu()));
    connect(boutonQuitter,SIGNAL(clicked()),this,SLOT(Quitter()));

    QWidget::setFixedSize(350,400);
    this->setObjectName("cocorico");
    this->setLayout(vlayout);
}


void Lanceur::LancerSGF()
{
    if (m_fenetreSGF.get()==0)
    {
        m_fenetreSGF.reset(new FenetreSGF());
        m_fenetreSGF->show();
    }
}

void Lanceur::LancerJeu()
{
}

void Lanceur::Quitter()
{
    if (m_fenetreJeu.get()!=0) m_fenetreJeu->close();
    if (m_fenetreSGF.get()!=0) m_fenetreSGF->close();
    this->close();
}
