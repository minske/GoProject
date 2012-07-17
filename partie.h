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
    Joueur* j;
    int numero;
    QString commentaires;


public :
    Coup() : abscisse(0), ordonnee(0), j(0), numero(0), commentaires(QString()) {}
    Coup(std::string const& s, std::string com=std::string());
    std::string print() const;
    int getAbs() const {return abscisse;}
    int getOrd() const {return ordonnee;}
    Joueur* getJoueur() const {return j;}
    void setJoueur(Joueur* J) {j = J;}
    void setNum(int n) {numero = n;}
    int getNum() const {return numero;}
    QString getComm() const {return commentaires;}

    ~Coup();
};


class partie
{
public :
    static partie* donneInstance();
    void chargerFichier(std::string const& nomFichier);
    //void avancer() {++courant;}
    //void reculer() {--courant;}
    vector<Coup> getListeCoups() const {return listeCoups;}

    class iterateur
    {
        const Coup* ptr;

        public :
            iterateur(Coup* c) : ptr(c) {}
            iterateur(Coup const& c) : ptr(&c) {}
            const Coup* getPtr() const {return ptr;}
            void operator++() {++ptr;}
            void operator--() {--ptr;}
            bool operator!=(iterateur const& i) const {return i.ptr!=ptr;}
            bool operator==(iterateur const& i) const {return i.ptr==ptr;}
            const Coup& operator*() const {return *ptr;}
    };

    iterateur dernierCoup() const {return iterateur(listeCoups.back());}
    iterateur fin() const { iterateur it(listeCoups.back()); ++it; return it; }
    iterateur debut() const { return iterateur(listeCoups.front()); }
    //iterateur getCourant() const {return courant;}

    Blanc* getBlanc() const {return joueurBlanc;}
    Noir* getNoir() const {return joueurNoir;}
    std::string infos() const;
    ~partie();
    static void libereInstance();
    QString getDate() const {return date;}
    QString getResultat() const {return resultat;}

private :
    vector<Coup> listeCoups;
    static partie* instanceUnique;
//    partie::iterateur courant;
    partie() : /*courant(0),*/ joueurBlanc(0), joueurNoir(0), date(QString()), resultat(QString())  {}
    Blanc* joueurBlanc;
    Noir* joueurNoir;
    QString date;
    QString resultat;

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
