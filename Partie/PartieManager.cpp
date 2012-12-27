#include "PartieManager.h"
/*
boost::shared_ptr<PartieManager> PartieManager::m_instance;

boost::shared_ptr<PartieManager> PartieManager::instance()
{
    if (m_instance.get()!=0)
        return m_instance;
    else
    {
        m_instance.reset(new PartieManager());
        return m_instance;
    }
}

boost::shared_ptr<Partie> PartieManager::getPartie(std::string name)
{
    if (m_PartieInstances.find(name)!=m_PartieInstances.end())
    {
        return m_PartieInstances.at(name);
    }
    else
    {
        boost::shared_ptr<Partie> newInstance (new Partie());
        m_PartieInstances.insert(std::pair<std::string,boost::shared_ptr<Partie> >(name,newInstance));
        return newInstance;
    }
}

PartieManager::PartieManager() {}

*/
