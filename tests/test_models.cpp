#include "test_models.h"
#include "../src/models/user.h"
#include "../src/models/game.h"

void TestModels::initTestCase()
{
    // Called once before all tests
}

void TestModels::cleanupTestCase()
{
    // Called once after all tests
}

void TestModels::testUserCreation()
{
    User user(QString("testUser"));
    QVERIFY(!user.getUsername().isEmpty());
    QCOMPARE(user.getUsername(), QString("testUser"));
}

void TestModels::testUserProperties()
{
    User user(QString("player1"));
    QCOMPARE(user.getUsername(), QString("player1"));

    user.setWins(10);
    QCOMPARE(user.getWins(), 10);

    user.setLosses(5);
    QCOMPARE(user.getLosses(), 5);
}

void TestModels::testUserEquality()
{
    User user1(QString("alice"));
    User user2(QString("alice"));
    User user3(QString("bob"));

    // Users with same username should be considered equal
    QCOMPARE(user1.getUsername(), user2.getUsername());
    QVERIFY(user1.getUsername() != user3.getUsername());
}

void TestModels::testGameCreation()
{
    Game game(QString("game123"));
    QVERIFY(!game.getName().isEmpty());
    QCOMPARE(game.getName(), QString("game123"));
}

void TestModels::testGameProperties()
{
    Game game(QString("testGame"));
    QCOMPARE(game.getName(), QString("testGame"));

    game.setGameId(1);
    QCOMPARE(game.getGameId(), static_cast<qulonglong>(1));

    game.setPlayerCount(2);
    QCOMPARE(game.getPlayerCount(), 2);
}

void TestModels::testGameValidation()
{
    Game game(QString("validGame"));
    QVERIFY(!game.getName().isEmpty());

    game.addUsedWord(QString("word1"));
    QCOMPARE(game.getUsedWordsCount(), 1);
}

QTEST_MAIN(TestModels)
#include "test_models.moc"
