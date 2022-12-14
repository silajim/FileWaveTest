#ifndef APPLICATIONUI_H
#define APPLICATIONUI_H

#include <QQmlApplicationEngine>
#include "rockpaperscissors.h"
#include "Moves.h"

#include <QString>
#include <QUuid>
#include <QList>
#include <QPair>

#include "DataStore.h"


#include <QAbstractListModel>

class GameComboModel:public QAbstractListModel{
    Q_OBJECT    

public:
    GameComboModel(std::shared_ptr<DataStore> datastore ,QObject *parent=nullptr);
    void addGame(Game game);
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
     std::shared_ptr<DataStore> m_datastore;
};

// I would refactor this to add a GameModel class that would have all of the properties related to the current game.
// The new class would be a Qobject with Q_PROPERTIES with it's respective getters and signals

class ApplicationUI: public QObject
{
    Q_OBJECT
    Q_PROPERTY(GameComboModel *gameCombomodel READ getGameCombomodel CONSTANT)
    Q_PROPERTY(QString gameName READ getCurrentGameName NOTIFY currentGameChanged)
    Q_PROPERTY(QString player1Name READ getCurrentP1Name NOTIFY currentGameChanged)
    Q_PROPERTY(QString player2Name READ getCurrentP2Name NOTIFY currentGameChanged)
    Q_PROPERTY(bool gamefinished READ isgameFinished NOTIFY gameDoneChanged)
    Q_PROPERTY(bool canp1move READ canp1move NOTIFY hasmovedChanged)
    Q_PROPERTY(bool canp2move READ canp2move NOTIFY hasmovedChanged)
public:
    ApplicationUI();
    ~ApplicationUI();

    enum Moves{
        Rock=::Moves::Rock,
        Paper=::Moves::Paper,
        Scissors=::Moves::Scissors
    };

    Q_ENUM(Moves);

public slots:

    bool createUser(QString user);
    bool login(QString user);
    void createGameSession(QString player1 , QString player2, QString name);
    void makeMove(QString username, ApplicationUI::Moves move);
    QString scores();
    void startNextGame();
    bool isgameFinished();
    GameComboModel *getGameCombomodel() const;

    void gameselectedChanged(int index);
    QString getCurrentGameName();
    QString getCurrentP1Name();
    QString getCurrentP2Name();

    bool canp1move();
    bool canp2move();

signals:
    void errorMessage(QString message);
    void gameCreated();
    void currentGameChanged();
    void winner(QString text);
    void gameDoneChanged();
    void nextGameStarted();
    void hasmovedChanged();

private:
     QQmlApplicationEngine engine;
     RockPaperScissors *rps=nullptr;
     GameComboModel *gameCombomodel=nullptr;
     std::shared_ptr<DataStore> datastore;
     std::shared_ptr<Game> currentgame;
};


#endif // APPLICATIONUI_H
