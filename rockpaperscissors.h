#ifndef ROCKPAPERSCISSORS_H
#define ROCKPAPERSCISSORS_H

#include <QString>
#include <QSet>
#include <QHash>
#include <QUuid>
#include <QMap>

#include <memory>

#include "gamesession.h"
#include "Moves.h"

class RockPaperScissors
{
public:
    RockPaperScissors();
    bool createUser(QString user);
    QUuid login(QString user);
    void logout(QUuid user);
//    QString getUserNameFromId()
    QUuid createGameSession(QUuid player1 , QUuid player2, QString name);
    void DeleteGameSession(QUuid session);
    bool checkSessionActive(QUuid session) noexcept;
    void setSessionName(QUuid session,QString name);
    QString geteSessionName(QUuid session);
    void makeMove(QUuid session, QUuid player, Moves move);
    QUuid result(QUuid session);
    void StartNextGame(QUuid session);

    QList<QPair<QString,uint>> gethighScores();

private:
    QSet<QString> m_users;
    QMap<QString,QUuid> m_loggedin;
    QHash<QUuid,std::shared_ptr<GameSession>> m_gameSessions;
    QMap<QString,uint> m_points;

    bool checkLogin(QUuid user);
    QString getUserforID(QUuid user);
//    QSet<QUuid> m_loggedin;
};

#endif // ROCKPAPERSCISSORS_H
