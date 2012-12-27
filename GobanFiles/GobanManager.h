#ifndef GOBANMANAGER_H
#define GOBANMANAGER_H

#include "Goban.h"

/*
  Classe GobanManager = classe qui gère les différentes instances de la classe Goban :
  Une pour le lecteur/éditeur de SGF, une pour le jeu contre l'ordinateur
*/

/*class GobanManager
{
public :
    static boost::shared_ptr<GobanManager> instance();

    boost::shared_ptr<Goban> getGoban(std::string);

private :
    GobanManager();

    std::map<std::string, boost::shared_ptr<Goban> > m_GobanInstances;
    static boost::shared_ptr<GobanManager> m_instance;
};*/

#endif // GOBANMANAGER_H
