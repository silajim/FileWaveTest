#include "applicationui.h"
#include <QGuiApplication>
#include <QQmlContext>
#include "rpsexception.h"
#include "Error.h"

ApplicationUI::ApplicationUI()
{   
    rps = new RockPaperScissors();
    datastore = std::make_shared<DataStore>();
    gameCombomodel = new GameComboModel(datastore,this);
    qRegisterMetaType<Moves>("Moves");
    qmlRegisterUncreatableType<ApplicationUI>("com.AppUI", 1,0,"AppUI","Cannot create this in QML");

    engine.rootContext()->setContextProperty("app",this);
    const QUrl url(u"qrc:/FileWaveTest/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     qApp, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    ///Debug data to make debugging easier

    //    rps->clearData();

//    createUser("user1");
//    createUser("user2");
//    createUser("user3");
//    createUser("user4");
//    login("user1");
//    login("user2");
//    login("user3");
//    login("user4");
//    createGameSession("user1","user2","test");
//    createGameSession("user3","user4","AAATEST");
}

ApplicationUI::~ApplicationUI()
{
    delete rps;
}

bool ApplicationUI::createUser(QString user)
{
    user = user.toLower();
    try {
        rps->createUser(user);
    } catch (RPSException e) {
        if(e.error()==Error::UserExists)
            emit errorMessage("User allready exists");
        else if(e.error() == Error::InvalidUSer)
            emit errorMessage("Invalid user");
        else
            emit errorMessage("Unkown error");
    }
    return true;
}

bool ApplicationUI::login(QString user)
{
    QUuid id;
    user = user.toLower();
    try {
        id = rps->login(user);
    } catch (RPSException e) {
        if(e.error()==Error::UserExists)
            emit errorMessage("User allready exists");
        else if(e.error() == Error::InvalidUSer)
            emit errorMessage("Invalid user");
        else
            emit errorMessage("Unkown error");
    }

    datastore->m_logins << qMakePair(user,id);
    return true;
}

void ApplicationUI::createGameSession(QString player1, QString player2, QString name)
{
    player1 = player1.toLower();
    player2 = player2.toLower();
    Game g;
    QUuid p1,p2;
    p1 = datastore->idfromUser(player1);
    p2 = datastore->idfromUser(player2);

    if(p1.isNull() || p2.isNull()){
        emit errorMessage("Invalid user");
        return;
    }

    try{
        g.id = rps->createGameSession(p1,p2,name);
    }catch(RPSException e){
        emit errorMessage(e.what());
    }
    g.name = name;
    g.player1 = p1;
    g.player2 = p2;
    g.player1UserName = player1;
    g.player2UserName = player2;

    gameCombomodel->addGame(g);

    emit gameCreated();
}

void ApplicationUI::makeMove(QString username, ApplicationUI::Moves move)
{
    if(!currentgame)
        return;

    QUuid id;
    bool p1=false;
    bool p2=false;
    if(username == currentgame->player1UserName){
        id = currentgame->player1;
        p1=true;
    }else if(username == currentgame->player2UserName){
        id = currentgame->player2;
        p2=true;
    }

    try {
        rps->makeMove(currentgame->id,id, static_cast<::Moves>(move));
    } catch (RPSException e) {
        emit errorMessage(e.what());
    }

    if(p1){
        currentgame->canp1move = false;
    }else if(p2){
        currentgame->canp2move = false;
    }

    emit hasmovedChanged();

//    emit moveMade(username);

    if(rps->isDone(currentgame->id)){
        QUuid win = rps->result(currentgame->id);
        if(win.isNull()){
            emit winner(tr("stalemate"));
        }else{
            QString username = datastore->userfromID(win);
            emit winner(username + " WINS!");
        }

        emit gameDoneChanged();
    }

}

QString ApplicationUI::scores()
{
    QString scores;
    auto sc = rps->gethighScores();
    QString str("%1 -> %2 \n");
    foreach(auto score , sc){
        scores+=str.arg(score.first).arg(score.second);
    }
    return scores;
}


void ApplicationUI::startNextGame()
{
    try {
        rps->StartNextGame(currentgame->id);
    } catch (RPSException e) {
        emit errorMessage(e.what());
    }

    currentgame->canp1move= true;
    currentgame->canp2move= true;


    emit hasmovedChanged();

    emit gameDoneChanged();
    emit nextGameStarted();
}

bool ApplicationUI::isgameFinished()
{
    if(!currentgame)
        return false;

    bool finished=false;
    try {
        finished = rps->isFinished(currentgame->id);
    } catch (RPSException e) {
        emit errorMessage(e.what());
        return false;
    }
    return finished;
}

GameComboModel *ApplicationUI::getGameCombomodel() const
{
    return gameCombomodel;
}

void ApplicationUI::gameselectedChanged(int index)
{
    currentgame = datastore->gamelist[index];
    emit currentGameChanged();
    emit gameDoneChanged();
    emit hasmovedChanged();
}

QString ApplicationUI::getCurrentGameName()
{
    if(!currentgame)
        return "";
    return currentgame->name;
}

QString ApplicationUI::getCurrentP1Name()
{
    if(!currentgame)
        return "";
    return currentgame->player1UserName;
}

QString ApplicationUI::getCurrentP2Name()
{
    if(!currentgame)
        return "";
    return currentgame->player2UserName;
}

bool ApplicationUI::canp1move()
{
    if(!currentgame)
        return true;
    return currentgame->canp1move;
}

bool ApplicationUI::canp2move()
{
    if(!currentgame)
        return true;
    return currentgame->canp2move;
}

GameComboModel::GameComboModel(std::shared_ptr<DataStore> datastore ,QObject *parent) : QAbstractListModel(parent) , m_datastore(datastore)
{

}

void GameComboModel::addGame(Game game)
{
    beginInsertRows(QModelIndex(), m_datastore->gamelist.size() ,  m_datastore->gamelist.size());
    m_datastore->gamelist.append(std::make_shared<Game>(game));
    endResetModel();
}

int GameComboModel::rowCount(const QModelIndex &parent) const
{
    return m_datastore->gamelist.size();
}

QVariant GameComboModel::data(const QModelIndex &index, int role) const
{
    if(index.row() >= m_datastore->gamelist.size()){
        return QVariant();
    }

    QString gtext("%1 : %2 %3");
    std::shared_ptr<Game> g = m_datastore->gamelist[index.row()];
    gtext = gtext.arg(g->name).arg(m_datastore->userfromID(g->player1)).arg(m_datastore->userfromID(g->player2));

    return gtext;
}
