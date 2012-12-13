#include "FenetreSGF.h"
#include "fenetreInfos.h"
#include "ActionNext.h"
#include "../GobanFiles/GobanManager.h"

FenetreSGF::FenetreSGF() : FenetrePrincipale()
{
    QDesktopWidget desk;
    qreal m_height = desk.height();
    ECART = ceil((m_height-150)/(20));
    m_goban.reset(new Goban(ECART,19));



    std::ostringstream os;
    os << "Taille de l'écran : " << m_height << std::endl;
    SGF::Debug::getInstance()->add(SGF::Normal,os.str());

    if (ECART <= 0)
    {
        SGF::Debug::getInstance()->add(SGF::Exception,"Mauvaise récupération de la taille de l'écran");
    }

    SGF::Debug* dbg = SGF::Debug::getInstance();

    try
    {
        pileUndo = new QUndoStack(this);
        viewUndo = new QUndoView(pileUndo);
        messagesExecution = new QTextEdit("Ouverture du programme.");
        //menu
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

        /************ BARRE D'ETAT ***********************/
        barreEtat = statusBar();
        nomFichier = new QLabel("Aucun fichier");


        /*************** BARRE D'OUTILS ******************/
        barreOutils = new QToolBar("nom");
        addToolBar(barreOutils);
        barreOutils->addAction(actionQuitter);

        layoutBoutonsNP =new QHBoxLayout();

        //on ajoute les boutons au layout goban
        //layoutV->addLayout(layoutBoutonsNP);

        vue = new QGraphicsView(m_goban.get());
        vue->setFixedSize(ECART*(m_goban->SIZE()+1),ECART*(m_goban->SIZE()+1));
        layoutV->addWidget(vue);

        /********* Définition d'une grille de boutons correspondant aux intersections *********/
        grilleBoutonsGoban =new QGridLayout();
        for (unsigned int i = 0; i<m_goban->SIZE(); i++)
            for (unsigned int j = 0; j<m_goban->SIZE(); j++)
            {
                BoutonGoban* bouton =new BoutonGoban(i,j,m_goban);
                grilleBoutonsGoban->addWidget(bouton,j+1,i+1);
                connect(bouton,SIGNAL(clicked()),bouton,SLOT(envoyerSignalClicked()));
                connect(bouton,SIGNAL(clickedBouton(int,int)),this,SLOT(bouton_goban(int,int)));
            }
        grilleBoutonsGoban->setSpacing(0);
        grilleBoutonsGoban->setMargin(ECART/2);
        vue->setLayout(grilleBoutonsGoban);

        //Définition du widget pour affichage des infos
        infosJoueur = new QHBoxLayout();

        //construction du widget pour les infos de noir et blanc
        infosNoir =new infosJoueurs();
        infosBlanc = new infosJoueurs();
        infosNoir->setTitre("<b>Noir</b>"); infosBlanc->setTitre("<b>Blanc</b>");

        infosJoueur->addLayout(infosNoir);
        infosJoueur->addLayout(infosBlanc);

        //widget pour l'affichage des commentaires
        commentaires = new QTextEdit();
        commentaires->setFixedHeight(350); commentaires->setFixedWidth(300);
        commentaires->setFrameStyle(2);

        widgetsCote = new QVBoxLayout();
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

        QWidget* m = new QWidget();
        m->setLayout(layoutPrincipal);

        setCentralWidget(m);

        /*Quand on met une pierre sur un bord pour la première fois, le goban se décale ... En attendant d'avoir
        réglé le problème, on met des pierres dans les coins pour que le goban soit à la bonne
        place*/
        QGraphicsPixmapItem* ellipse = new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(ECART*R,ECART*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        ellipse->setX(ECART-(ECART*R/2));
        ellipse->setY(ECART-(ECART*R/2));
        ellipse->setVisible(false);
        m_goban->addItem(ellipse);

        QGraphicsPixmapItem* ellipse2 = new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(ECART*R,ECART*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        ellipse2->setX(m_goban->SIZE()*ECART-(ECART*R/2));
        ellipse2->setY(m_goban->SIZE()*ECART-(ECART*R/2));
        ellipse2->setVisible(false);
        m_goban->addItem(ellipse2);

        //dbg->printToFile();
    }
    catch (std::exception const& e)
    {
        std::ostringstream os;
        os << "Création de la fenêtre principale : " << e.what();
        dbg->add(SGF::Exception,os.str());
        throw(e);
    }

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

    barreOutils->addAction(nouveauFichier);
    barreOutils->addAction(ouvrirFichier);
    barreOutils->addAction(enregistrerFichier);
    barreOutils->addAction(fermerFichier);

    //boutons avancer, reculer, etc
    const int w = 45;
    QPushButton* prev =new QPushButton("<");
    QPushButton* next =new QPushButton(">");
    QPushButton* next5 = new QPushButton(">>");
    QPushButton* prev5 =new QPushButton("<<");
    QPushButton* debutPartie =new QPushButton("Début");
    QPushButton* finPartie =new QPushButton("Fin");
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

    layoutBoutonsNP->addWidget(debutPartie);
    layoutBoutonsNP->addWidget(prev5);
    layoutBoutonsNP->addWidget(prev);
    layoutBoutonsNP->addWidget(next);
    layoutBoutonsNP->addWidget(next5);
    layoutBoutonsNP->addWidget(finPartie);
}

void FenetreSGF::ouvrirFichier()
{
    SGF::Debug* dbg = SGF::Debug::getInstance();
    dbg->add(SGF::Normal,"Ouverture du fichier");

    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Fichier SGF (*.sgf)");
    QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);
    dbg->add(SGF::Normal,"Fichier : "+ fichier.toStdString());
    m_goban->getPartie()->chargerFichier(fichier.toStdString());
    m_goban->setCourant(-1);
    m_goban->setHasPartie(true);

    //infosNoir->setJoueur(m_goban->getPartie().getNoir());
    //infosBlanc->setJoueur(m_goban->getPartie().getBlanc());

    commentaires->setText("Fichier : "+fichier+". Début de la partie.\n Partie jouée le "+m_goban->getPartie()->getDate());
    nomFichier->setText("Fichier : "+fichier);
    barreEtat->addWidget(nomFichier);
}

/*** nextMove : permet de passer au coup suivant
 Si on peut refaire une action contenue dans la pileUndo, on le fait
Sinon on crée une nouvelle ActionNext qu'on met dans la pile
***/
void FenetreSGF::nextMove()
{
    SGF::Debug* dbg = SGF::Debug::getInstance();
    dbg->add(SGF::Normal,"\n * Click:nextMove");
    std::cout << "next move. goban.courant = " << m_goban->getCourant() << std::endl;

    if (pileUndo->canRedo())
    {
        pileUndo->redo();
    }
    else if ((m_goban->getPartie().get()!=0) && (m_goban->getCourant()!=m_goban->getPartie()->getListeCoups().size()))
    {
        std::cout << "Ajout dans la pile undoStack" << std::endl;
        dbg->add(SGF::Normal,"Création d'un new ActionNext");
        ActionNext* an = new ActionNext();
        an->setFenetrePrincipalePtr(sharedFromThis());
        dbg->add(SGF::Normal,"Push dans la pile UndoStack");
        pileUndo->push(an);
        dbg->add(SGF::Normal,an->text().toStdString());

    }
    dbg->add(SGF::Normal," * fin nextMove\n");

}

void FenetreSGF::fermerFichier()
{
    //delete Partie;
    pileUndo->clear();
    //Partie.get()=0;
    m_goban->setHasPartie(false);
    m_goban->init();
    infosNoir->setNom(" "); infosNoir->setNiveau(" ");
    infosBlanc->setNom(" "); infosBlanc->setNiveau(" ");
    commentaires->setText(" ");
}

void FenetreSGF::prevMove()
{
    if (pileUndo->canUndo())
    {
        pileUndo->undo();

    }
}


void FenetreSGF::prev5Moves()
{
    for (unsigned int i = 0; i<5 ; i++)
        prevMove();
}

void FenetreSGF::debutPartie()
{
    pileUndo->clear();
    m_goban->init();
    m_goban->getPartie()->getBlanc()->setCapt(0); m_goban->getPartie()->getNoir()->setCapt(0);
    m_goban->setCourant(0);
    commentaires->setText("Début de la partie.\n Partie jouée le "+m_goban->getPartie()->getDate());

    SGF::Debug::getInstance()->add(SGF::Normal,"\nRetour au début de la partie, nettoyage de la pile d'Undo/Redo, init du goban.\n\n");

}

void FenetreSGF::next5Moves()
{
    if (m_goban->hasPartie())
    {
        for (unsigned int i = 0; i<5; i++)
            FenetreSGF::nextMove();
    }
}

void FenetreSGF::finPartie()
{
    if (m_goban->hasPartie())
    {
        while (m_goban->getCourant()!=m_goban->getPartie()->getListeCoups().size())
            FenetreSGF::nextMove();
    }
}

void FenetreSGF::enregistrerFichier()
{
    if (m_goban->hasPartie())
    {
        QString nomFich = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "SGF (*.sgf)");
        if (nomFich.size()!=0)
            m_goban->getPartie()->enregistrerFichier(nomFich);
    }
}

void FenetreSGF::nouveauFichier()
{
    /*** ouverture d'une boîte de dialogue pour demander le nom des joueurs, leur niveau,
    la date de la partie ***/
    if (m_goban->hasPartie())
    {
        enregistrerFichier();
        fermerFichier();
        FenetreInfos* fi = new FenetreInfos(this);
        fi->show();
    }
    else
    {
        FenetreInfos* fi = new FenetreInfos(this);
        fi->show();
    }
}
