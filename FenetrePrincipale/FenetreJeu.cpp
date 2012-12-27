#include "FenetreJeu.h"
#include "DebutJeu.h"
#include "../GobanFiles/GobanIA.h"

FenetreJeu::FenetreJeu() : FenetrePrincipale()
{
    std::cout << "Création fenêtre de jeu\n";
    QDesktopWidget desk;
    qreal m_height = desk.height();
    ECART = ceil((m_height-150)/(10));
    m_goban.reset(new GobanIA(ECART,9));

    std::cout << "0";
    vue = new QGraphicsView(m_goban.get());
    vue->setFixedSize(ECART*(m_goban->SIZE()+1),ECART*(m_goban->SIZE()+1));
    layoutV->addWidget(vue);

    std::cout << "A";

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

    std::cout << "B";

    std::ostringstream os;
    os << "Taille de l'écran : " << m_height << std::endl;
    SGF::Debug::getInstance()->add(SGF::Normal,os.str());

    if (ECART <= 0)
    {
        SGF::Debug::getInstance()->add(SGF::Exception,"Mauvaise récupération de la taille de l'écran");
    }

    SGF::Debug* dbg = SGF::Debug::getInstance();

    std::cout << "C";
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
    std::cout << "D";
    //DebutJeu* wdw = new DebutJeu();
    //wdw->show();
    std::cout << "Fenêtre de jeu créée\n";
}


void FenetreJeu::bouton_goban(int a, int o)
{
    std::cout << "Clicked : " << a << "-" << o << std::endl;
    if (!m_goban->getPartieIA()->partieFinie())
    {
        if (m_goban->getPartieIA()->couleurAJouer()!=m_goban->getPartieIA()->getCouleurIA())
        {
            //si c'est bien à l'utilisateur de jouer
            boost::shared_ptr<Joueur> joueurUser = m_goban->getPartieIA()->getJoueur(m_goban->getPartieIA()->couleurAJouer());
            Coup c(a,o,joueurUser);
            m_goban->getPartieIA()->ajouterCoup(c);
            boost::shared_ptr<Pierre> pierrePtr(new Pierre(c,9));
            m_goban->ajouterPierre(pierrePtr);

            //ensuite, l'IA doit choisir un coup
        }
    }
}
