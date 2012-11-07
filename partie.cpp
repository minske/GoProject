#include "partie.h"
#include "debug.h"

using namespace std;


boost::shared_ptr<partie> partie::instanceUnique;// = boost::shared_ptr<partie>(0);

coup_exception::coup_exception(const std::string& i) throw():info(i){}

const char* coup_exception::what() const throw() { return info.c_str(); }

/******************* CLASSE COUP ***********************************************************************************/

Coup::Coup(std::string const& s, std::string com)
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

    int p=com.size();
    for (int n=0; n<p; n++)
    {
        if (com[n]=='&') com.replace(n,1,1,'\n');
    }
    commentaires=QString::fromStdString(com);
    abscisse = s[2]-'a';
    ordonnee = s[3]-'a';
    j.reset();
}

string Coup::print() const
{
    stringstream r;
    r << "Coup n°" << numero << " - " <<j->couleur().toStdString() << " : " << abscisse+1 << "-" << ordonnee+1;
    return r.str();
}

Coup::~Coup(){}

void Coup::addComm(QString const& s)
{
    commentaires.append(s);
}


/***************************** CLASSE PARTIE **********************************************************************/

/***** Chargement d'un SGF *****/
void partie::chargerFichier(string const& f)
{
    SGF::Debug* dbg = SGF::Debug::getInstance();
    dbg->add(SGF::Normal,"Chargement de la partie. \n");
    int numero = 1; //sert à numéroter les coups au fur et à mesure qu'on lit le fichier

    /********** Ouverture du fichier et test **************/
    ifstream sgf(f.c_str(),ios::in);
    if (sgf) //si l'ouverture a réussi
    {
        /* Le début du fichier contient des informations sur la partie
        La liste des coups commence au deuxième ';'
        On va récupérer toutes ces infos dans une chaîne pour les traiter ensuite */
        string contenu, ligne;

        while (getline(sgf,ligne))
        {
            contenu += ligne;
            contenu += '&'; //on ajoute un caractère spécial à la fin de chaque ligne
        }

        /********** Récupération des infos sur la partie **************/
        unsigned int i = 2;
        while (contenu[i] != ';') i++; // on avance jusqu'à ce qu'on trouve un ';'
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

            /* L'erreur "string subscript out of range" était provoquée par cette ligne : en rajoutant -1, le problème
              a disparu. Dans la première boucle while on ne va que jusque infos.size()-1, donc ici on ne pouvait pas
              aller jusque infos.size() ? En tout cas, problème à priori résolu ...*/
            while ((infos[j]!=']') && (j<infos.size()-1))
            {
                inf += infos[j];
                j++;
            }
            if (inf[0] == '&') inf.erase(0,1);
            if (!inf.empty())
            {
                if (inf.substr(0,2)=="PW") jblanc=inf.substr(3);
                else if (inf.substr(0,2)=="PB") jnoir=inf.substr(3);
                else if (inf.substr(0,2)=="WR") nblanc=inf.substr(3);
                else if (inf.substr(0,2)=="BR") nnoir=inf.substr(3);
                else if (inf.substr(0,2)=="DT") date=QString::fromStdString(inf.substr(3));
                else if (inf.substr(0,2)=="RE") resultat=QString::fromStdString(inf.substr(3));
            }

            j++;
        }

        //Initialisation des joueurs
        joueurNoir = Noir::donneInstance(QString::fromStdString(jnoir),QString::fromStdString(nnoir));
        joueurBlanc = Blanc::donneInstance(QString::fromStdString(jblanc),QString::fromStdString(nblanc));
        dbg->add(SGF::Normal,"Joueur Noir : "+jnoir+" "+nnoir);
        dbg->add(SGF::Normal,"Joueur Blanc : "+jblanc+" "+nblanc+"\n");

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
            int k=0;
            string comCom("z");
            while ((contenu[i]!=';') && (contenu[i]!=')') && (k!=2)) // on arrete si on trouve le bon nombre de crochet fermé
            {
                coup+=contenu[i];
                if ((contenu[i]==']') && ((contenu[i+1]!='O') && (contenu[i+1]!='W') && (contenu[i+1]!='B' ))) k++; /* si on trouve un crochet fermé non suivi d'un O
                                                                     (il resterait alors le nombre de byo restant à lire)
                                                                     on augmente k */
                if ((contenu[i+1]=='C') && (k==1) && (contenu[i+2]!='R')) k++;
                i++;
            }
            if (contenu[i]=='C') // si il y a un C, il y aura un commentaire
            {
                int l=1; // permet de savoir où on en est des crochets
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
            if (coup.size()==0) throw coup_exception("Problème de lecture du coup\n");
            if (coup[2]==']') break;
            /* Sinon, on crée un objet de type coup et on l'ajoute à la liste des coups */
            //cout << coup << endl;
            comCom.erase(0,1);
            listeCoups.push_back(Coup(coup, comCom));
            if (coup[0]=='B')
                listeCoups.back().setJoueur(joueurNoir);
            else listeCoups.back().setJoueur(joueurBlanc);
            listeCoups.back().setNum(numero);
            numero++;
            dbg->add(SGF::Normal,listeCoups.back().print());
            if (contenu[i]==')') break;
            if (contenu[i]=='&') i++;
            if (contenu[i]!=';') i++;
            i++;
        }

        //courant = debut();
    }
    else dbg->add(SGF::Exception,"Ce fichier n'existe pas.\n");
}

boost::shared_ptr<partie> partie::donneInstance()
{
    if (instanceUnique == 0)
    {
        instanceUnique = boost::shared_ptr<partie>(new partie());
    }
    return instanceUnique;
}

boost::shared_ptr<partie> partie::donneInstance(QString const& noirNom, QString const& blancNom, QString const& noirNiveau,
                             QString const& blancNiveau, QString const& partieDate)
{
    if (instanceUnique == 0)
    {
        instanceUnique = boost::shared_ptr<partie>(new partie());

        instanceUnique->joueurNoir = Noir::donneInstance(noirNom,noirNiveau);
        instanceUnique->joueurBlanc = Blanc::donneInstance(blancNom,blancNiveau);
        instanceUnique->date = partieDate;
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
    instanceUnique.reset();
}

partie::~partie()
{
    //delete joueurBlanc; delete joueurNoir; partie::libereInstance();
}

void partie::enregistrerFichier(QString nomFich)
{
    std::ostringstream  os;
    /* Ecriture des informations en début de partie */
    os << "(;" << std::endl << "PB[" << joueurNoir->getNom().toStdString() << "]" << std::endl;
    os << "BR[" << joueurNoir->getRank().toStdString() << "]" << std::endl;
    os << "PW[" << joueurBlanc->getNom().toStdString() << "]" << std::endl;
    os << "WR[" << joueurBlanc->getRank().toStdString() << "]" << std::endl;

    /* Ecriture de la liste des coups */
    if (!listeCoups.empty())
    {
        for (partie::iterateur it = debut(); it!=fin(); ++it)
        {
            /* Pour chaque coup, si joueur=Noir, on écrit B[coup], sinon W
            L'abscisse et l'ordonnée doivent être converties en caractères : a-a pour 0-0 */
            if (it->getJoueur()->couleur()=="Noir") os << ";B[" ;
            else os << ";W[";
            char abs('a' + it->getAbs()), ord('a' + it->getOrd());
            os << abs << ord << "]" << std::endl;

            if (!it->getComm().toStdString().empty())
            {
                //s'il y a des commentaires pour ce coup
                os << "C[" << it->getComm().toStdString() << "]" << std::endl;
            }
        }
    }

    /* Parenthèse qui ferme le SGF */
    os << ";)";
    /* ouverture du fichier en écriture, effacé s'il existait déjà*/
    ofstream fichier(nomFich.toStdString(), ios::out | ios::trunc);

    if (fichier) //si l'ouverture a réussi
    {
        fichier << os.str();
        fichier.close();
    }

}

void partie::ajouterCoup(Coup const& c)
{
    listeCoups.push_back(c);
}
