#include "CoupException.h"


coup_exception::coup_exception(const std::string& i) throw():m_info(i){}

const char* coup_exception::what() const throw() { return m_info.c_str(); }

coupImpossible::coupImpossible(const std::string& i) throw():m_info(i){}

const char* coupImpossible::what() const throw() { return m_info.c_str(); }
