#ifndef PARTIEMANAGER_H
#define PARTIEMANAGER_H

#include "Partie.h"
#include <boost/shared_ptr.hpp>
#include <map>

/*
  Classe PartieManager = classe qui g�re les diff�rentes instances de la classe Partie :
  Une pour le lecteur/�diteur de SGF, une pour le jeu contre l'ordinateur
*/
/*
class PartieManager
{
public :
    static boost::shared_ptr<PartieManager> instance();

    boost::shared_ptr<Partie> getPartie(std::string);

private :
    PartieManager();

    std::map<std::string, boost::shared_ptr<Partie> > m_PartieInstances;
    static boost::shared_ptr<PartieManager> m_instance;
};*/

#endif // PARTIEMANAGER_H
