#include "FenetreJeu.h"
#include "DebutJeu.h"
#include "../GobanFiles/GobanIA.h"
#include "../IA/IA.h"

FenetreJeu::FenetreJeu() : FenetrePrincipale()
{
    std::cout << "Cr�ation fen�tre de jeu\n";
    QDesktopWidget desk;
    qreal m_height = desk.height();
    ECART = ceil((m_height-150)/(10));
    m_goban.reset(new GobanIA(ECART,9));
//    m_goban->init();

    std::cout << "0";
    vue = new QGraphicsView(m_goban.get());
    vue->setFixedSize(ECART*(m_goban->SIZE()+1),ECART*(m_goban->SIZE()+1));
    layoutV->addWidget(vue);

    std::cout << "A";

    /********* D�finition d'une grille de boutons correspondant aux intersections *********/
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
    os << "Taille de l'�cran : " << m_height << std::endl;
    SGF::Debug::getInstance()->add(SGF::Normal,os.str());

    if (ECART <= 0)
    {
        SGF::Debug::getInstance()->add(SGF::Exception,"Mauvaise r�cup�ration de la taille de l'�cran");
    }

    SGF::Debug* dbg = SGF::Debug::getInstance();

    std::cout << "C";
    /*Quand on met une pierre sur un bord pour la premi�re fois, le goban se d�cale ... En attendant d'avoir
    r�gl� le probl�me, on met des pierres dans les coins pour que le goban soit � la bonne
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
    std::cout << "Fen�tre de jeu cr��e\n";
}


void FenetreJeu::bouton_goban(int a, int o)
{
    std::cout << "Clicked : " << a << "-" << o << std::endl;
    boost::shared_ptr<GobanIA> gobanPtr = boost::dynamic_pointer_cast<GobanIA>(m_goban);

    if (!gobanPtr->getPartieIA()->partieFinie())
    {
        std::cout << "eeeeeeeeeeeeeeeeeeeeeeeeee\n";
        std::cout << "couleur ia = " << gobanPtr->getPartieIA()->getCouleurIA()
                  << ", couleur � jouer = " << gobanPtr->getPartieIA()->couleurAJouer()
                     << std::endl;
        if (gobanPtr->getPartieIA()->couleurAJouer()!=gobanPtr->getPartieIA()->getCouleurIA())
        {
            //si c'est bien � l'utilisateur de jouer
            std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n";
            boost::shared_ptr<Joueur> joueurUser = gobanPtr->getPartieIA()->getJoueur(gobanPtr->getPartieIA()->couleurAJouer());
            Coup c(a,o,joueurUser);
            gobanPtr->getPartieIA()->ajouterCoup(c);
            boost::shared_ptr<Pierre> pierrePtr(new Pierre(c,gobanPtr->ECART()));
            gobanPtr->ajouterPierre(pierrePtr);

            //ensuite, l'IA doit choisir un coup
            std::pair<int,int> coupIA = gobanPtr->getPartieIA()->getIA()->choixCoup();
            std::cout << "Choix de l'ia : " << coupIA.first << " - " << coupIA.second << std::endl;
            Coup c2(coupIA.first,coupIA.second,gobanPtr->getPartieIA()->getIA());
            gobanPtr->getPartieIA()->ajouterCoup(c2);
            boost::shared_ptr<Pierre> pierre2 (new Pierre(c2,gobanPtr->ECART()));
            gobanPtr->ajouterPierre(pierre2);
        }
    }
}


void FenetreJeu::init()
{
    m_goban->init();
}
