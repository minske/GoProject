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
    messagesExecution = new QTextEdit("Ouverture du programme.");
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

    QAction* msgExec = menuOptions->addAction("Afficher les messages d'exécution");
    msgExec->setShortcut(QKeySequence("Ctrl+L"));
    connect(msgExec,SIGNAL(triggered()),this,SLOT(afficherMsgExec()));

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

    /********* Définition d'une grille de boutons correspondant aux intersections *********/
    grilleBoutonsGoban = new QGridLayout;
    for (unsigned int i = 0; i<19; i++)
        for (unsigned int j = 0; j<19; j++)
        {
            BoutonGoban* bouton = new BoutonGoban(i,j);
            grilleBoutonsGoban->addWidget(bouton,j+1,i+1);
            connect(bouton,SIGNAL(clicked()),bouton,SLOT(envoyerSignalClicked()));
            connect(bouton,SIGNAL(clickedBouton(int,int)),this,SLOT(bouton_goban(int,int)));
        }
    grilleBoutonsGoban->setSpacing(0);
    grilleBoutonsGoban->setMargin(E/2);
    vue->setLayout(grilleBoutonsGoban);

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

    /*Quand on met une pierre sur un bord pour la première fois, le goban se décale ... En attendant d'avoir
    réglé le problème, on met des pierres dans les coins pour que le goban soit à la bonne
    place*/
    QGraphicsPixmapItem* ellipse = new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    ellipse->setX(E-(E*R/2));
    ellipse->setY(E-(E*R/2));
    ellipse->setVisible(false);
    goban->addItem(ellipse);

    QGraphicsPixmapItem* ellipse2 = new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    ellipse2->setX(19*E-(E*R/2));
    ellipse2->setY(19*E-(E*R/2));
    ellipse2->setVisible(false);
    goban->addItem(ellipse2);


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
        //std::cout << "Ajout dans la pile undoStack" << std::endl;
        messagesExecution->append("Ajout dans la pile undoStack");
        actionNext* an = new actionNext(this);
        pileUndo->push(an);
        messagesExecution->append(an->text());

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

void FP::bouton_goban(int a, int o)
{

    if ((mode==creationSGF) && (Partie!=0))
    {
        /* Si la partie n'a encore aucun coup, c'est le premier, donc à noir de jouer
        Sinon, on regarde le dernier coup joué.*/
        if (goban->getCourant()==0)
        {
            Coup* c = new Coup(a,o,Partie->getNoir());

            Partie->ajouterCoup(*c);
            Pierre* p = new Pierre(c);
            goban->ajouterPierre(p);
            goban->setCourant(Partie->debut());
        }
        else if ((*(goban->getCourant())).getJoueur()->couleur()=="Noir")
        {
            Coup* c = new Coup(a,o,Partie->getBlanc());

            Partie->ajouterCoup(*c);
            Pierre* p = new Pierre(c);
            goban->ajouterPierre(p);
            goban->avancer();
        }
        else if ((*(goban->getCourant())).getJoueur()->couleur()=="Blanc")
        {
            Coup* c = new Coup(a,o,Partie->getNoir());

            Partie->ajouterCoup(*c);
            Pierre* p = new Pierre(c);
            goban->ajouterPierre(p);
            goban->avancer();
        }
        else throw coup_exception("Impossible d'ajouter un coup.");

        /* Test : ok
        QGraphicsPixmapItem* ellipse2 = new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        ellipse2->setX((a+1)*E-(E*R/2));
        ellipse2->setY((o+1)*E-(E*R/2));
        goban->addItem(ellipse2);*/
    }


    /* TEST : on récupère les coordonnées correctes

    ostringstream os;
    os << "Vous avez cliqué sur " << a << "-" << o;
    QWidget* widg = new QWidget;
    QVBoxLayout* lv = new QVBoxLayout;
    QLabel* txt = new QLabel(QString::fromStdString(os.str()));
    QPushButton* ok = new QPushButton("Ok");
    lv->addWidget(txt); lv->addWidget(ok);
    connect(ok,SIGNAL(clicked()),widg,SLOT(close()));
    widg->setLayout(lv);
    widg->show();*/
}

void FP::afficherMsgExec()
{
    messagesExecution->show();
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
