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
    connect(prev,SIGNAL(clicked()),this,SLOT(prevMove()));
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
    infosJoueur = new QHBoxLayout;

    //construction du widget pour les infos de noir et blanc
    infosNoir = new infosJoueurs;
    infosBlanc = new infosJoueurs;
    infosNoir->setTitre("<b>Noir</b>"); infosBlanc->setTitre("<b>Blanc</b>");

    infosJoueur->addLayout(infosNoir);
    infosJoueur->addLayout(infosBlanc);


    widgetsCote = new QVBoxLayout;
    widgetsCote->addLayout(infosJoueur);
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
    goban->setCourant(partie::iterateur(Partie->debut()));

    infosNoir->setNom(Partie->getNoir()->getNom());
    infosNoir->setNiveau(Partie->getNoir()->getRank());
    infosNoir->setCapt("0");

    infosBlanc->setNom(Partie->getBlanc()->getNom());
    infosBlanc->setNiveau(Partie->getBlanc()->getRank());
    infosBlanc->setCapt("0");

}


void FP::nextMove()
{
    if(goban->getCourant()!=Partie->fin())
    {
        Pierre* p = new Pierre(goban->getCourant().getPtr());
        unsigned int nbCapt = goban->ajouterPierre(p);
        if (nbCapt>0)
        {
            if (p->getCoup()->getJoueur()->couleur()=="Blanc")
            {
                ostringstream os;
                os << Partie->getBlanc()->getCapt() + nbCapt;
                Partie->getBlanc()->addCapt(nbCapt);
                infosBlanc->setCapt(QString::fromStdString(os.str()));

            }
            else
            {
                ostringstream os;
                os << Partie->getNoir()->getCapt() + nbCapt;
                Partie->getNoir()->addCapt(nbCapt);
                infosNoir->setCapt(QString::fromStdString(os.str()));

            }
        }

        goban->avancer();
    }
}

FP::~FP()
{
    if (goban!=0) delete goban;
    if (Partie!=0) delete Partie;
    if (infosJoueur!=0) delete infosJoueur;
}

void FP::fermerFichier()
{
    delete Partie;
    Partie=0;
    goban->init();
    infosNoir->setNom(" "); infosNoir->setNiveau(" ");
    infosBlanc->setNom(" "); infosBlanc->setNiveau(" ");
    //infosJoueurs = new QWidget;
    //infosJoueurs->setFixedWidth(300);
}

void FP::prevMove()
{

}


infosJoueurs::infosJoueurs() : QGridLayout(), j(0)
{
    QLabel* rank = new QLabel("Niveau : ");
    QLabel* name = new QLabel("Nom : ");
    QLabel* capt = new QLabel("Capturées : ");
    nom = new QLabel(" ");
    niveau = new QLabel(" ");
    pierresCapturees= new QLabel(" ");
    titre= new QLabel(" ");

    addWidget(titre,0,0,1,2, Qt::AlignCenter);
    addWidget(name,1,0);
    addWidget(nom,1,1);
    addWidget(rank,2,0);
    addWidget(niveau,2,1);
    addWidget(capt,3,0);
    addWidget(pierresCapturees,3,1);
}

void infosJoueurs::setJoueur(Joueur* J)
{
    j=J;
    nom->setText(j->getNom());
    niveau->setText(j->getRank());
}
