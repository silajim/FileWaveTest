#include "rpsexception.h"

RPSException::RPSException(Error error):m_error(error)
{

}

const char *RPSException::what() const
{
    switch (m_error) {
    case Error::InvalidUSer:
        return "Invalid User";
        break;
    case Error::UserExists:
        return "User allready exists";
        break;
    case Error::InvalidGame:
        return "Game not valid";
        break;
    case Error::NotDone:
        return "Game not done, turn pending";
        break;
    case Error::NotFinished:
        return "Game result not read";
        break;
    case Error::NotTurn:
        return "Not players turn";
        break;
    default:
        return "Uknown Error";
        break;
    }
}

Error RPSException::error()
{
    return m_error;
}
