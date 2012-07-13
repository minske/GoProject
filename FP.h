#ifndef FORM_H
#define FORM_H

#include <QApplication>
#include <QtGui>
#include "goban.h"
#include <stack>

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

public :
    FP();
    ~FP();
    QGraphicsView* getVue() const {return vue;}
    void setVue(QGraphicsView* v) {vue = v;}

public slots :
    void ouvrirFichier();
    void nextMove();
    void prevMove();
    void fermerFichier();
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
