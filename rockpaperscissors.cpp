#include "rockpaperscissors.h"

#include <exception>
#include "rpsexception.h"
#include <QUuid>
#include <QFile>

RockPaperScissors::RockPaperScissors()
{
    //*******************************************
    /*Instead of serializing and de-serializing the user and point arrays the proper way should be to have a DB with 2 tables, one for useres, the other for the high scores
     * CREATE TABLE Users (
        id integer PRIMARY KEY AUTOINCREMENT,
        username string
       );

       CREATE TABLE Scores (
        id integer,
        score integer
        FOREIGN KEY(id) REFERENCES Users(id)
       );
     * */
    QFile file("data.dat");
    if(file.open(QIODevice::ReadOnly)){
        QDataStream in(&file);
        in >> m_users >> m_points;
    }
}

RockPaperScissors::~RockPaperScissors()
{
    foreach(auto game , m_gameSessions){
        if(game->status() == GameSession::GameDone){
            result(game->id());
        }
    }

    QFile file("data.dat");
    if(file.open(QIODevice::WriteOnly)){
        QDataStream out(&file);
        out << m_users << m_points;
    }
}

void RockPaperScissors::clearData()
{


    if(QFile::exists("data.dat")){
        QFile::remove("data.dat");
    }
    m_users.clear();
    m_loggedin.clear();
    m_gameSessions.clear();
    m_points.clear();
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
    if(!m_users.contains(user) || m_loggedin.contains(user)){
        throw RPSException(Error::InvalidUSer);
    }

    m_loggedin[user] = QUuid::createUuid();

    return m_loggedin[user];
}

void RockPaperScissors::logout(QUuid user)
{
    if(!checkLogin(user))
        throw RPSException(Error::InvalidUSer);

    QString username = getUserforID(user);
    if(!username.isEmpty()){
        m_loggedin.remove(username);
    }
}

QUuid RockPaperScissors::createGameSession(QUuid player1, QUuid player2,QString name)
{
    if(player1 == player2 && !checkLogin(player1) && !checkLogin(player2))
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

    if(ses->player1() != player && ses->player2() != player){
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
    bool updateStats =  m_gameSessions[session]->status() == GameSession::GameDone;

    try{
        winner = m_gameSessions[session]->result();
    } catch (RPSException e) {
        throw e;
    }
    if(!winner.isNull()){
        uint points=0;
        QString username;
        if(updateStats){
            username = getUserforID(winner);
            if(!username.isEmpty() && m_points.contains(username)){
                points = m_points[username];
            }
            ++points;
            if(!username.isEmpty())
                m_points[username] = points;
        }
    }

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

bool RockPaperScissors::isDone(QUuid session)
{
    if(!m_gameSessions.contains(session)){
        throw RPSException(Error::InvalidGame);
    }
    return m_gameSessions[session]->status() == GameSession::GameDone;
}

bool RockPaperScissors::isFinished(QUuid session)
{
    if(!m_gameSessions.contains(session)){
        throw RPSException(Error::InvalidGame);
    }
    return m_gameSessions[session]->status() == GameSession::Finished;
}

QList<QPair<QString, uint> > RockPaperScissors::gethighScores()
{
    foreach(auto game , m_gameSessions){
        if(game->status() == GameSession::GameDone){
            result(game->id());
        }
    }

    QList<QPair<QString, uint>> scores;
    for(auto i= m_points.begin();i != m_points.end() ; ++i){
        scores << qMakePair(i.key(),i.value());
    }
    return scores;
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

QString RockPaperScissors::getUserforID(QUuid user)
{
    for(auto i= m_loggedin.begin();i != m_loggedin.end() ; ++i){
        if(i.value()==user){
            return i.key();
        }
    }
    return "";
}
