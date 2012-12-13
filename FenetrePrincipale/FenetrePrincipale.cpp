#include "FenetrePrincipale.h"
#include "fenetreInfos.h"
#include "../Partie/partie.h"
#include "actionNext.h"
#include "../Tools/debug.h"

using namespace std;


FenetrePrincipale::FenetrePrincipale() : QMainWindow()
{
}




FenetrePrincipale::~FenetrePrincipale()
{
}



void FenetrePrincipale::changerFondClair()
{
    m_goban->setBackgroundBrush(m_goban->getBrushClair());
}

void FenetrePrincipale::changerFondFonce()
{
    m_goban->setBackgroundBrush(m_goban->getBrushFonce());
}

void FenetrePrincipale::changerFondMoyen()
{
    m_goban->setBackgroundBrush(m_goban->getBrushMoyen());
}

void FenetrePrincipale::changerFondSansMotif()
{
   m_goban->setBackgroundBrush(m_goban->getBrushSansMotif());
}




void FenetrePrincipale::bouton_goban(int a, int o)
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

void FenetrePrincipale::afficherMsgExec()
{
    messagesExecution->show();
}
