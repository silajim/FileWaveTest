#include "rockpaperscissorslibtest.h"
#include "rockpaperscissors.h"
#include "Error.h"
#include "Moves.h"
#include "rpsexception.h"

rockpaperscissorsLibTest::rockpaperscissorsLibTest(QObject *parent) : QObject{parent}
{

}

void rockpaperscissorsLibTest::addLoginUser()
{
    RockPaperScissors rps;
    rps.clearData();
    rps.createUser("User1");
    rps.createUser("User2");
    QVERIFY_EXCEPTION_THROWN(rps.createUser("User1"),std::exception);

    QUuid user1id = rps.login("User1");
    QVERIFY_EXCEPTION_THROWN(rps.login("User1"),std::exception);
    QUuid user2id = rps.login("User2");

    rps.clearData();
}

void rockpaperscissorsLibTest::testGame()
{
    RockPaperScissors rps;
    rps.clearData();
    rps.createUser("User1");
    rps.createUser("User2");

    rps.createUser("User3");
    rps.createUser("User4");


    QUuid user1id = rps.login("User1");
    QUuid user2id = rps.login("User2");


    QUuid user3id = rps.login("User3");
    QUuid user4id = rps.login("User4");


    QUuid session1id = rps.createGameSession(user1id,user2id,"Test");
    QUuid session2id = rps.createGameSession(user3id,user4id,"Test1");
    rps.makeMove(session1id,user1id,Moves::Paper);
    rps.makeMove(session2id,user3id,Moves::Scissors);
    QVERIFY_EXCEPTION_THROWN(rps.makeMove(session2id,user1id,Moves::Paper),std::exception);
    QVERIFY_EXCEPTION_THROWN(rps.makeMove(session1id,user1id,Moves::Paper),std::exception);
    rps.makeMove(session1id,user2id,Moves::Rock);
    QUuid result = rps.result(session1id);
    rps.result(session1id);
    QCOMPARE(user1id,result);
    rps.StartNextGame(session1id);

    rps.makeMove(session2id,user4id,Moves::Rock);
    QCOMPARE(rps.result(session2id) , user4id);

    auto scores = rps.gethighScores();

    QCOMPARE(scores.size(),2);
    QCOMPARE(scores[0].first , "User1");
    QCOMPARE(scores[0].second , 1);
    QCOMPARE(scores[1].first , "User4");
    QCOMPARE(scores[1].second , 1);

    rps.clearData();

}

void rockpaperscissorsLibTest::testDataSerialization()
{
    {
        RockPaperScissors rps;
        rps.clearData();
        rps.createUser("User1");
        rps.createUser("User2");
        rps.createUser("User3");
        rps.createUser("User4");


        QUuid user1id = rps.login("User1");
        QUuid user2id = rps.login("User2");

        QUuid user3id = rps.login("User3");
        QUuid user4id = rps.login("User4");


        QUuid session1id = rps.createGameSession(user1id,user2id,"Test");
        QUuid session2id = rps.createGameSession(user3id,user4id,"Test1");
        rps.makeMove(session1id,user1id,Moves::Paper);
        rps.makeMove(session2id,user3id,Moves::Scissors);
        rps.makeMove(session1id,user2id,Moves::Rock);
        rps.makeMove(session2id,user4id,Moves::Rock);

        auto scores = rps.gethighScores();

        QCOMPARE(scores.size(),2);
        QCOMPARE(scores[0].first , "User1");
        QCOMPARE(scores[0].second , 1);
        QCOMPARE(scores[1].first , "User4");
        QCOMPARE(scores[1].second , 1);
    }
    {
        RockPaperScissors rps;
        QVERIFY_EXCEPTION_THROWN(rps.createUser("User1"),std::exception);
        QVERIFY_EXCEPTION_THROWN(rps.createUser("User4"),std::exception);

        auto scores = rps.gethighScores();

        QCOMPARE(scores.size(),2);
        QCOMPARE(scores[0].first , "User1");
        QCOMPARE(scores[0].second , 1);
        QCOMPARE(scores[1].first , "User4");
        QCOMPARE(scores[1].second , 1);

        rps.clearData();
    }
}

QTEST_MAIN(rockpaperscissorsLibTest)
#include "rockpaperscissorslibtest.moc"
