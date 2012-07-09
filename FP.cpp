#include "FP.h"
#include "partie.h"

FP::FP() : QMainWindow(), Partie(0)
{
    //menu
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");


    QAction* ouvrirFichier = menuFichier->addAction("Ouvrir");
    ouvrirFichier->setShortcut(QKeySequence("Ctrl+O"));
    connect(ouvrirFichier,SIGNAL(triggered()),this,SLOT(ouvrirFichier()));

    QAction* actionQuitter = menuFichier->addAction("Quitter");
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));

    //Layout horizontal principal : à gauche le goban, à droite les infos
    QHBoxLayout* layoutPrincipal = new QHBoxLayout;
    //Layout vertical pour le goban de la fenêtre :
    QVBoxLayout* layoutV = new QVBoxLayout;

    //boutons avancer et reculer
    QPushButton* prev = new QPushButton("<");
    QPushButton* next = new QPushButton(">");
    next->setShortcut(QKeySequence::MoveToNextChar);
    connect(next,SIGNAL(clicked()),this,SLOT(nextMove()));
    QHBoxLayout* layoutBoutons = new QHBoxLayout;
    layoutBoutons->addWidget(prev);
    layoutBoutons->addWidget(next);


    //on ajoute les boutons au layout goban
    layoutV->addLayout(layoutBoutons);

    //définition du goban
    goban = new Goban();
    QGraphicsView* vue = new QGraphicsView(goban);
    vue->setFixedSize(E*20,E*20);
    layoutV->addWidget(vue);


    //Définition du widget pour affichage des infos
    infosJoueurs = new QWidget;
    infosJoueurs->setFixedWidth(300);
    QVBoxLayout* widgetsCote = new QVBoxLayout;
    widgetsCote->addWidget(infosJoueurs);
    widgetsCote->addSpacing(500);
    //infosPartie->setFont();

    layoutPrincipal->addLayout(layoutV);
    layoutPrincipal->addLayout(widgetsCote);
    layoutPrincipal->addSpacing(200);

    QWidget* m = new QWidget;
    m->setLayout(layoutPrincipal);

    setCentralWidget(m);
}


void FP::ouvrirFichier()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Fichier SGF (*.sgf)");
    QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);
    Partie = partie::donneInstance();
    Partie->chargerFichier(fichier.toStdString());

    QHBoxLayout* joueurs = new QHBoxLayout;
    joueurs->addWidget(Partie->getNoir()->getInfos());
    joueurs->addWidget(Partie->getBlanc()->getInfos());
    infosJoueurs->setLayout(joueurs);
}


void FP::nextMove()
{
    if(Partie->getCourant()!=Partie->fin())
    {
        Pierre* p = new Pierre(Partie->getCourant().getPtr());
        goban->ajouterPierre(p);
        Partie->avancer();

        //Partie->getBlanc()->addCapt(5);
    }
}

FP::~FP()
{
    delete goban; delete Partie; delete infosJoueurs;
}
