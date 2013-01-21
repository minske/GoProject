#include "Pierre.h"
#include "../FenetrePrincipale/FenetrePrincipale.h"
#include "Goban.h"

using namespace std;

Pierre::Pierre(Coup c, int ecartGoban) : m_coup(c)
{
    if (c.getJoueur()->getNom()=="Kiral")
        ellipse = boost::shared_ptr<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap("Images/circle_rose.png").scaled(ecartGoban*R,ecartGoban*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));

    else if (c.getJoueur()->couleur()=="noir")
        ellipse = boost::shared_ptr<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap("Images/pierreNoire.png").scaled(ecartGoban*R,ecartGoban*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));

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
    vector<pair<int,int> > adj = intersectionsAdjacentes();
    std::map<std::pair<int,int>,boost::shared_ptr<Pierre> > plateau = m_groupe.lock()->getGoban()->getPlateau();

    int libertes = adj.size();
    for (std::vector<std::pair<int,int> >::iterator it = adj.begin(); it != adj.end(); it++)
    {
        if (plateau.find(*it)!=plateau.end())
        {
            libertes--;
        }
    }

//    std::cout << "\nNombre de libertés : " << libertes << std::endl;
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

    for (vector<pair<int,int> >::iterator it = resultat.begin(); it != resultat.end();)
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
        else it++;
    }

    return resultat;
}

vector<pair<int,int> > Pierre::intersectionsAdjacentes(int gobanSize) const
{
    vector<pair<int, int> > resultat;
    int abs = getCoup().getAbs();
    int ord = getCoup().getOrd();

    resultat.push_back(pair<int,int>(abs,ord+1));
    resultat.push_back(pair<int,int>(abs,ord-1));
    resultat.push_back(pair<int,int>(abs+1,ord));
    resultat.push_back(pair<int,int>(abs-1,ord));

    for (vector<pair<int,int> >::iterator it = resultat.begin(); it != resultat.end();)
    {
        int a = it->first; int o = it->second;
        if (a<0 || a>=gobanSize)
        {
            it = resultat.erase(it);
        }
        else if (o<0 || o>=gobanSize)
        {
            it = resultat.erase(it);
        }
        else it++;
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
    vector<pair<int,int> > adj = intersectionsAdjacentes(gobanPtr);
    int libertes = adj.size();
    std::map<std::pair<int,int>,boost::shared_ptr<Pierre> > plateau = gobanPtr->getPlateau();

    for (std::vector<std::pair<int,int> >::iterator it = adj.begin(); it != adj.end(); it++)
    {
        if (plateau.find(*it)!=plateau.end())
        {
            libertes--;
        }
    }

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

    for (vector<pair<int,int> >::iterator it = resultat.begin(); it != resultat.end();)
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
        else it++;
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


Pierre::Pierre(Pierre const& p, bool copyGroupe) : rect(p.rect), m_coup(p.m_coup), m_name(p.m_name), ellipse(p.ellipse)
{
    if (copyGroupe)
    {
        m_groupe = p.m_groupe;
    }
}


Pierre::Pierre(boost::shared_ptr<Pierre> p, bool copyGroupe) : rect(p->rect), m_coup(p->m_coup), m_name(p->m_name), ellipse(p->ellipse)
{
    if (copyGroupe)
    {
        m_groupe = p->m_groupe;
    }
}
