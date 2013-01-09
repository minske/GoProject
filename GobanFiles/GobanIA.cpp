#include "GobanIA.h"
#include "../IA/IA.h"
#include "../Tools/CoupException.h"

GobanIA::GobanIA(double ecart, int size) : Goban(ecart,size)
{
    std::cout << "GobanIA en cours de création\n";
    boost::shared_ptr<PartieIA> partiePtr (new PartieIA());
    std::cout << "eeeeeeeeeeeee";
//    partiePtr->init(sharedFromThis());
    std::cout << "oooo";
    m_partie = partiePtr;
    std::cout << "okokokok\n";
}

void GobanIA::init()
{
    try
    {
        std::cout << "init goban ia";
        boost::shared_ptr<PartieIA> partiePtr = boost::dynamic_pointer_cast<PartieIA>(m_partie);
        if (partiePtr.get()==0) throw coup_exception("\n!!!!!! partie NULL !!!!!\n"); ///ERREUR
        boost::shared_ptr<GobanIA> shrdThis = sharedFromThis();
        std::cout << "hahahahahahahahahah\n";
        if (shrdThis.get()!=0)
            partiePtr->init(shrdThis);
        else throw coup_exception("sharedFromThis de GobanIA = NULL");
    }
    catch(std::exception const& e)
    {
        std::cout << e.what();
        throw std::exception(e);
    }
}

bool GobanIA::coupPossible(int abs, int ord)
{
    try
    {
        std::pair<int,int> coord (abs,ord);
        std::map<std::pair<int,int>,boost::shared_ptr<Pierre> > plateau = getPlateau();

        if ((plateau.find(coord)!=plateau.end()) || (abs < 0) || (abs > 9) || (ord < 0) || (ord > 9))
        {
            return false;
        }
        else
        {
            boost::shared_ptr<Pierre> p;
            if ("noir"==getPartieIA()->couleurAJouer())
            {
                Coup c(abs,ord, m_partie->getNoir());
                c.setNum(getPartieIA()->getListeCoups().size());
                p.reset(new Pierre(c,ECART()));
            }
            else
            {
                Coup c(abs,ord, m_partie->getBlanc());
                c.setNum(getPartieIA()->getListeCoups().size());
                p.reset(new Pierre(c,ECART()));
            }

            m_copie.reset(new GobanIA(5,9));
            m_copie->m_partie = m_partie;
            m_copie->copieGroupes(sharedFromThis());

            m_copie->ajouterPierre(p,false);
            if (p->getGroupe().get()!=0)
            {
                if (p->getGroupe()->nbLibertes()==0)
                {
                    std::cout << "Groupe auquel la pierre ajoutée appartient n'a pas de libertés\n";
                    return false;
                }

                std::cout << "coup possible !\n";
            }
            else throw coup_exception("Erreur pierre mal ajoutée au goban copie");
            m_copie.reset();
        }

        return true;
        ///TODO à terminer
    }
    catch(std::exception const& e)
    {
        std::ostringstream errorMsg;
        errorMsg << "Impossible de déterminer si le coup " << abs << " - " << ord << " est possible : \n " << e.what();
        throw coup_exception(errorMsg.str());
    }
}

boost::shared_ptr<GobanIA> GobanIA::sharedFromThis()
{
    try
    {
        std::cout << "NOOOOOOOOOOOOOOOOOOOOOOOOOO\n";
        return boost::shared_dynamic_cast<GobanIA,Goban>(shared_from_this());
    }
    catch(std::exception const& e)
    {
        std::cout << "Impossible de créer un shared ptr à partir de GobanIA : " << e.what();
        throw std::exception(e);
    }
}

void GobanIA::copieGroupes(boost::shared_ptr<Goban> gobanPtr)
{
    for(std::set<boost::shared_ptr<Groupe> >::iterator it = gobanPtr->getGroupes().begin(); it != gobanPtr->getGroupes().end(); it++)
    {
        //copie de chaque groupe
        boost::shared_ptr<Groupe> groupePtr (new Groupe(*((*it).get())));
        ajouterGroupe(groupePtr);
    }
}

void GobanIA::ajouterGroupe(boost::shared_ptr<Groupe> groupePtr)
{
    m_groupes.insert(groupePtr);
    groupePtr->setGoban(sharedFromThis());
}
