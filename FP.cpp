#include "FP.h"
#include "partie.h"

FP::FP() : QMainWindow(), Partie(0)
{
    //menu
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");
    QMenu *menuOptions = menuBar()->addMenu("&Options");


    QAction* ouvrirFichier = menuFichier->addAction("Ouvrir");
    ouvrirFichier->setShortcut(QKeySequence("Ctrl+O"));
    connect(ouvrirFichier,SIGNAL(triggered()),this,SLOT(ouvrirFichier()));

    QAction* fermerFichier = menuFichier->addAction("Fermer");
    fermerFichier->setShortcut(QKeySequence("Ctrl+W"));
    connect(fermerFichier,SIGNAL(triggered()),this,SLOT(fermerFichier()));

    QAction* actionQuitter = menuFichier->addAction("Quitter");
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));

    QMenu *changerFond = menuOptions->addMenu("Choisir le fond du goban");
    QAction* changerFondClair = changerFond->addAction("Fond clair");
    QAction* changerFondMoyen = changerFond->addAction("Fond moyen");
    QAction* changerFondFonce = changerFond->addAction("Fond foncé");
    QAction* changerFondSansMotif = changerFond->addAction("Aucun motif");
    connect(changerFondClair,SIGNAL(triggered()),this,SLOT(changerFondClair()));
    connect(changerFondMoyen,SIGNAL(triggered()),this,SLOT(changerFondMoyen()));
    connect(changerFondFonce,SIGNAL(triggered()),this,SLOT(changerFondFonce()));
    connect(changerFondSansMotif,SIGNAL(triggered()),this,SLOT(changerFondSansMotif()));


    //Layout horizontal principal : à gauche le goban, à droite les infos
    layoutPrincipal = new QHBoxLayout;
    //Layout vertical pour le goban de la fenêtre :
    layoutV = new QVBoxLayout;


    /*************** BARRE D'OUTILS ******************/
    QToolBar* barreOutils = new QToolBar("nom");
    addToolBar(barreOutils);
    barreOutils->addAction(ouvrirFichier);
    barreOutils->addAction(fermerFichier);
    barreOutils->addAction(actionQuitter);

    //boutons avancer et reculer
    QPushButton* prev = new QPushButton("<");
    QPushButton* next = new QPushButton(">");
    QPushButton* next5 = new QPushButton(">>");
    QPushButton* prev5 = new QPushButton("<<");
    QPushButton* debutPartie = new QPushButton("Début");
    QPushButton* finPartie = new QPushButton("Fin");
    next->setShortcut(QKeySequence::MoveToNextChar);
    connect(next,SIGNAL(clicked()),this,SLOT(nextMove()));
    connect(prev,SIGNAL(clicked()),this,SLOT(prevMove()));
    connect(next5,SIGNAL(clicked()),this,SLOT(next5Moves()));
    connect(prev5,SIGNAL(clicked()),this,SLOT(prev5Moves()));
    connect(debutPartie,SIGNAL(clicked()),this,SLOT(debutPartie()));
    connect(finPartie,SIGNAL(clicked()),this,SLOT(finPartie()));
    QHBoxLayout* layoutBoutonsNP = new QHBoxLayout;
    layoutBoutonsNP->addWidget(debutPartie);
    layoutBoutonsNP->addWidget(prev5);
    layoutBoutonsNP->addWidget(prev);
    layoutBoutonsNP->addWidget(next);
    layoutBoutonsNP->addWidget(next5);
    layoutBoutonsNP->addWidget(finPartie);

    /*layoutBoutons = new QVBoxLayout;
    layoutBoutons->addLayout(layoutBoutonsOFQ);
    layoutBoutons->addLayout(layoutBoutonsNP);*/


    //on ajoute les boutons au layout goban
    //layoutV->addLayout(layoutBoutonsNP);

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

    //widget pour l'affichage des commentaires
    commentaires = new QTextEdit;
    commentaires->setFixedHeight(350); commentaires->setFixedWidth(300);
    commentaires->setFrameStyle(2);

    widgetsCote = new QVBoxLayout;
    widgetsCote->addLayout(infosJoueur);
    widgetsCote->addLayout(layoutBoutonsNP);
    widgetsCote->addWidget(commentaires);
    widgetsCote->addSpacing(200);
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

    commentaires->setText("Fichier : "+fichier+". Début de la partie.\n Partie jouée le "+Partie->getDate());

}


void FP::nextMove()
{
    if (Partie!=0)
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
            ostringstream os;
            os << goban->getCourant().getPtr()->getNum();
            commentaires->setText("Coup numéro "+QString::fromStdString(os.str())+"\n "
                                  +goban->getCourant().getPtr()->getComm());

            goban->avancer();
        }
        else commentaires->setText("Fin de la partie. Résultat : " + Partie->getResultat());
    }
}

FP::~FP()
{
    if (goban!=0) delete goban;
    if (Partie!=0) delete Partie;
    if (infosJoueur!=0) delete infosJoueur;
    if (commentaires!=0) delete commentaires;
}

void FP::fermerFichier()
{
    delete Partie;
    Partie=0;
    goban->init();
    infosNoir->setNom(" "); infosNoir->setNiveau(" ");
    infosBlanc->setNom(" "); infosBlanc->setNiveau(" ");
    commentaires->setText(" ");
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


void FP::prev5Moves()
{

}

void FP::debutPartie()
{
    goban->init();
    Partie->getBlanc()->setCapt(0); Partie->getNoir()->setCapt(0);
    goban->setCourant(Partie->debut());
    commentaires->setText("Début de la partie.\n Partie jouée le "+Partie->getDate());

}

void FP::next5Moves()
{
    if (Partie!=0)
    {
        for (unsigned int i = 0; i<5; i++)
            FP::nextMove();
    }
}

void FP::finPartie()
{
    if (Partie!=0)
    {
        while (goban->getCourant()!=Partie->fin())
            FP::nextMove();
    }
}

void FP::changerFondClair()
{
    goban->setBackgroundBrush(goban->getBrushClair());
}

void FP::changerFondFonce()
{
    goban->setBackgroundBrush(goban->getBrushFonce());
}

void FP::changerFondMoyen()
{
    goban->setBackgroundBrush(goban->getBrushMoyen());
}

void FP::changerFondSansMotif()
{
    goban->setBackgroundBrush(goban->getBrushSansMotif());
}
