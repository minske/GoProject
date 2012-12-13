#ifndef JOUEUR_H
#define JOUEUR_H
#include <QtGui>

#include <boost/shared_ptr.hpp>

class Joueur
{
protected :
    unsigned int territoire;
    QString nom;
    QString niveau;
    unsigned int pierresCapturees;

public :
    Joueur(QString const& Nom, QString const& rank);
    QString getNom() const {return nom;}
    QString getRank() const {return niveau;}
    unsigned int getCapt() const {return pierresCapturees;}
    unsigned int getTerr() const {return territoire;}
    void addCapt(unsigned int i);
    virtual QString couleur() const=0;
    virtual ~Joueur();
    void setCapt(unsigned int i) {pierresCapturees=i;}
};

class Noir : public Joueur
{
private :
    unsigned int handicap;
    static boost::shared_ptr<Noir> instanceUnique;
    Noir(QString const& Nom, QString const& rank, unsigned int h);

public :
    unsigned int getHandi() const {return handicap;}
    static boost::shared_ptr<Noir> donneInstance(QString const& Nom, QString const& rank, unsigned int h=0);
    static void libereInstance();
    QString couleur() const {return "Noir";}
    ~Noir();
};

class Blanc : public Joueur
{
private :
    double komi;
    static boost::shared_ptr<Blanc> instanceUnique;
    Blanc(QString const& Nom, QString const& rank, double k);

public :
    double getKomi() const {return komi;}
    static boost::shared_ptr<Blanc> donneInstance(QString const& Nom, QString const& rank, double k=6.5);
    static void libereInstance();
    QString couleur() const {return "Blanc";}
    ~Blanc();
};


#endif // JOUEUR_H