#ifndef COUPEXCEPTION_H
#define COUPEXCEPTION_H

#include <iostream>

class coup_exception : public std::exception {
    std::string m_info;
public:
    coup_exception(const std::string& i) throw();
    const char* what() const throw();
    virtual ~coup_exception() throw() {}
};

#endif // COUPEXCEPTION_H
