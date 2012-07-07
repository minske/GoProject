#ifndef PARTIE_H_INCLUDED
#define PARTIE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "joueur.h"

using namespace std;

class coup
{
    int abscisse;
    int ordonnee;
    Joueur* j;

public :
    coup() : abscisse(0), ordonnee(0), j(0) {}
    coup(std::string const& s);
    std::string print() const;
    int getAbs() const {return abscisse;}
    int getOrd() const {return ordonnee;}
    Joueur* getJoueur() const {return j;}
    void setJoueur(Joueur* J) {j = J;}
};


class partie
{
public :
    static partie* donneInstance();
    void chargerFichier(std::string const& nomFichier);
    void avancer() {++courant;}
    void reculer() {--courant;}
    vector<coup> getListeCoups() const {return listeCoups;}

    class iterateur
    {
        const coup* ptr;

        public :
            iterateur(coup* c) : ptr(c) {}
            iterateur(coup const& c) : ptr(&c) {}
            const coup* getPtr() const {return ptr;}
            void operator++() {++ptr;}
            void operator--() {--ptr;}
            bool operator!=(iterateur const& i) const {return i.ptr!=ptr;}
            bool operator==(iterateur const& i) const {return i.ptr==ptr;}
            const coup& operator*() const {return *ptr;}
    };

    iterateur dernierCoup() const {return iterateur(listeCoups.back());}
    iterateur fin() const { iterateur it(listeCoups.back()); ++it; return it; }
    iterateur debut() const { return iterateur(listeCoups.front()); }
    iterateur getCourant() const {return courant;}

    Blanc* getBlanc() const {return joueurBlanc;}
    Noir* getNoir() const {return joueurNoir;}
    std::string infos() const;

private :
    vector<coup> listeCoups;
    static partie* instanceUnique;
    partie::iterateur courant;
    partie() : courant(0), joueurBlanc(0), joueurNoir(0) {}
    Blanc* joueurBlanc;
    Noir* joueurNoir;

};

ostream& operator<<(ostream& f, coup const& c);

class coup_exception : public exception {
	std::string info;
public:
	coup_exception(const std::string& i) throw();
	const char* what() const throw();
	virtual ~coup_exception() throw() {}
};

#endif // PARTIE_H_INCLUDED
