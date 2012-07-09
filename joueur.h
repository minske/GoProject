#ifndef JOUEUR_H
#define JOUEUR_H
#include <QtGui>

class Joueur
{
protected :
    unsigned int territoire;
    QWidget* infos;
    QLabel* nom;
    QLabel* niveau;
    QLabel* pierresCapturees;

public :
    Joueur(QString const& Nom, QString const& rank);
    QString getNom() const {return nom->text();}
    QString getRank() const {return niveau->text();}
    unsigned int getCapt() const {return pierresCapturees->text().toInt();}
    unsigned int getTerr() const {return territoire;}
    void addCapt(unsigned int i);
    virtual QString couleur() const=0;
    virtual QWidget* getInfos() const;
    virtual ~Joueur();
};

class Noir : public Joueur
{
private :
    unsigned int handicap;
    static Noir* instanceUnique;
    Noir(QString const& Nom, QString const& rank, unsigned int h);

public :
    unsigned int getHandi() const {return handicap;}
    static Noir* donneInstance(QString const& Nom, QString const& rank, unsigned int h=0);
    static void libereInstance();
    QString couleur() const {return "Noir";}
    ~Noir();
};

class Blanc : public Joueur
{
private :
    double komi;
    static Blanc* instanceUnique;
    Blanc(QString const& Nom, QString const& rank, double k);

public :
    double getKomi() const {return komi;}
    static Blanc* donneInstance(QString const& Nom, QString const& rank, double k=6.5);
    static void libereInstance();
    QString couleur() const {return "Blanc";}
    ~Blanc();
};


#endif // JOUEUR_H
