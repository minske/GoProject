#include "Pierre.h"
#include "../FenetrePrincipale/FenetrePrincipale.h"
#include "Goban.h"

using namespace std;

Pierre::Pierre(Coup c, int ecartGoban) : m_coup(c)
{
    cout << "création p pour le coup " << c.print() << endl;
    if (c.getJoueur()->getNom()=="Kiral")
    {
        cout << "joueur = kiral" << endl;
        ellipse = boost::shared_ptr<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap("Images/circle_rose.png").scaled(ecartGoban*R,ecartGoban*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    }
    else if (c.getJoueur()->couleur()=="Noir")
    {

        ellipse = boost::shared_ptr<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap("Images/pierreNoire.png").scaled(ecartGoban*R,ecartGoban*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    }

    else
    {
        unsigned int nb = rand() % 6 +1;
        std::ostringstream os;
        os << "Images/pierreBlanche" << nb << ".png";
        QString nomFichier = QString::fromStdString(os.str());
        ellipse = boost::shared_ptr<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap(nomFichier).scaled(ecartGoban*R,ecartGoban*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    }

    ellipse->setX((c.getAbs()+1)*ecartGoban-(ecartGoban*R/2));
    ellipse->setY((c.getOrd()+1)*ecartGoban-(ecartGoban*R/2));

    //rect.setWidth(E*0.9);
    //rect.setHeight(E*0.9);
    //les pierres font 100 pixels de diamètre, on veut qu'elles fassent E*0,9
    //ellipse->setScale(E*0.9/100);

    std::ostringstream name;
    name << rand() % 300 +1;
    m_name = name.str();

}


void Pierre::setEllipse(boost::shared_ptr<QGraphicsPixmapItem> el)
{
    ellipse = el;
}


int Pierre::libertes() const
{
    ostringstream os;
    os << "Calcul du nombre de libertés de la pierre " << m_coup.print();
    //SGF::Debug::getInstance()->add(SGF::Normal,os.str());
    vector<pair<int,int> > adj = intersectionsAdjacentes();

    ostringstream oss;
    oss << "Nombre d'intersections adjacentes : " << adj.size();
    //SGF::Debug::getInstance()->add(SGF::Normal,oss.str());
    std::map<std::pair<int,int>,boost::shared_ptr<Pierre> > plateau = m_groupe.lock()->getGoban()->getPlateau();

    int libertes = adj.size();
    for (std::vector<std::pair<int,int> >::iterator it = adj.begin(); it != adj.end(); it++)
    {
        if (plateau.find(*it)!=plateau.end())
        {
            libertes--;
        }
    }

    ostringstream libs;
    libs << "Nombre de libertés : " << libertes;
    //SGF::Debug::getInstance()->add(SGF::Normal,libs.str());
    return libertes;
}

vector<pair<int,int> > Pierre::intersectionsAdjacentes() const
{
    vector<pair<int, int> > resultat;
    int abs = getCoup().getAbs();
    int ord = getCoup().getOrd();

    resultat.push_back(pair<int,int>(abs,ord+1));
    resultat.push_back(pair<int,int>(abs,ord-1));
    resultat.push_back(pair<int,int>(abs+1,ord));
    resultat.push_back(pair<int,int>(abs-1,ord));

    for (vector<pair<int,int> >::iterator it = resultat.begin(); it != resultat.end(); it++)
    {
        int a = it->first; int o = it->second;
        if (a<0 || a>=m_groupe.lock()->getGoban()->SIZE())
        {
            it = resultat.erase(it);
        }
        else if (o<0 || o>=m_groupe.lock()->getGoban()->SIZE())
        {
            it = resultat.erase(it);
        }
    }

    return resultat;
}

vector<boost::shared_ptr<Pierre> > Pierre::pierresAutourMemeCouleur() const
{
    vector<pair<int,int> > intersect = intersectionsAdjacentes();
    vector<boost::shared_ptr<Pierre> > result;
    boost::shared_ptr<Goban> g = m_groupe.lock()->getGoban();
    std::map<std::pair<int,int>,boost::shared_ptr<Pierre> > plateau = g->getPlateau();
    for (vector<pair<int,int> >::iterator it = intersect.begin(); it != intersect.end(); it++)
    {
        if (plateau.find(*it) != plateau.end())
        {
            boost::shared_ptr<Pierre> pierrePtr = plateau.find(*it)->second;
            if (pierrePtr->couleur() == this->couleur())
            {
                result.push_back(pierrePtr);
            }
        }
    }

    return result;
}


int Pierre::libertes(boost::shared_ptr<Goban> gobanPtr) const
{
    ostringstream os;
    os << "Calcul du nombre de libertés de la pierre " << m_coup.print();
    cout << os.str() << endl;
    //SGF::Debug::getInstance()->add(SGF::Normal,os.str());
    vector<pair<int,int> > adj = intersectionsAdjacentes(gobanPtr);

    ostringstream oss;
    oss << "Nombre d'intersections adjacentes : " << adj.size();
    //SGF::Debug::getInstance()->add(SGF::Normal,oss.str());
    cout << oss.str() << endl;
    int libertes = adj.size();
    std::map<std::pair<int,int>,boost::shared_ptr<Pierre> > plateau = gobanPtr->getPlateau();
    cout << "plateau" << endl;
    for (std::vector<std::pair<int,int> >::iterator it = adj.begin(); it != adj.end(); it++)
    {
        if (plateau.find(*it)!=plateau.end())
        {
            libertes--;
        }
    }

    ostringstream libs;
    cout << "Nombre de libertés : " << libertes;
    //SGF::Debug::getInstance()->add(SGF::Normal,libs.str());
    return libertes;
}

vector<pair<int,int> > Pierre::intersectionsAdjacentes(boost::shared_ptr<Goban> gobanPtr) const
{
    vector<pair<int, int> > resultat;
    int abs = getCoup().getAbs();
    int ord = getCoup().getOrd();

    resultat.push_back(pair<int,int>(abs,ord+1));
    resultat.push_back(pair<int,int>(abs,ord-1));
    resultat.push_back(pair<int,int>(abs+1,ord));
    resultat.push_back(pair<int,int>(abs-1,ord));

    for (vector<pair<int,int> >::iterator it = resultat.begin(); it != resultat.end(); it++)
    {
        int a = it->first; int o = it->second;
        if (a<0 || a>=gobanPtr->SIZE())
        {
            it = resultat.erase(it);
        }
        else if (o<0 || o>=gobanPtr->SIZE())
        {
            it = resultat.erase(it);
        }
    }

    return resultat;
}

vector<boost::shared_ptr<Pierre> > Pierre::pierresAutourMemeCouleur(boost::shared_ptr<Goban> gobanPtr) const
{
    vector<pair<int,int> > intersect = intersectionsAdjacentes(gobanPtr);
    vector<boost::shared_ptr<Pierre> > result;

    std::map<std::pair<int,int>,boost::shared_ptr<Pierre> > plateau = gobanPtr->getPlateau();

    for (vector<pair<int,int> >::iterator it = intersect.begin(); it != intersect.end(); it++)
    {
        if (plateau.find(*it) != plateau.end())
        {
            boost::shared_ptr<Pierre> pierrePtr = plateau.find(*it)->second;
            if (pierrePtr->couleur() == this->couleur())
            {
                result.push_back(pierrePtr);
            }
        }
    }

    return result;
}
