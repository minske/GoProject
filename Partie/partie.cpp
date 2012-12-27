#include "Partie.h"
#include "../Tools/debug.h"
#include "../Tools/CoupException.h"

using namespace std;


/***** Chargement d'un SGF *****/
void Partie::chargerFichier(string const& f)
{
    SGF::Debug* dbg = SGF::Debug::getInstance();
    dbg->add(SGF::Normal,"Chargement de la Partie. \n");
    int numero = 1; //sert � num�roter les coups au fur et � mesure qu'on lit le fichier

    /********** Ouverture du fichier et test **************/
    ifstream sgf(f.c_str(),ios::in);
    if (sgf) //si l'ouverture a r�ussi
    {
        /* Le d�but du fichier contient des informations sur la Partie
        La liste des coups commence au deuxi�me ';'
        On va r�cup�rer toutes ces infos dans une cha�ne pour les traiter ensuite */
        string contenu, ligne;

        while (getline(sgf,ligne))
        {
            contenu += ligne;
            contenu += '&'; //on ajoute un caract�re sp�cial � la fin de chaque ligne
        }

        /********** R�cup�ration des infos sur la Partie **************/
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

            /* L'erreur "string subscript out of range" �tait provoqu�e par cette ligne : en rajoutant -1, le probl�me
              a disparu. Dans la premi�re boucle while on ne va que jusque infos.size()-1, donc ici on ne pouvait pas
              aller jusque infos.size() ? En tout cas, probl�me � priori r�solu ...*/
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
                else if (inf.substr(0,2)=="DT") m_date=QString::fromStdString(inf.substr(3));
                else if (inf.substr(0,2)=="RE") m_resultat=QString::fromStdString(inf.substr(3));
            }

            j++;
        }

        //Initialisation des joueurs
//        m_joueurNoir = Noir::donneInstance(QString::fromStdString(jnoir),QString::fromStdString(nnoir));
//        m_joueurBlanc = Blanc::donneInstance(QString::fromStdString(jblanc),QString::fromStdString(nblanc));
        m_joueurNoir.reset(new Joueur(QString::fromStdString(jnoir),QString::fromStdString(nnoir),"noir"));
        m_joueurBlanc.reset(new Joueur(QString::fromStdString(jblanc),QString::fromStdString(nblanc),"blanc"));
        dbg->add(SGF::Normal,"Joueur Noir : "+jnoir+" "+nnoir);
        dbg->add(SGF::Normal,"Joueur Blanc : "+jblanc+" "+nblanc+"\n");

        /***  init en attendant que les infos de la Partie fonctionnent  ***/
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
            m_coups.push_back(Coup(coup, comCom));
            if (coup[0]=='B')
                m_coups.back().setJoueur(m_joueurNoir);
            else m_coups.back().setJoueur(m_joueurBlanc);
            m_coups.back().setNum(numero);
            numero++;
            dbg->add(SGF::Normal,m_coups.back().print());
            if (contenu[i]==')') break;
            if (contenu[i]=='&') i++;
            if (contenu[i]!=';') i++;
            i++;
        }

        //courant = debut();
    }
    else dbg->add(SGF::Exception,"Ce fichier n'existe pas.\n");
}

void Partie::init(QString const& noirNom, QString const& blancNom, QString const& noirNiveau, QString const& blancNiveau, QString const& partieDate)
{

//    m_joueurNoir = Noir::donneInstance(noirNom,noirNiveau);
//    m_joueurBlanc = Blanc::donneInstance(blancNom,blancNiveau);
    m_joueurNoir.reset(new Joueur(noirNom,noirNiveau,"noir"));
    m_joueurBlanc.reset(new Joueur(blancNom,blancNiveau,"blanc"));
    m_date = partieDate;

}


ostream& operator<<(ostream& f, Coup const& c)
{
    f << c.print();
    return f;
}


std::string Partie::infos() const
{
    std::stringstream r;
    r << "Noir : " << m_joueurNoir->getNom().toStdString() << " - " << m_joueurNoir->getRank().toStdString() << std::endl;
    r << "Blanc : " << m_joueurBlanc->getNom().toStdString() << " - " << m_joueurBlanc->getRank().toStdString() << std::endl;
    return r.str();
}


Partie::~Partie()
{
    //delete joueurBlanc; delete joueurNoir; Partie::libereInstance();
}

void Partie::enregistrerFichier(QString nomFich)
{
    std::ostringstream  os;
    /* Ecriture des informations en d�but de Partie */
    os << "(;" << std::endl << "PB[" << m_joueurNoir->getNom().toStdString() << "]" << std::endl;
    os << "BR[" << m_joueurNoir->getRank().toStdString() << "]" << std::endl;
    os << "PW[" << m_joueurBlanc->getNom().toStdString() << "]" << std::endl;
    os << "WR[" << m_joueurBlanc->getRank().toStdString() << "]" << std::endl;

    /* Ecriture de la liste des coups */
    if (!m_coups.empty())
    {
        for (std::vector<Coup>::iterator it = m_coups.begin(); it!=m_coups.end(); ++it)
        {
            /* Pour chaque coup, si joueur=Noir, on �crit B[coup], sinon W
            L'abscisse et l'ordonn�e doivent �tre converties en caract�res : a-a pour 0-0 */
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

    /* Parenth�se qui ferme le SGF */
    os << ";)";
    /* ouverture du fichier en �criture, effac� s'il existait d�j�*/
    ofstream fichier(nomFich.toStdString(), ios::out | ios::trunc);

    if (fichier) //si l'ouverture a r�ussi
    {
        fichier << os.str();
        fichier.close();
    }

}

void Partie::ajouterCoup(Coup const& c)
{
    m_coups.push_back(c);
}
