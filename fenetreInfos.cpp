#include "fenetreInfos.h"

/******************************************************************************************************************/
/********************************************                     *************************************************/
/******************************************** CLASSE FENETREINFOS *************************************************/
/********************************************                     *************************************************/
/******************************************************************************************************************/

/********** Constructeur **********************************************************/
FenetreInfos::FenetreInfos(boost::shared_ptr<FP> f) : QWidget(), fenPrincipale(f)
{
    /*** Définition d'un layout pour positionner les infos à demander */
    boost::shared_ptr<QGridLayout> layoutGrille (new QGridLayout);
    boost::shared_ptr<QGroupBox> box (new QGroupBox("Informations sur la partie"));
    boost::shared_ptr<QLabel> jn (new QLabel("<b>Joueur noir</b>"));
    boost::shared_ptr<QLabel> jb (new QLabel("<b>Joueur blanc</b>"));
    boost::shared_ptr<QLabel> nn (new QLabel("Nom"));
    boost::shared_ptr<QLabel> nb (new QLabel("Nom"));
    boost::shared_ptr<QLabel> Nn (new QLabel("Niveau"));
    boost::shared_ptr<QLabel> Nb (new QLabel("Niveau"));
    boost::shared_ptr<QLabel> d (new QLabel("Date"));

    nomNoir = boost::shared_ptr<QLineEdit>(new QLineEdit("Noir"));
    nomBlanc =boost::shared_ptr<QLineEdit>( new QLineEdit("Blanc"));
    niveauNoir = boost::shared_ptr<QLineEdit>(new QLineEdit("-"));
    niveauBlanc = boost::shared_ptr<QLineEdit>(new QLineEdit("-"));
    datePartie = boost::shared_ptr<QLineEdit>(new QLineEdit("date"));

    layoutGrille->addWidget(jn.get(),0,0,1,2,Qt::AlignCenter); layoutGrille->addWidget(jb.get(),0,2,1,2,Qt::AlignCenter);
    layoutGrille->addWidget(nn.get(),1,0); layoutGrille->addWidget(nomNoir.get(),1,1);
    layoutGrille->addWidget(nb.get(),1,2); layoutGrille->addWidget(nomBlanc.get(),1,3);
    layoutGrille->addWidget(Nn.get(),2,0); layoutGrille->addWidget(niveauNoir.get(),2,1);
    layoutGrille->addWidget(Nb.get(),2,2); layoutGrille->addWidget(niveauBlanc.get(),2,3);
    layoutGrille->addWidget(d.get(),3,0,1,2,Qt::AlignRight); layoutGrille->addWidget(datePartie.get(),3,2,1,2);

    boost::shared_ptr<QPushButton> valider (new QPushButton("Valider"));
    boost::shared_ptr<QPushButton> annuler (new QPushButton("Annuler"));
    layoutGrille->addWidget(valider.get(),4,0,1,2);
    layoutGrille->addWidget(annuler.get(),4,2,1,2);
    box->setLayout(layoutGrille.get());
    boost::shared_ptr<QVBoxLayout> layoutP (new QVBoxLayout);
    layoutP->addWidget(box.get());
    setLayout(layoutP.get());
    connect(valider.get(),SIGNAL(clicked()),this,SLOT(valider()));
    connect(annuler.get(),SIGNAL(clicked()),this,SLOT(annuler()));
}

/**********  SLOT annuler : fermeture de la fenêtre **********/
void FenetreInfos::annuler()
{
    close();
}

FenetreInfos::~FenetreInfos()
{
    //delete nomNoir; delete nomBlanc; delete niveauNoir; delete niveauBlanc; delete datePartie;
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



