#include "fenetreInfos.h"
#include "../Partie/PartieManager.h"

using namespace std;

/******************************************************************************************************************/
/********************************************                     *************************************************/
/******************************************** CLASSE FENETREINFOS *************************************************/
/********************************************                     *************************************************/
/******************************************************************************************************************/

/********** Constructeur **********************************************************/
FenetreInfos::FenetreInfos(FenetrePrincipale* f) : QWidget(), fenPrincipale(f)
{
    /*** D�finition d'un layout pour positionner les infos � demander */
    QGridLayout* layoutGrille = new QGridLayout();
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
    QPushButton* annuler  = new QPushButton("Annuler");
    layoutGrille->addWidget(valider,4,0,1,2);
    layoutGrille->addWidget(annuler,4,2,1,2);
    box->setLayout(layoutGrille);
    QVBoxLayout* layoutP = new QVBoxLayout();
    layoutP->addWidget(box);
    setLayout(layoutP);
    connect(valider,SIGNAL(clicked()),this,SLOT(valider()));
    connect(annuler,SIGNAL(clicked()),this,SLOT(annuler()));
}

/**********  SLOT annuler : fermeture de la fen�tre **********/
void FenetreInfos::annuler()
{
    close();
}

FenetreInfos::~FenetreInfos()
{
    //delete nomNoir; delete nomBlanc; delete niveauNoir; delete niveauBlanc; delete datePartie;
}

/******** SLOT valider : v�rification des infos entr�es et cr�ation d'une instance de Partie ********/
void FenetreInfos::valider()
{
    QString nom_noir, nom_Blanc, niveau_Noir, niveau_Blanc, date_partie;
    if (nomNoir->text().size()==0) nom_noir="Noir"; else nom_noir=nomNoir->text();
    if (nomBlanc->text().size()==0) nom_Blanc="Blanc"; else nom_Blanc=nomBlanc->text();
    if (niveauNoir->text().size()==0) niveau_Noir="-"; else niveau_Noir=niveauNoir->text();
    if (niveauBlanc->text().size()==0) niveau_Blanc="-"; else niveau_Blanc=niveauBlanc->text();
    if (datePartie->text().size()==0) date_partie="inconnue"; else date_partie=datePartie->text();

    fenPrincipale->getGoban()->getPartie()->init(nom_noir,nom_Blanc,niveau_Noir,niveau_Blanc,date_partie);
    //fenPrincipale->setPartie(partie::donneInstance(nom_noir,nom_Blanc,niveau_Noir,niveau_Blanc,date_partie));
    fenPrincipale->getInfosNoir()->setJoueur(fenPrincipale->getGoban()->getPartie()->getNoir());
    fenPrincipale->getInfosBlanc()->setJoueur(fenPrincipale->getGoban()->getPartie()->getBlanc());
    cout << "Partie initialis�e\n";
    close();
}



