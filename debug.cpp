#include "debug.h"
#include <fstream>

namespace SGF {

Debug* Debug::m_instance = 0;

Debug* Debug::getInstance()
{
    if (m_instance!=0)
    {
        return m_instance;
    }
    else
    {
        m_instance = new Debug();
        return m_instance;
    }
}

void Debug::deleteInstance()
{
    if (m_instance!=0) delete m_instance;
}

Debug::Debug() : m_text(std::ostringstream())
{
    m_text << "=============  Lancement du programme ============== \n\n\n";
}

void Debug::add(alertLevel lvl, std::string const& str)
{
    if (lvl==Normal)
    {
        m_text << str << std::endl;
    }
    else if (lvl==Exception)
    {
        m_text << "\n\n ---------- EXCEPTION  : " << str << " -------------- \n\n";
    }

    std::ofstream myfile("debug.txt", std::ios::out | std::ios::trunc);
    if (myfile)
    {
        //fichier ouvert
        myfile << m_text.str();
        myfile << "\n\n ============= Fin du debug =============\n";
        myfile.close();
    }
}

void Debug::printToFile()
{
    std::ofstream myfile("C:\Users\Marie\debug.txt", std::ios::out | std::ios::trunc);

    if (myfile)
    {
        //fichier ouvert
        myfile << m_text.str();
        myfile << "\n\n ============= Fin du debug =============\n";
        myfile.close();
    }
    else throw std::exception("Impossible d'écrire dans le fichier de debug");
}

}

SGF::Debug& operator<<(SGF::Debug& dbg, std::string const& str)
{
    dbg.add(SGF::Normal,str);
    return dbg;
}


