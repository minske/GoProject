#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <sstream>

namespace SGF
{

typedef enum {Normal, Exception} alertLevel;

class Debug
{
    public :
        static Debug* getInstance();
        static void deleteInstance();
        void printToFile();
        void add(alertLevel lvl, std::string const& str);

    private :
        Debug();
        ~Debug() {}

        static Debug* m_instance;
        std::ostringstream m_text;
};

}

SGF::Debug& operator<<(SGF::Debug& dbg, std::string const& str);

#endif // DEBUG_H
