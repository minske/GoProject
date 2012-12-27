#ifndef USER_H
#define USER_H
#include "../Partie/Joueur.h"
#include "../Partie/PartieIA.h"

class User : public Joueur
{

    private :
        boost::weak_ptr<PartieIA> m_partie;
        boost::weak_ptr<GobanIA> m_goban;

    public :
        User(boost::shared_ptr<PartieIA> partie, boost::shared_ptr<GobanIA> goban, std::string couleur="noir");

};

#endif // USER_H
