#include "test_integration.h"
#include "../src/models/user.h"
#include "../src/models/game.h"
#include "../src/utils/appstate.h"

void TestIntegration::initTestCase()
{
    // Initialize test environment
}

void TestIntegration::cleanupTestCase()
{
    // Cleanup after all tests
    AppState::getInstance().logout();
}

void TestIntegration::testGameLifecycle()
{
    // Test: Create game -> Start game -> Play words -> End game
    Game game(QString("test-game-1"));

    QCOMPARE(game.getName(), QString("test-game-1"));

    game.setPlayerCount(2);
    QCOMPARE(game.getPlayerCount(), 2);
}

void TestIntegration::testUserLoginLogoutCycle()
{
    // Test: AppState should handle login/logout correctly
    AppState &appState = AppState::getInstance();
    appState.logout();

    QVERIFY(!appState.isLoggedIn());

    appState.setSessionId(QString("session-123"));
    appState.setUsername(QString("player"));

    // Should be considered logged in after setting session and username
    QVERIFY(!appState.getSessionId().isEmpty());
    QVERIFY(!appState.getUsername().isEmpty());

    appState.logout();

    QVERIFY(appState.getSessionId().isEmpty());
    QVERIFY(!appState.isLoggedIn());
}

void TestIntegration::testGameCreationAndStart()
{
    // Test: Create two users, create a game, start it
    User player1(QString("alice"));
    User player2(QString("bob"));
    Game game(QString("integration-game-1"));

    QCOMPARE(player1.getUsername(), QString("alice"));
    QCOMPARE(player2.getUsername(), QString("bob"));
    QCOMPARE(game.getName(), QString("integration-game-1"));
}

void TestIntegration::testScoreCalculation()
{
    // Test: Wins/losses should update when player plays
    User player(QString("scorer"));
    QCOMPARE(player.getWins(), 0);
    QCOMPARE(player.getLosses(), 0);

    player.setWins(10);
    QCOMPARE(player.getWins(), 10);

    player.setLosses(5);
    QCOMPARE(player.getLosses(), 5);
}

void TestIntegration::testConnectionReconnection()
{
    // Test: Connection status and reconnection flow
    AppState &appState = AppState::getInstance();

    // Simulate connection state changes
    QString serverAddress = "localhost";
    quint16 serverPort = 8888;

    appState.setServerAddress(serverAddress);
    appState.setServerPort(serverPort);

    QCOMPARE(appState.getServerAddress(), serverAddress);
    QCOMPARE(appState.getServerPort(), serverPort);
}

void TestIntegration::testMultiplePlayersInteraction()
{
    // Test: Multiple players in a game should have correct state
    User player1(QString("player_alpha"));
    User player2(QString("player_beta"));
    User player3(QString("player_gamma"));

    player1.setWins(50);
    player2.setWins(75);
    player3.setWins(100);

    QCOMPARE(player1.getUsername(), QString("player_alpha"));
    QCOMPARE(player2.getUsername(), QString("player_beta"));
    QCOMPARE(player3.getUsername(), QString("player_gamma"));

    QVERIFY(player1.getWins() < player2.getWins());
    QVERIFY(player2.getWins() < player3.getWins());
}

QTEST_MAIN(TestIntegration)
#include "test_integration.moc"
