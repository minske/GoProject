#include "pierre.h"
#include "FP.h"

Pierre::Pierre(Coup c) : m_coup(c)
{
    if (c.getJoueur()->getNom()=="Kiral")
        ellipse = boost::shared_ptr<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap("pierreRose.png").scaled(FP::ECART_T()*R,FP::ECART_T()*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));

    else if (c.getJoueur()->couleur()=="Noir")
    ellipse = boost::shared_ptr<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap("pierreNoire.png").scaled(FP::ECART_T()*R,FP::ECART_T()*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));


    else
    {
        unsigned int nb = rand() % 6 +1;
        ostringstream os;
        os << "pierreBlanche" << nb << ".png";
        QString nomFichier = QString::fromStdString(os.str());
        ellipse = boost::shared_ptr<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap(nomFichier).scaled(FP::ECART_T()*R,FP::ECART_T()*R,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    }

    ellipse->setX((c.getAbs()+1)*FP::ECART_T()-(FP::ECART_T()*R/2));
    ellipse->setY((c.getOrd()+1)*FP::ECART_T()-(FP::ECART_T()*R/2));

    //rect.setWidth(E*0.9);
    //rect.setHeight(E*0.9);
    //les pierres font 100 pixels de diamètre, on veut qu'elles fassent E*0,9
    //ellipse->setScale(E*0.9/100);

    ostringstream name;
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

    int libertes = adj.size();
    for (vector<pair<int,int> >::iterator it = adj.begin(); it != adj.end(); it++)
    {
        if (Goban::getInstance()->getPlateau().find(*it)!=Goban::getInstance()->getPlateau().end())
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
        if (a<0 || a>=Goban::SIZE())
        {
            it = resultat.erase(it);
        }
        else if (o<0 || o>=Goban::SIZE())
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
    Goban* g = Goban::getInstance();

    for (vector<pair<int,int> >::iterator it = intersect.begin(); it != intersect.end(); it++)
    {
        if (g->getPlateau().find(*it) != g->getPlateau().end())
        {
            boost::shared_ptr<Pierre> pierrePtr = g->getPlateau().find(*it)->second;
            if (pierrePtr->couleur() == this->couleur())
            {
                result.push_back(pierrePtr);
            }
        }
    }

    return result;
}
