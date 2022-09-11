#ifndef RPSEXCEPTION_H
#define RPSEXCEPTION_H

#include <exception>
#include "Error.h"

class RPSException : public std::exception
{
public:
    RPSException(Error error);
    // exception interface
    const char *what() const override;
    Error error();

private:
    Error m_error;
};

#endif // RPSEXCEPTION_H
