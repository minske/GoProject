#include "FP.h"
#include "partie.h"
#include "actionNext.h"

/******************************************************************************************************************/
/********************************************               *******************************************************/
/********************************************   CLASSE FP   *******************************************************/
/********************************************               *******************************************************/
/******************************************************************************************************************/


FP::FP() : QMainWindow(), Partie(0), pileUndo(0), mode(lectureSGF)
{
    pileUndo = new QUndoStack(this);
    viewUndo = new QUndoView(pileUndo);
    //menu
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");
    QMenu *menuOptions = menuBar()->addMenu("&Options");

    QAction* nouveauFichier = menuFichier->addAction("Nouveau");
    nouveauFichier->setShortcut(QKeySequence("Ctrl+N"));
    connect(nouveauFichier,SIGNAL(triggered()),this,SLOT(nouveauFichier()));

    QAction* ouvrirFichier = menuFichier->addAction("Ouvrir");
    ouvrirFichier->setShortcut(QKeySequence("Ctrl+O"));
    connect(ouvrirFichier,SIGNAL(triggered()),this,SLOT(ouvrirFichier()));

    QAction* enregistrerFichier = menuFichier->addAction("Enregistrer");
    enregistrerFichier->setShortcut(QKeySequence("Ctrl+S"));
    connect(enregistrerFichier,SIGNAL(triggered()),this,SLOT(enregistrerFichier()));

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

    /************ BARRE D'ETAT ***********************/
    barreEtat = statusBar();
    nomFichier = new QLabel("Aucun fichier");


    /*************** BARRE D'OUTILS ******************/
    QToolBar* barreOutils = new QToolBar("nom");
    addToolBar(barreOutils);
    barreOutils->addAction(nouveauFichier);
    barreOutils->addAction(ouvrirFichier);
    barreOutils->addAction(enregistrerFichier);
    barreOutils->addAction(fermerFichier);
    barreOutils->addAction(actionQuitter);

    //boutons avancer et reculer
    const int w = 45;
    QPushButton* prev = new QPushButton("<");
    QPushButton* next = new QPushButton(">");
    QPushButton* next5 = new QPushButton(">>");
    QPushButton* prev5 = new QPushButton("<<");
    QPushButton* debutPartie = new QPushButton("Début");
    QPushButton* finPartie = new QPushButton("Fin");
    prev->setFixedWidth(w);
    next->setFixedWidth(w);
    prev5->setFixedWidth(w);
    next5->setFixedWidth(w);
    debutPartie->setFixedWidth(w);
    finPartie->setFixedWidth(w);
    next->setShortcut(QKeySequence::MoveToNextChar);
    prev->setShortcut(QKeySequence::MoveToPreviousChar);
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
    //widgetsCote->addWidget(viewUndo);
    viewUndo->setFixedWidth(300);
    //widgetsCote->addSpacing(200);
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
    if (Partie!=0) FP::fermerFichier();

    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Fichier SGF (*.sgf)");
    QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);
    Partie = partie::donneInstance();
    Partie->chargerFichier(fichier.toStdString());
    goban->setCourant(partie::iterateur(Partie->debut()));

    infosNoir->setJoueur(Partie->getNoir());
    infosBlanc->setJoueur(Partie->getBlanc());

    commentaires->setText("Fichier : "+fichier+". Début de la partie.\n Partie jouée le "+Partie->getDate());
    nomFichier->setText("Fichier : "+fichier);
    barreEtat->addWidget(nomFichier);

}


/*** nextMove : permet de passer au coup suivant
 Si on peut refaire une action contenue dans la pileUndo, on le fait
Sinon on crée une nouvelle actionNext qu'on met dans la pile
***/
void FP::nextMove()
{
    if (pileUndo->canRedo())
    {
        pileUndo->redo();
    }

    else if ((Partie!=0) && (goban->getCourant()!=Partie->fin()))
    {
        std::cout << "Ajout dans la pile undoStack" << std::endl;
        pileUndo->push(new actionNext(this));

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
    pileUndo->clear();
    Partie=0;
    goban->init();
    infosNoir->setNom(" "); infosNoir->setNiveau(" ");
    infosBlanc->setNom(" "); infosBlanc->setNiveau(" ");
    commentaires->setText(" ");
}

void FP::prevMove()
{
    if (pileUndo->canUndo())
    {
        pileUndo->undo();

    }
}


void FP::prev5Moves()
{
    for (unsigned int i = 0; i<5 ; i++)
        prevMove();
}

void FP::debutPartie()
{
    pileUndo->clear();
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


void FP::enregistrerFichier()
{
    if ((Partie!=0) && (mode==creationSGF))
    {
        QString nomFich = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "SGF (*.sgf)");
        if (nomFich.size()!=0)
            Partie->enregistrerFichier(nomFich);
    }
}

void FP::nouveauFichier()
{
    /*** ouverture d'une boîte de dialogue pour demander le nom des joueurs, leur niveau,
    la date de la partie ***/
    if ((Partie!=0) && (mode==creationSGF))
    {
        enregistrerFichier();
    }
    else if (Partie!=0)
    {
        fermerFichier();
        FenetreInfos* fi = new FenetreInfos(this);
        fi->show();
        mode=creationSGF;
    }
    else
    {
        FenetreInfos* fi = new FenetreInfos(this);
        fi->show();
        mode=creationSGF;
    }
}


/******************************************************************************************************************/
/********************************************                     *************************************************/
/******************************************** CLASSE FENETREINFOS *************************************************/
/********************************************                     *************************************************/
/******************************************************************************************************************/

/********** Constructeur **********************************************************/
FenetreInfos::FenetreInfos(FP* f) : QWidget(), fenPrincipale(f)
{
    /*** Définition d'un layout pour positionner les infos à demander */
    QGridLayout* layoutGrille = new QGridLayout;
    QGroupBox* box = new QGroupBox("Informations sur la partie");
    QLabel* jn = new QLabel("<b>Joueur noir</b>");
    QLabel* jb = new QLabel("<b>Joueur blanc</b>");
    QLabel* nn = new QLabel("Nom");
    QLabel* nb = new QLabel("Nom");
    QLabel* Nn = new QLabel("Niveau");
    QLabel* Nb = new QLabel("Niveau");
    QLabel* d = new QLabel("Date");

    nomNoir = new QLineEdit("Noir");
    nomBlanc = new QLineEdit("Blanc");
    niveauNoir = new QLineEdit("-");
    niveauBlanc = new QLineEdit("-");
    datePartie = new QLineEdit("date");

    layoutGrille->addWidget(jn,0,0,1,2,Qt::AlignCenter); layoutGrille->addWidget(jb,0,2,1,2,Qt::AlignCenter);
    layoutGrille->addWidget(nn,1,0); layoutGrille->addWidget(nomNoir,1,1);
    layoutGrille->addWidget(nb,1,2); layoutGrille->addWidget(nomBlanc,1,3);
    layoutGrille->addWidget(Nn,2,0); layoutGrille->addWidget(niveauNoir,2,1);
    layoutGrille->addWidget(Nb,2,2); layoutGrille->addWidget(niveauBlanc,2,3);
    layoutGrille->addWidget(d,3,0,1,2,Qt::AlignRight); layoutGrille->addWidget(datePartie,3,2,1,2);

    QPushButton* valider = new QPushButton("Valider");
    QPushButton* annuler = new QPushButton("Annuler");
    layoutGrille->addWidget(valider,4,0,1,2);
    layoutGrille->addWidget(annuler,4,2,1,2);
    box->setLayout(layoutGrille);
    QVBoxLayout* layoutP = new QVBoxLayout;
    layoutP->addWidget(box);
    setLayout(layoutP);
    connect(valider,SIGNAL(clicked()),this,SLOT(valider()));
    connect(annuler,SIGNAL(clicked()),this,SLOT(annuler()));
}

/**********  SLOT annuler : fermeture de la fenêtre **********/
void FenetreInfos::annuler()
{
    close();
}

FenetreInfos::~FenetreInfos()
{
    delete nomNoir; delete nomBlanc; delete niveauNoir; delete niveauBlanc; delete datePartie;
}

/******** SLOT valider : vérification des infos entrées et création d'une instance de Partie ********/
void FenetreInfos::valider()
{
    QString nom_noir, nom_Blanc, niveau_Noir, niveau_Blanc, date_partie;
    if (nomNoir->text().size()==0) nom_noir="Noir"; else nom_noir=nomNoir->text();
    if (nomBlanc->text().size()==0) nom_Blanc="Blanc"; else nom_Blanc=nomBlanc->text();
    if (niveauNoir->text().size()==0) niveau_Noir="-"; else niveau_Noir=niveauNoir->text();
    if (niveauBlanc->text().size()==0) niveau_Blanc="-"; else niveau_Blanc=niveauBlanc->text();
    if (datePartie->text().size()==0) date_partie="inconnue"; else date_partie=datePartie->text();
    fenPrincipale->setPartie(partie::donneInstance(nom_noir,nom_Blanc,niveau_Noir,niveau_Blanc,date_partie));

    fenPrincipale->getInfosNoir()->setJoueur(fenPrincipale->getPartie()->getNoir());
    fenPrincipale->getInfosBlanc()->setJoueur(fenPrincipale->getPartie()->getBlanc());
    close();
}



/***************************************************************************************************************/
/*************************************                             *********************************************/
/*************************************     CLASSE INFOSJOUEURS     *********************************************/
/*************************************                             *********************************************/
/***************************************************************************************************************/


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
    pierresCapturees->setText("0");
}
