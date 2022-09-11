#include "rpsexception.h"

RPSException::RPSException(Error error):m_error(error)
{

}

const char *RPSException::what() const
{
    switch (m_error) {
    case Error::InvalidUSer:
        return "Invalid USer";
        break;
    case Error::UserExists:
        return "User allready exists";
        break;
    default:
        return "";
        break;
    }
}

Error RPSException::error()
{
    return m_error;
}
