#include "rockpaperscissors.h"

#include <exception>
#include "rpsexception.h"
#include <QUuid>

RockPaperScissors::RockPaperScissors()
{

}

bool RockPaperScissors::createUser(QString user)
{
    if(user.isEmpty()){
        throw RPSException(Error::InvalidUSer);
    }

    if(m_users.contains(user)){
        throw RPSException(Error::UserExists);
    }

    m_users << user;
    return true;
}

QUuid RockPaperScissors::login(QString user)
{
    if(!m_users.contains(user)){
        throw RPSException(Error::InvalidUSer);
    }

    m_loggedin[user] = QUuid::createUuid();

    return m_loggedin[user];
}

void RockPaperScissors::logout(QUuid user)
{
    if(!checkLogin(user))
        throw RPSException(Error::InvalidUSer);

    for(auto i= m_loggedin.begin();i != m_loggedin.end() ; ++i){
        if(i.value()==user){
            m_loggedin.remove(i.key());
            break;
        }
    }
}

QUuid RockPaperScissors::createGameSession(QUuid player1, QUuid player2,QString name)
{
    if(!checkLogin(player1) && !checkLogin(player2))
        throw RPSException(Error::InvalidUSer);

    auto session = std::make_shared<GameSession>(player1,player2,name);
    m_gameSessions[session->id()] = session;
    return session->id();
}

void RockPaperScissors::DeleteGameSession(QUuid session)
{
    if(!m_gameSessions.contains(session)){
        throw RPSException(Error::InvalidGame);
    }

    m_gameSessions.remove(session);

}

bool RockPaperScissors::checkSessionActive(QUuid session) noexcept
{
    if(!m_gameSessions.contains(session)){
        return false;
    }

    return true;
}

void RockPaperScissors::setSessionName(QUuid session, QString name)
{
    if(!m_gameSessions.contains(session)){
        throw RPSException(Error::InvalidGame);
    }

    m_gameSessions[session]->setName(name);
}

QString RockPaperScissors::geteSessionName(QUuid session)
{
    if(!m_gameSessions.contains(session)){
        throw RPSException(Error::InvalidGame);
    }

    return  m_gameSessions[session]->getName();
}

void RockPaperScissors::makeMove(QUuid session, QUuid player, Moves move)
{
    if(!checkLogin(player))
        throw RPSException(Error::InvalidUSer);


    if(!m_gameSessions.contains(session)){
        throw RPSException(Error::InvalidGame);
    }
    auto ses = m_gameSessions[session];

    if(ses->player1() != player && ses->player2() == player){
        throw RPSException(Error::InvalidUSer);
    }

    try {
        if(ses->player1() == player){
            ses->setPlayer1move(move);
        }
        if(ses->player2() == player){
            ses->setPlayer2move(move);
        }

    } catch (RPSException e) {
        throw e;
    }

}

QUuid RockPaperScissors::result(QUuid session)
{
    if(!m_gameSessions.contains(session)){
        throw RPSException(Error::InvalidGame);
    }
    QUuid winner;

    try{
        winner = m_gameSessions[session]->result();
    } catch (RPSException e) {
        throw e;
    }

    uint points=0;
    if(m_points.contains(winner)){
        points = m_points[winner];
    }
    ++points;

    m_points[winner] = points;

    return winner;
}

void RockPaperScissors::StartNextGame(QUuid session)
{
    if(!m_gameSessions.contains(session)){
        throw RPSException(Error::InvalidGame);
    }

    try{
        m_gameSessions[session]->nextGame();
    } catch (RPSException e) {
        throw e;
    }
}

bool RockPaperScissors::checkLogin(QUuid user)
{
    bool found = false;
    for(auto uid : m_loggedin){
        if(uid==user){
            found = true;
            break;
        }

    }
    return found;
}
