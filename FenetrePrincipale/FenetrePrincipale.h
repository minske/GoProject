#ifndef FORM_H
#define FORM_H

#include <QApplication>
#include <QtGui>
#include <stack>
#include <boost/enable_shared_from_this.hpp>


#include "../Tools/Debug.h"
#include "../GobanFiles/Goban.h"
#include "BoutonGoban.h"
#include "InfosJoueur.h"



/***
Classe MyApplication : hérite de QApplication, permet de capter les exceptions lancées dans le
programme (sinon, ne peut capter que les QExceptions)

Classe FP = fenêtre principale, hérite de QMainWindow, affiche le goban, les boutons, les infos
sur les joueurs, la barre de menus, etc.
***/

//enum modeSGF { creationSGF, lectureSGF};

class FenetrePrincipale : public QMainWindow, public boost::enable_shared_from_this<FenetrePrincipale>
{
    Q_OBJECT
protected :
    boost::shared_ptr<Goban> m_goban;

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

    //menus
    QMenu* menuFichier;
    QMenu* menuOptions;
    QMenu* changerFond;
    QToolBar* barreOutils;

    //boutons
    QHBoxLayout* layoutBoutonsNP;

    double ECART;

public :
    FenetrePrincipale();
    ~FenetrePrincipale();

    double ECART_T() {return ECART;}

    QUndoCommand* nextaction();
    QGraphicsView* getVue() const {return vue;}
    void setVue(QGraphicsView* v) {vue = v;}
    //boost::shared_ptr<partie> getPartie() const {return Partie;}
    //void setPartie(boost::shared_ptr<partie> p) {Partie = p;}
    infosJoueurs* getInfosNoir() const {return infosNoir;}
    infosJoueurs* getInfosBlanc() const {return infosBlanc;}
    QTextEdit* getComm() const {return commentaires;}

    boost::shared_ptr<Goban> getGoban() {return m_goban;}


public slots :

    void changerFondFonce();
    void changerFondClair();
    void changerFondMoyen();
    void changerFondSansMotif();
    void bouton_goban(int a, int o);
    void afficherMsgExec();
};

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

#endif // FORM_H
