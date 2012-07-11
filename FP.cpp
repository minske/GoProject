#include "FP.h"
#include "partie.h"

FP::FP() : QMainWindow(), Partie(0)
{
    //menu
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");


    QAction* ouvrirFichier = menuFichier->addAction("Ouvrir");
    ouvrirFichier->setShortcut(QKeySequence("Ctrl+O"));
    connect(ouvrirFichier,SIGNAL(triggered()),this,SLOT(ouvrirFichier()));

    QAction* fermerFichier = menuFichier->addAction("Fermer");
    fermerFichier->setShortcut(QKeySequence("Ctrl+W"));
    connect(fermerFichier,SIGNAL(triggered()),this,SLOT(fermerFichier()));

    QAction* actionQuitter = menuFichier->addAction("Quitter");
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));


    //Layout horizontal principal : à gauche le goban, à droite les infos
    layoutPrincipal = new QHBoxLayout;
    //Layout vertical pour le goban de la fenêtre :
    layoutV = new QVBoxLayout;

    //boutons ouvrir,fermer,quitter
    QPushButton* ouvrir = new QPushButton("Ouvrir");
    QPushButton* fermer = new QPushButton("Fermer");
    QPushButton* quitter = new QPushButton("Quitter");
    connect(ouvrir,SIGNAL(clicked()),this,SLOT(ouvrirFichier()));
    connect(fermer,SIGNAL(clicked()),this,SLOT(fermerFichier()));
    connect(quitter,SIGNAL(clicked()),qApp,SLOT(quit()));
    QHBoxLayout* layoutBoutonsOFQ = new QHBoxLayout;
    layoutBoutonsOFQ->addWidget(ouvrir);
    layoutBoutonsOFQ->addWidget(fermer);
    layoutBoutonsOFQ->addWidget(quitter);

    //boutons avancer et reculer
    QPushButton* prev = new QPushButton("<");
    QPushButton* next = new QPushButton(">");
    next->setShortcut(QKeySequence::MoveToNextChar);
    connect(next,SIGNAL(clicked()),this,SLOT(nextMove()));
    QHBoxLayout* layoutBoutonsNP = new QHBoxLayout;
    layoutBoutonsNP->addWidget(prev);
    layoutBoutonsNP->addWidget(next);

    layoutBoutons = new QVBoxLayout;
    layoutBoutons->addLayout(layoutBoutonsOFQ);
    layoutBoutons->addLayout(layoutBoutonsNP);


    //on ajoute les boutons au layout goban
    layoutV->addLayout(layoutBoutons);

    //définition du goban
    goban = new Goban();
    vue = new QGraphicsView(goban);
    vue->setFixedSize(E*20,E*20);
    layoutV->addWidget(vue);


    //Définition du widget pour affichage des infos
    infosJoueurs = new QWidget;
    infosJoueurs->setFixedWidth(300);
    widgetsCote = new QVBoxLayout;
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
    if (goban!=0) delete goban;
    if (Partie!=0) delete Partie;
    if (infosJoueurs!=0) delete infosJoueurs;
}

void FP::fermerFichier()
{
    delete Partie;
    Partie=0;
    goban->init(); infosJoueurs = new QWidget;
    infosJoueurs->setFixedWidth(300);
}
