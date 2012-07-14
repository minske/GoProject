#include "partie.h"
using namespace std;


partie* partie::instanceUnique = 0;

coup_exception::coup_exception(const std::string& i) throw():info(i){}

const char* coup_exception::what() const throw() { return info.c_str(); }


coup::coup(std::string const& s)
{
    numero = 0;
    // un coup donné dans le fichier est normalement de la forme B[mc]BL[151.84]OB[11]C[commentaires ...]
    /*
    On cherche à extraire :
    l'abscisse et l'ordonnée du coup
    on ne s'occupe pas du temps et des commentaires pour le moment
    */
    if (s.size() < 5) throw coup_exception("Fichier invalide !\n");
    //les abscisses et ordonnées vont de 0 à 18 en commençant en haut à gauche
    if (s[2]=='k') commentaires="HA HA HA HA HA";
    abscisse = s[2]-'a';
    ordonnee = s[3]-'a';
    j = 0;
}

string coup::print() const
{
    stringstream r;
    r << "Coup n°" << numero <<j->couleur().toStdString() << " : " << abscisse+1 << "-" << ordonnee+1;
    return r.str();
}

coup::~coup(){}

void partie::chargerFichier(string const& f)
{
    int numero = 1;
    /* Ouverture du fichier et test */
    ifstream sgf(f.c_str(),ios::in);
    if (sgf) //si l'ouverture a réussi
    {
        /* Le début du fichier contient des informations sur la partie
        La liste des coups commence au deuxième ';'
        On va récupérer toutes ces infos dans une chaîne pour les traiter ensuite */
        string contenu;
        string ligne;
        while (getline(sgf,ligne)) contenu += ligne;

        //contenu = contenu du fichier

        /********************* AURELIEN -> LA RECUP DES INFOS SUR LA PARTIE COMMENCE ICI ***************************/
        unsigned int i = 2;
        while (contenu[i] != ';') i++; // on avance jusqu'à ce qu'on trouve un ';'
        //i = position avant le premier coup
        string infos = contenu.substr(2,i-1);
        unsigned int j=0;
        string jblanc, jnoir, nblanc, nnoir, komi, Date, Resultat;

        while (j<infos.size()-1)
        {
            /*recherche des infos suivantes : PW[NomBlanc], PB[NomNoir],
              WR[NiveauBlanc], BR[NiveauNoir], KM[Komi]
             */
            string inf;

            while (infos[j]!=']')
            {
                inf += infos[j];
                j++;
            }
            if (inf.substr(0,2)=="PW") jblanc=inf.substr(3);
            else if (inf.substr(0,2)=="PB") jnoir=inf.substr(3);
            else if (inf.substr(0,2)=="WR") nblanc=inf.substr(3);
            else if (inf.substr(0,2)=="BR") nnoir=inf.substr(3);
            else if (inf.substr(0,2)=="DT") date=QString::fromStdString(inf.substr(3));
            else if (inf.substr(0,2)=="RE") resultat=QString::fromStdString(inf.substr(3));


            j++;
        }

        //Initialisation des joueurs
        joueurNoir = Noir::donneInstance(QString::fromStdString(jnoir),QString::fromStdString(nnoir));
        joueurBlanc = Blanc::donneInstance(QString::fromStdString(jblanc),QString::fromStdString(nblanc));

        /***  init en attendant que les infos de la partie fonctionnent  ***/
        //joueurNoir = Noir::donneInstance("Noir","NiveauNoir");
        //joueurBlanc = Blanc::donneInstance("Blanc","NiveauBlanc");

        i++;
        //while(contenu[i]!='\0')
        while((i<contenu.size()) && ( contenu[i]!=')'))
        {
            /* On va lire la liste des coups un par un
            On lit jusqu'à ce qu'on trouve un ; */
            string coup;
            while ((contenu[i]!=';') && (contenu[i]!=')'))
            {
                coup+=contenu[i];
                i++;
            }
            /* Si longueur(coup) = 0 -> erreur ... */
            if (coup.size()==0) throw coup_exception("Problème de lecture du coup\n");
            if (coup[2]==']') break;
            /* Sinon, on crée un objet de type coup et on l'ajoute à la liste des coups */
            //cout << coup << endl;
            listeCoups.push_back(coup);
            if (coup[0]=='B')
                listeCoups.back().setJoueur(joueurNoir);
            else listeCoups.back().setJoueur(joueurBlanc);
            listeCoups.back().setNum(numero);
            numero++;
            cout << listeCoups.back().print() << endl;
            if (contenu[i]==')') break;
            i++;
        }

        courant = debut();
    }
    else cout << "Ce fichier n'existe pas.\n";
}

partie* partie::donneInstance()
{
    if (instanceUnique == 0)
    {
        instanceUnique = new partie();
    }
    return instanceUnique;
}


ostream& operator<<(ostream& f, coup const& c)
{
    f << c.print();
    return f;
}


std::string partie::infos() const
{
    std::stringstream r;
    r << "Noir : " << joueurNoir->getNom().toStdString() << " - " << joueurNoir->getRank().toStdString() << std::endl;
    r << "Blanc : " << joueurBlanc->getNom().toStdString() << " - " << joueurBlanc->getRank().toStdString() << std::endl;
    return r.str();
}

void partie::libereInstance()
{
    instanceUnique=0;
}

partie::~partie()
{
    delete joueurBlanc; delete joueurNoir; partie::libereInstance();
}
