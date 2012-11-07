#include "FP.h"
#include "partie.h"
#include "actionNext.h"
#include "debug.h"

double FP::ECART = 0;

/******************************************************************************************************************/
/********************************************               *******************************************************/
/********************************************   CLASSE FP   *******************************************************/
/********************************************               *******************************************************/
/******************************************************************************************************************/


FP::FP() : QMainWindow(), mode(lectureSGF)
{
    QDesktopWidget desk;
    qreal m_height = desk.height();
    ECART = ceil((m_height-150)/(Goban::SIZE()+1));

    std::ostringstream os;
    os << "Taille de l'écran : " << m_height << std::endl;
    SGF::Debug::getInstance()->add(SGF::Normal,os.str());

    if (ECART <= 0)
    {
        SGF::Debug::getInstance()->add(SGF::Exception,"Mauvaise récupération de la taille de l'écran");
    }

    SGF::Debug* dbg = SGF::Debug::getInstance();

    try {
        pileUndo = new QUndoStack(this);
        viewUndo = new QUndoView(pileUndo);
        messagesExecution = new QTextEdit("Ouverture du programme.");
        //menu
        QMenu* menuFichier = menuBar()->addMenu("&Fichier");
        QMenu* menuOptions = menuBar()->addMenu("&Options");

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

        QAction* actionQuitter =menuFichier->addAction("Quitter");
        actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
        connect(actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));

        QMenu* changerFond = menuOptions->addMenu("Choisir le fond du goban");
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
        layoutPrincipal = new QHBoxLayout();
        //Layout vertical pour le goban de la fenêtre :
        layoutV = new QVBoxLayout();

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
        QHBoxLayout* layoutBoutonsNP =new QHBoxLayout();
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
        Goban* goban = Goban::getInstance();
        vue = new QGraphicsView(goban);
        vue->setFixedSize(ECART*(Goban::SIZE()+1),ECART*(Goban::SIZE()+1));
        layoutV->addWidget(vue);

        /********* Définition d'une grille de boutons correspondant aux intersections *********/
        grilleBoutonsGoban =new QGridLayout();
        for (unsigned int i = 0; i<Goban::SIZE(); i++)
            for (unsigned int j = 0; j<Goban::SIZE(); j++)
            {
                BoutonGoban* bouton =new BoutonGoban(i,j);
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
        goban->addItem(ellipse);

        QGraphicsPixmapItem* ellipse2 = new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(ECART*R,ECART*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        ellipse2->setX(Goban::SIZE()*ECART-(ECART*R/2));
        ellipse2->setY(Goban::SIZE()*ECART-(ECART*R/2));
        ellipse2->setVisible(false);
        goban->addItem(ellipse2);

        //dbg->printToFile();
    }
    catch (std::exception const& e)
    {
        std::ostringstream os;
        os << "Création de la fenêtre principale : " << e.what();
        dbg->add(SGF::Exception,os.str());
        throw(e);
    }
}


void FP::ouvrirFichier()
{
    Goban* goban = Goban::getInstance();
    SGF::Debug* dbg = SGF::Debug::getInstance();
    dbg->add(SGF::Normal,"Ouverture du fichier");

    if (Partie.get()!=0) FP::fermerFichier();

    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Fichier SGF (*.sgf)");
    QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);
    dbg->add(SGF::Normal,"Fichier : "+ fichier.toStdString());
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
    SGF::Debug* dbg = SGF::Debug::getInstance();
    Goban* goban = Goban::getInstance();
    dbg->add(SGF::Normal,"\n * Click:nextMove");
    if (pileUndo->canRedo())
    {
        pileUndo->redo();
    }

    else if ((Partie.get()!=0) && (goban->getCourant()!=Partie->fin()))
    {
        //std::cout << "Ajout dans la pile undoStack" << std::endl;
        dbg->add(SGF::Normal,"Création d'un new ActionNext");
        actionNext* an = new actionNext();
        an->setFpPtr(shared_from_this());
        dbg->add(SGF::Normal,"Push dans la pile UndoStack");
        pileUndo->push(an);
        dbg->add(SGF::Normal,an->text().toStdString());

    }
    dbg->add(SGF::Normal," * fin nextMove\n");

}

FP::~FP()
{/*
    if (goban!=0) delete goban;
    if (Partie!=0) delete Partie;
    if (infosJoueur!=0) delete infosJoueur;
    if (commentaires!=0) delete commentaires;*/
}

void FP::fermerFichier()
{
    Goban* goban = Goban::getInstance();
    //delete Partie;
    pileUndo->clear();
    //Partie.get()=0;
    Partie.reset();
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
    Goban* goban = Goban::getInstance();
    pileUndo->clear();
    goban->init();
    Partie->getBlanc()->setCapt(0); Partie->getNoir()->setCapt(0);
    goban->setCourant(Partie->debut());
    commentaires->setText("Début de la partie.\n Partie jouée le "+Partie->getDate());

    SGF::Debug::getInstance()->add(SGF::Normal,"\nRetour au début de la partie, nettoyage de la pile d'Undo/Redo, init du goban.\n\n");

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
    Goban* goban = Goban::getInstance();
    if (Partie!=0)
    {
        while (goban->getCourant()!=Partie->fin())
            FP::nextMove();
    }
}

void FP::changerFondClair()
{
    Goban* goban = Goban::getInstance();

    goban->setBackgroundBrush(goban->getBrushClair());
}

void FP::changerFondFonce()
{
    Goban* goban = Goban::getInstance();

    goban->setBackgroundBrush(goban->getBrushFonce());
}

void FP::changerFondMoyen()
{
    Goban* goban = Goban::getInstance();

    goban->setBackgroundBrush(goban->getBrushMoyen());
}

void FP::changerFondSansMotif()
{
    Goban* goban = Goban::getInstance();

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
        boost::shared_ptr<FenetreInfos> fi (new FenetreInfos(this->f()));
        fi->show();
        mode=creationSGF;
    }
    else
    {
        boost::shared_ptr<FenetreInfos> fi (new FenetreInfos(this->f()));
        fi->show();
        mode=creationSGF;

    }
}

void FP::bouton_goban(int a, int o)
{
    Goban* goban = Goban::getInstance();


    if ((mode==creationSGF) && (Partie!=0))
    {
        /* Si la partie n'a encore aucun coup, c'est le premier, donc à noir de jouer
        Sinon, on regarde le dernier coup joué.*/
        if (goban->getCourant()==Partie->debut())
        {
            boost::shared_ptr<Coup> c (new Coup(a,o,Partie->getNoir()));
            /*if (!commentaires->toPlainText().toStdString().empty())
            {
                //s'il y a du texte dans la zone de commentaires, on l'ajoute au coup
                c->addComm(commentaires->toPlainText());
            }*/

            Partie->ajouterCoup(*c);
            boost::shared_ptr<Pierre> p (new Pierre(c));
            goban->ajouterPierre(p);
            goban->setCourant(Partie->debut());
        }
        else if ((*(goban->getCourant())).getJoueur()->couleur()=="Noir")
        {
            boost::shared_ptr<Coup> c(new Coup(a,o,Partie->getBlanc()));

            Partie->ajouterCoup(*c);
            boost::shared_ptr<Pierre> p(new Pierre(c));
            goban->ajouterPierre(p);
            goban->avancer();
        }
        else if ((*(goban->getCourant())).getJoueur()->couleur()=="Blanc")
        {
            boost::shared_ptr<Coup> c(new Coup(a,o,Partie->getNoir()));

            Partie->ajouterCoup(*c);
            boost::shared_ptr<Pierre> p(new Pierre(c));
            goban->ajouterPierre(p);
            goban->avancer();
        }
        else throw coup_exception("Impossible d'ajouter un coup.");

        /* Test : ok
        boost::shared_ptr<QGraphicsPixmapItem> ellipse2 = new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        ellipse2->setX((a+1)*E-(E*R/2));
        ellipse2->setY((o+1)*E-(E*R/2));
        goban->addItem(ellipse2);*/
    }


    /* TEST : on récupère les coordonnées correctes

    ostringstream os;
    os << "Vous avez cliqué sur " << a << "-" << o;
    QWidget* widg = new QWidget;
    QVBoxLayout* lv = new QVBoxLayout;
    boost::shared_ptr<QLabel> txt = new QLabel(QString::fromStdString(os.str()));
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


infosJoueurs::infosJoueurs() : QGridLayout()
{
    QLabel* rank =new QLabel("Niveau : ");
    QLabel* name =new QLabel("Nom : ");
    QLabel* capt =new QLabel("Capturées : ");
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

void infosJoueurs::setJoueur(boost::shared_ptr<Joueur> J)
{
    j=J;
    nom->setText(j->getNom());
    niveau->setText(j->getRank());
    pierresCapturees->setText("0");
}
