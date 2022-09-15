#ifndef DATASTORE_H
#define DATASTORE_H

#include <QString>
#include <QUuid>
#include <QList>
#include <QPair>

struct Game{
    QUuid id;
    QString name;
    bool canp1move = true; //Maybe convert this to an API call?
    QUuid player1;
    QString player1UserName;
    bool canp2move = true; //Maybe convert this to an API call?
    QUuid player2;
    QString player2UserName;
};

struct DataStore{
    QList<QPair<QString,QUuid>> m_logins;
    QList<std::shared_ptr<Game>> gamelist;

    QString userfromID(QUuid id){
        foreach(auto user,m_logins){
            if(user.second==id)
                return user.first;
        }
        return "";
    }

    QUuid idfromUser(QString user){
        foreach(auto login,m_logins){
            if(login.first==user)
                return login.second;
        }
        return QUuid();
    }
};

#endif // DATASTORE_H
