#include "FenetreSGF.h"
#include "fenetreInfos.h"
#include "ActionNext.h"
#include "../GobanFiles/GobanManager.h"
#include "../GobanFiles/GobanIA.h"

using namespace std;

FenetreSGF::FenetreSGF() : FenetrePrincipale()
{
    QDesktopWidget desk;
    qreal m_height = desk.height();
    ECART = ceil((m_height-150)/(20));
    m_goban.reset(new Goban(ECART,19));

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




        /************ BARRE D'ETAT ***********************/
        barreEtat = statusBar();
        nomFichier = new QLabel("Aucun fichier");


        /*************** BARRE D'OUTILS ******************/
        barreOutils = new QToolBar("nom");
        addToolBar(barreOutils);
        QAction* actionQuitter =menuFichier->addAction("Quitter");
        actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
        connect(actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));
        barreOutils->addAction(actionQuitter);

        layoutBoutonsNP =new QHBoxLayout();

        //on ajoute les boutons au layout goban
        //layoutV->addLayout(layoutBoutonsNP);



        //widget pour l'affichage des commentaires
        commentaires = new QTextEdit();
        commentaires->setFixedHeight(350); commentaires->setFixedWidth(300);
        commentaires->setFrameStyle(2);


        widgetsCote->addLayout(layoutBoutonsNP);
        widgetsCote->addWidget(commentaires);
        //widgetsCote->addWidget(viewUndo);
        viewUndo->setFixedWidth(300);
        //widgetsCote->addSpacing(200);
        //infosPartie->setFont();



        /*Quand on met une pierre sur un bord pour la première fois, le goban se décale ... En attendant d'avoir
        réglé le problème, on met des pierres dans les coins pour que le goban soit à la bonne
        place*/
        QGraphicsPixmapItem* ellipse = new QGraphicsPixmapItem(QPixmap("Images/pierreNoire.png").scaled(ECART*R,ECART*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        ellipse->setX(ECART-(ECART*R/2));
        ellipse->setY(ECART-(ECART*R/2));
        ellipse->setVisible(false);
        m_goban->addItem(ellipse);

        QGraphicsPixmapItem* ellipse2 = new QGraphicsPixmapItem(QPixmap("Images/pierreNoire.png").scaled(ECART*R,ECART*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
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


void FenetreSGF::bouton_goban(int a, int o)
{

    cout << "Clicked : " << a << "-" << o << endl;

        /* Si la partie n'a encore aucun coup, c'est le premier, donc à noir de jouer
        Sinon, on regarde le dernier coup joué.*/
        if (m_goban->getPartie()->getListeCoups().empty())
        {
            cout << "Debut :\n";
            Coup c (a,o,m_goban->getPartie()->getNoir());
            cout << "Ajout du premier coup pour noir en " << a << "-" << o << endl;
            /*if (!commentaires->toPlainText().toStdString().empty())
            {
                //s'il y a du texte dans la zone de commentaires, on l'ajoute au coup
                c->addComm(commentaires->toPlainText());
            }*/

            m_goban->getPartie()->ajouterCoup(c);
            boost::shared_ptr<Pierre> p (new Pierre(c, m_goban->ECART()));
            m_goban->ajouterPierre(p);
            m_goban->setCourant(-1);
        }
        else if (m_goban->getPartie()->getCoup(m_goban->getCourant()).couleur()=="Noir")
        {
            Coup c(a,o,m_goban->getPartie()->getBlanc());

            m_goban->getPartie()->ajouterCoup(c);
            boost::shared_ptr<Pierre> p(new Pierre(c, m_goban->ECART()));
            m_goban->ajouterPierre(p);
            m_goban->avancer();
        }
        else if (m_goban->getPartie()->getCoup(m_goban->getCourant()).couleur()=="Blanc")
        {
            Coup c(a,o,m_goban->getPartie()->getNoir());

            m_goban->getPartie()->ajouterCoup(c);
            boost::shared_ptr<Pierre> p(new Pierre(c, m_goban->ECART()));
            m_goban->ajouterPierre(p);
            //goban->avancer();
        }
        //else throw coup_exception("Impossible d'ajouter un coup.");*/

        /* Test : ok
        boost::shared_ptr<QGraphicsPixmapItem> ellipse2 = new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(E*R,E*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        ellipse2->setX((a+1)*E-(E*R/2));
        ellipse2->setY((o+1)*E-(E*R/2));
        goban->addItem(ellipse2);*/



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
