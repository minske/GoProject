#ifndef FORM_H
#define FORM_H

#include <QApplication>
#include <QtGui>
#include "goban.h"

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
    QWidget* infosJoueurs;

public :
    FP();

public slots :
    void ouvrirFichier();
    void nextMove();
    //void prevMove();
};

#endif // FORM_H
