#ifndef FORM_H
#define FORM_H

#include <QApplication>
#include <QtGui>
#include "goban.h"
#include "bouton.h"
#include "fenetreInfos.h"
#include <stack>

/***
Classe infosJoueurs : permet d'afficher les infos sur un joueur (blanc ou noir) telles que
son nom, son niveau, le nombre de pierres qu'il a capturées, dans un GridLayout.

Classe MyApplication : hérite de QApplication, permet de capter les exceptions lancées dans le
programme (sinon, ne peut capter que les QExceptions)

Classe FP = fenêtre principale, hérite de QMainWindow, affiche le goban, les boutons, les infos
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
        qCritical() << "Exception thrown:" << e.what();
    }
    return false;
  }
};

class FP : public QMainWindow
{
    Q_OBJECT
private :
    modeSGF mode;
    Goban* goban;
    partie* Partie;
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
    QStatusBar *barreEtat;
    QLabel* nomFichier;
    QGridLayout* grilleBoutonsGoban;
    QTextEdit* messagesExecution;

public :
    FP();
    ~FP();
    QUndoCommand* nextaction();
    QGraphicsView* getVue() const {return vue;}
    void setVue(QGraphicsView* v) {vue = v;}
    Goban* getGoban() const {return goban;}
    partie* getPartie() const {return Partie;}
    void setPartie(partie* p) {Partie = p;}
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
    Joueur* j;

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
    void setJoueur(Joueur* J);
};

#endif // FORM_H
