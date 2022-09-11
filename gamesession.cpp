#include "gamesession.h"
#include "rpsexception.h"
GameSession::GameSession(QUuid &player1,QUuid &player2,QString name): m_player1(player1) , m_player2(player2) , m_name(name)
{
    m_id = QUuid::createUuid();
    m_status = GameSession::Player1Turn;
}

const QString &GameSession::getName() const
{
    return m_name;
}

void GameSession::setName(const QString &newName)
{
    m_name = newName;
}

const QUuid &GameSession::player1() const
{
    return m_player1;
}

const QUuid &GameSession::player2() const
{
    return m_player2;
}

void GameSession::setPlayer1move(Moves newPlayer1m)
{
    if(m_status!=GameSession::Player1Turn)
        throw RPSException(Error::NotTurn);

    m_player1m = newPlayer1m;

    m_status=GameSession::Player2Turn;
}

void GameSession::setPlayer2move(Moves newPlayer2m)
{
    if(m_status!=GameSession::Player2Turn)
        throw RPSException(Error::NotTurn);

    m_player2m = newPlayer2m;
    m_status = GameSession::GameDone;
}

QUuid GameSession::result()
{
    if(m_status != GameSession::GameDone || m_status != GameSession::Finished)
        throw RPSException(Error::NotDone);
    QUuid winner;

    if(m_player1m == Moves::Rock && m_player2m == Moves::Paper)
        winner =  m_player2;
    else if(m_player1m == Moves::Paper && m_player2m == Moves::Scissors)
        winner = m_player2;
    else if(m_player1m == Moves::Scissors && m_player2m == Moves::Rock)
        winner = m_player2;
    else if(m_player1m == Moves::Rock && m_player2m == Moves::Scissors)
        winner = m_player1;
    else if(m_player1m == Moves::Scissors && m_player2m == Moves::Paper)
        winner = m_player1;
    else if(m_player1m == Moves::Paper && m_player2m == Moves::Rock)
        winner = m_player1;

    m_status = GameSession::Finished;
    return winner;

}

void GameSession::nextGame()
{
    if(m_status != GameSession::Finished)
        throw RPSException(Error::NotFinished);

    m_status = GameSession::Player1Turn;

}

GameSession::Status GameSession::status() const
{
    return m_status;
}

const QUuid &GameSession::id() const
{
    return m_id;
}
