#include "partie.h"
using namespace std;


partie* partie::instanceUnique = 0;

coup_exception::coup_exception(const std::string& i) throw():info(i){}

const char* coup_exception::what() const throw() { return info.c_str(); }

/******************* CLASSE COUP ***********************************************************************************/

Coup::Coup(std::string const& s, std::string com)
{
    numero = 0;
    // un coup donn� dans le fichier est normalement de la forme B[mc]BL[151.84]OB[11]C[commentaires ...]
    /*
    On cherche � extraire :
    l'abscisse et l'ordonn�e du coup
    on ne s'occupe pas du temps et des commentaires pour le moment
    */
    if (s.size() < 5) throw coup_exception("Fichier invalide !\n");
    //les abscisses et ordonn�es vont de 0 � 18 en commen�ant en haut � gauche

    int p=com.size();
    for (int n=0; n<p; n++)
    {
        if (com[n]=='&') com.replace(n,1,1,'\n');
    }
    commentaires=QString::fromStdString(com);
    abscisse = s[2]-'a';
    ordonnee = s[3]-'a';
    j = 0;
}

string Coup::print() const
{
    stringstream r;
    r << "Coup n�" << numero <<j->couleur().toStdString() << " : " << abscisse+1 << "-" << ordonnee+1;
    return r.str();
}

Coup::~Coup(){}




/***************************** CLASSE PARTIE **********************************************************************/

/***** Chargement d'un SGF *****/
void partie::chargerFichier(string const& f)
{
    int numero = 1; //sert � num�roter les coups au fur et � mesure qu'on lit le fichier

    /********** Ouverture du fichier et test **************/
    ifstream sgf(f.c_str(),ios::in);
    if (sgf) //si l'ouverture a r�ussi
    {
        /* Le d�but du fichier contient des informations sur la partie
        La liste des coups commence au deuxi�me ';'
        On va r�cup�rer toutes ces infos dans une cha�ne pour les traiter ensuite */
        string contenu, ligne;

        while (getline(sgf,ligne))
        {
            contenu += ligne;
            contenu += '&'; //on ajoute un caract�re sp�cial � la fin de chaque ligne
        }

        /********** R�cup�ration des infos sur la partie **************/
        unsigned int i = 2;
        while (contenu[i] != ';') i++; // on avance jusqu'� ce qu'on trouve un ';'
        //i = position avant le premier coup
        string infos = contenu.substr(2,i-1);
        unsigned int j=0;
        string jblanc, jnoir, nblanc, nnoir, komi;// Date, Resultat;

        while (j<infos.size()-1)
        {
            /*recherche des infos suivantes : PW[NomBlanc], PB[NomNoir],
              WR[NiveauBlanc], BR[NiveauNoir], KM[Komi]
             */
            string inf;

            while ((infos[j]!=']') && (j<infos.size()))
            {
                inf += infos[j];
                j++;
            }
            if (inf[0] == '&') inf.erase(0,1);
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
            On lit jusqu'� ce qu'on trouve un ; */
            string coup;
            int k=0;
            string comCom("z");
            while ((contenu[i]!=';') && (contenu[i]!=')') && (k!=2)) // on arrete si on trouve le bon nombre de crochet ferm�
            {
                coup+=contenu[i];
                if ((contenu[i]==']') && ((contenu[i+1]!='O') && (contenu[i+1]!='W') && (contenu[i+1]!='B' ))) k++; /* si on trouve un crochet ferm� non suivi d'un O
                                                                     (il resterait alors le nombre de byo restant � lire)
                                                                     on augmente k */
                if ((contenu[i+1]=='C') && (k==1) && (contenu[i+2]!='R')) k++;
                i++;
            }
            if (contenu[i]=='C') // si il y a un C, il y aura un commentaire
            {
                int l=1; // permet de savoir o� on en est des crochets
               i++; i++;
               while (l!=0)
               {
                   if (contenu[i]== '[') l++;
                   if (contenu[i]== ']') l--;
                   comCom.push_back(contenu[i]);
                   i++;
               }

               comCom.erase(comCom.size()-1,1);
            }
            /* Si longueur(coup) = 0 -> erreur ... */
            if (coup.size()==0) throw coup_exception("Probl�me de lecture du coup\n");
            if (coup[2]==']') break;
            /* Sinon, on cr�e un objet de type coup et on l'ajoute � la liste des coups */
            //cout << coup << endl;
            comCom.erase(0,1);
            listeCoups.push_back(Coup(coup, comCom));
            if (coup[0]=='B')
                listeCoups.back().setJoueur(joueurNoir);
            else listeCoups.back().setJoueur(joueurBlanc);
            listeCoups.back().setNum(numero);
            numero++;
            cout << listeCoups.back().print() << endl;
            if (contenu[i]==')') break;
            if (contenu[i]=='&') i++;
            if (contenu[i]!=';') i++;
            i++;
        }

        //courant = debut();
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


ostream& operator<<(ostream& f, Coup const& c)
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
