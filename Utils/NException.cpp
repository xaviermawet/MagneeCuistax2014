#include "NException.hpp"

NException::NException(const QString& message, int code) :
    _message(message), _code(code)
{
}

NException::NException(const NException& copy) :
    _message(copy._message), _code(copy._code)
{
}

NException& NException::operator=(NException const& copy)
{
    if (this != &copy)
        this->_message = copy._message;

    return *this;
}

NException::~NException(void) throw()
{
    // No pointer to manage
}

QString NException::what(void) const throw()
{
    return this->_message;
}

int NException::code(void) const throw()
{
    return this->_code;
}
