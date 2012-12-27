#include "GobanManager.h"

/*boost::shared_ptr<GobanManager> GobanManager::m_instance;

boost::shared_ptr<GobanManager> GobanManager::instance()
{
    if (m_instance.get()!=0)
        return m_instance;
    else
    {
        m_instance.reset(new GobanManager());
        return m_instance;
    }
}

boost::shared_ptr<Goban> GobanManager::getGoban(std::string name)
{
    if (m_GobanInstances.find(name)!=m_GobanInstances.end())
    {
        return m_GobanInstances.at(name);
    }
    else
    {
        boost::shared_ptr<Goban> newInstance (new Goban());
        m_GobanInstances.insert(std::pair<std::string,boost::shared_ptr<Goban>(name,newInstance));
        return newInstance;
    }
}

GobanManager::GobanManager() {}
*/
