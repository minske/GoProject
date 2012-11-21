#ifndef FORM_H
#define FORM_H

#include <QApplication>
#include <QtGui>
#include "goban.h"
#include "bouton.h"
#include "fenetreInfos.h"
#include <stack>
#include "debug.h"

#include "boost/enable_shared_from_this.hpp"

/***
Classe infosJoueurs : permet d'afficher les infos sur un joueur (blanc ou noir) telles que
son nom, son niveau, le nombre de pierres qu'il a captur�es, dans un GridLayout.

Classe MyApplication : h�rite de QApplication, permet de capter les exceptions lanc�es dans le
programme (sinon, ne peut capter que les QExceptions)

Classe FP = fen�tre principale, h�rite de QMainWindow, affiche le goban, les boutons, les infos
sur les joueurs, la barre de menus, etc.
***/

enum modeSGF { creationSGF, lectureSGF};
class infosJoueurs;

class MyApplication : public QApplication
{
public:
  MyApplication(int& argc, char ** argv) :
    QApplication(argc, argv) { }
  virtual ~MyApplication() { }

  // reimplemented from QApplication so we can throw exceptions in slots
  virtual bool notify(QObject * receiver, QEvent * event)
  {
    try { return QApplication::notify(receiver, event);}
    catch(std::exception& e)
    {
        SGF::Debug::getInstance()->add(SGF::Exception,e.what());
        qCritical() << "Exception thrown:" << e.what();
    }
    return false;
  }
};

class FP : public QMainWindow, public boost::enable_shared_from_this<FP>
{
    Q_OBJECT
private :
    modeSGF mode;
    //boost::shared_ptr<partie> Partie;
    QHBoxLayout* infosJoueur;
    infosJoueurs* infosNoir;
    infosJoueurs* infosBlanc;
    QHBoxLayout* layoutPrincipal;
    QVBoxLayout* layoutV;
    QVBoxLayout* layoutBoutons;
    QGraphicsView* vue;
    QVBoxLayout* widgetsCote;
    QTextEdit* commentaires;
    QUndoStack* pileUndo;
    QUndoView* viewUndo;
    QStatusBar* barreEtat;
    QLabel* nomFichier;
    QGridLayout* grilleBoutonsGoban;
    QTextEdit* messagesExecution;

    static double ECART;

public :
    FP();
    ~FP();
    boost::shared_ptr<FP> f()
   {
       return shared_from_this();
   }

    static double ECART_T() {return ECART;}

    QUndoCommand* nextaction();
    QGraphicsView* getVue() const {return vue;}
    void setVue(QGraphicsView* v) {vue = v;}
    //boost::shared_ptr<partie> getPartie() const {return Partie;}
    //void setPartie(boost::shared_ptr<partie> p) {Partie = p;}
    infosJoueurs* getInfosNoir() const {return infosNoir;}
    infosJoueurs* getInfosBlanc() const {return infosBlanc;}
    QTextEdit* getComm() const {return commentaires;}


public slots :
    void ouvrirFichier();
    void enregistrerFichier();
    void nouveauFichier();
    void nextMove();
    void prevMove();
    void next5Moves();
    void prev5Moves();
    void debutPartie();
    void finPartie();
    void fermerFichier();
    void changerFondFonce();
    void changerFondClair();
    void changerFondMoyen();
    void changerFondSansMotif();
    void bouton_goban(int a, int o);
    void afficherMsgExec();
};

class infosJoueurs : public QGridLayout
{
    QLabel* titre;
    QLabel* nom;
    QLabel* niveau;
    QLabel* pierresCapturees;
    boost::shared_ptr<Joueur> j;

public :
    infosJoueurs();
    void setTitre(QString const& t) {titre->setText(t);}
    void setTitre(QLabel* t) {titre=t;}
    void setNom(QString const& n) {nom->setText(n);}
    void setNom(QLabel* n) {nom=n;}
    void setNiveau(QString const& n) {niveau->setText(n);}
    void setNiveau(QLabel* n) {niveau=n;}
    void setCapt(QString const& c) {pierresCapturees->setText(c);}
    void setCapt(QLabel* c) {pierresCapturees=c;}
    void setJoueur(boost::shared_ptr<Joueur> J);
};

#endif // FORM_H
