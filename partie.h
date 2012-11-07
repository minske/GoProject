#ifndef PARTIE_H_INCLUDED
#define PARTIE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "joueur.h"

using namespace std;

class Coup
{
    int abscisse;
    int ordonnee;
    boost::shared_ptr<Joueur> j;
    int numero;
    QString commentaires;


public :
    Coup() : abscisse(0), ordonnee(0), numero(0), commentaires(QString()) {}
    Coup(int abs, int ord, boost::shared_ptr<Joueur> jou) : abscisse(abs), ordonnee(ord), j(jou) {}
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

    ~Coup();
};


class partie
{
public :
    typedef vector<Coup>::const_iterator iterateur;

    static boost::shared_ptr<partie> donneInstance();
    static boost::shared_ptr<partie> donneInstance(QString const& noirNom, QString const& blancNom, QString const& noirNiveau,
                                 QString const& blancNiveau, QString const& partieDate);
    void chargerFichier(std::string const& nomFichier);
    //void avancer() {++courant;}
    //void reculer() {--courant;}
    vector<Coup> getListeCoups() const {return listeCoups;}
    void ajouterCoup(Coup const& c);

    /*class iterateur
    {
        const boost::shared_ptr<Coup> ptr;

        public :
            iterateur(boost::shared_ptr<Coup> c) : ptr(c) {}
            iterateur(Coup const& c) : ptr(&c) {}
            const boost::shared_ptr<Coup> getPtr() const {return ptr;}
            void operator++() {++(ptr.get());}
            void operator--() {--(ptr.get());}
            bool operator!=(iterateur const& i) const {return i.ptr!=ptr;}
            bool operator==(iterateur const& i) const {return i.ptr==ptr;}
            const Coup& operator*() const {return ptr.get();}
    };

    iterateur dernierCoup() const {return iterateur(listeCoups.back());}
    iterateur fin() const { iterateur it(listeCoups.back()); ++it; return it; }
    iterateur debut() const { return iterateur(listeCoups.front()); }*/
    vector<Coup>::const_iterator debut() const {return listeCoups.begin();}
    vector<Coup>::const_iterator fin() const {return listeCoups.end();}
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
