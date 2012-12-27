#include "FenetrePrincipale.h"
#include "fenetreInfos.h"
#include "../Partie/partie.h"
#include "actionNext.h"
#include "../Tools/debug.h"

using namespace std;


FenetrePrincipale::FenetrePrincipale() : QMainWindow()
{
    menuFichier = menuBar()->addMenu("&Fichier");
    menuOptions = menuBar()->addMenu("&Options");

    QAction* actionQuitter =menuFichier->addAction("Quitter");
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));

    changerFond = menuOptions->addMenu("Choisir le fond du goban");
    QAction* changerFondClair = changerFond->addAction("Fond clair");
    QAction* changerFondMoyen = changerFond->addAction("Fond moyen");
    QAction* changerFondFonce = changerFond->addAction("Fond foncé");
    QAction* changerFondSansMotif = changerFond->addAction("Aucun motif");
    connect(changerFondClair,SIGNAL(triggered()),this,SLOT(changerFondClair()));
    connect(changerFondMoyen,SIGNAL(triggered()),this,SLOT(changerFondMoyen()));
    connect(changerFondFonce,SIGNAL(triggered()),this,SLOT(changerFondFonce()));
    connect(changerFondSansMotif,SIGNAL(triggered()),this,SLOT(changerFondSansMotif()));

    //Layout horizontal principal : à gauche le goban, à droite les infos
    layoutPrincipal = new QHBoxLayout();
    //Layout vertical pour le goban de la fenêtre :
    layoutV = new QVBoxLayout();



    //Définition du widget pour affichage des infos
    infosJoueur = new QHBoxLayout();

    //construction du widget pour les infos de noir et blanc
    infosNoir =new infosJoueurs();
    infosBlanc = new infosJoueurs();
    infosNoir->setTitre("<b>Noir</b>"); infosBlanc->setTitre("<b>Blanc</b>");

    infosJoueur->addLayout(infosNoir);
    infosJoueur->addLayout(infosBlanc);

    widgetsCote = new QVBoxLayout();
    widgetsCote->addLayout(infosJoueur);

    layoutPrincipal->addLayout(layoutV);
    layoutPrincipal->addLayout(widgetsCote);
    layoutPrincipal->addSpacing(200);

    QWidget* m = new QWidget();
    m->setLayout(layoutPrincipal);

    setCentralWidget(m);



}




FenetrePrincipale::~FenetrePrincipale()
{
}



void FenetrePrincipale::changerFondClair()
{
    m_goban->setBackgroundBrush(m_goban->getBrushClair());
}

void FenetrePrincipale::changerFondFonce()
{
    m_goban->setBackgroundBrush(m_goban->getBrushFonce());
}

void FenetrePrincipale::changerFondMoyen()
{
    m_goban->setBackgroundBrush(m_goban->getBrushMoyen());
}

void FenetrePrincipale::changerFondSansMotif()
{
   m_goban->setBackgroundBrush(m_goban->getBrushSansMotif());
}






void FenetrePrincipale::afficherMsgExec()
{
    messagesExecution->show();
}
