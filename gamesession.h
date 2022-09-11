#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <QUuid>
#include <QString>
#include "Moves.h"

class GameSession
{
public:
    GameSession(QUuid &player1,QUuid &player2,QString name);

    enum Status{
        Player1Turn,
        Player2Turn,
        Finished,
        GameDone
    };

    const QString &getName() const;
    void setName(const QString &newName);

    const QUuid &player1() const;

    const QUuid &player2() const;

    void setPlayer1move(Moves newPlayer1m);

    void setPlayer2move(Moves newPlayer2m);

    QUuid result();

    void nextGame();

    Status status() const;

    const QUuid &id() const;

private:
    QUuid m_id;
    QString m_name;

    Status m_status;

    QUuid m_player1;
    QUuid m_player2;

    Moves m_player1m;
    Moves m_player2m;

};

#endif // GAMESESSION_H
