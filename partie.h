#ifndef PARTIE_H_INCLUDED
#define PARTIE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "joueur.h"
#include <boost/enable_shared_from_this.hpp>


using namespace std;

class Coup : public boost::enable_shared_from_this<Coup>
{
    int abscisse;
    int ordonnee;
    boost::shared_ptr<Joueur> j;
    int numero;
    QString commentaires;


public :
    Coup() : abscisse(0), ordonnee(0), numero(0), commentaires(QString()) {}
    Coup(int abs, int ord, boost::shared_ptr<Joueur> jou) : abscisse(abs), ordonnee(ord), j(jou)
    {std::cout << "coup créé\n";}
    Coup(std::string const& s, std::string com=std::string());
    std::string print() const;
    int getAbs() const {return abscisse;}
    int getOrd() const {return ordonnee;}
    boost::shared_ptr<Joueur> getJoueur() const {return j;}
    void setJoueur(boost::shared_ptr<Joueur> J) {j = J;}
    void setNum(int n) {numero = n;}
    int getNum() const {return numero;}
    QString getComm() const {return commentaires;}
    void addComm(QString const& s);

    std::string couleur() const {return j->couleur().toStdString();}

    ~Coup();
};


class partie
{
public :
    typedef vector<Coup>::iterator iterateur;

    static boost::shared_ptr<partie> instance();
    static boost::shared_ptr<partie> instance(QString const& noirNom, QString const& blancNom, QString const& noirNiveau,
                                 QString const& blancNiveau, QString const& partieDate);
    void chargerFichier(std::string const& nomFichier);
    //void avancer() {++courant;}
    //void reculer() {--courant;}
    vector<Coup> getListeCoups() const {return listeCoups;}
    void ajouterCoup(Coup const& c);


    iterateur debut() {return listeCoups.begin();}
    iterateur fin() {return listeCoups.end();}
    boost::shared_ptr<Blanc> getBlanc() const {return joueurBlanc;}
    boost::shared_ptr<Noir> getNoir() const {return joueurNoir;}
    std::string infos() const;
    ~partie();
    static void libereInstance();
    QString getDate() const {return date;}
    QString getResultat() const {return resultat;}
    /*std::string getContenuFichier() const {return contenuFichier;}
    void setContenuFichier(std::string const& c) {contenuFichier=c;}*/
    void enregistrerFichier(QString nomFich);

private :
    vector<Coup> listeCoups;
    static boost::shared_ptr<partie> instanceUnique;
    partie() : date(QString()), resultat(QString()), contenuFichier(std::string())  {}
    boost::shared_ptr<Blanc> joueurBlanc;
    boost::shared_ptr<Noir> joueurNoir;
    QString date;
    QString resultat;
    std::string contenuFichier;
};

ostream& operator<<(ostream& f, Coup const& c);

class coup_exception : public exception {
	std::string info;
public:
	coup_exception(const std::string& i) throw();
	const char* what() const throw();
	virtual ~coup_exception() throw() {}
};

#endif // PARTIE_H_INCLUDED
