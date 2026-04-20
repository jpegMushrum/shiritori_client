#include "test_utils.h"
#include "../src/utils/appstate.h"

void TestUtils::initTestCase()
{
    // Called once before all tests
}

void TestUtils::cleanupTestCase()
{
    // Called once after all tests
    AppState::getInstance().logout();
}

void TestUtils::testAppStateSessionId()
{
    AppState &appState = AppState::getInstance();
    appState.logout();

    QString testSessionId = "test-session-123";
    appState.setSessionId(testSessionId);

    QCOMPARE(appState.getSessionId(), testSessionId);
}

void TestUtils::testAppStateUsername()
{
    AppState &appState = AppState::getInstance();
    appState.logout();

    QString testUsername = "testPlayer";
    appState.setUsername(testUsername);

    QCOMPARE(appState.getUsername(), testUsername);
}

void TestUtils::testAppStateServerAddress()
{
    AppState &appState = AppState::getInstance();

    QString testAddress = "192.168.1.100";
    quint16 testPort = 8888;

    appState.setServerAddress(testAddress);
    appState.setServerPort(testPort);

    QCOMPARE(appState.getServerAddress(), testAddress);
    QCOMPARE(appState.getServerPort(), testPort);
}

void TestUtils::testAppStateLoginLogout()
{
    AppState &appState = AppState::getInstance();
    appState.logout();

    QVERIFY(!appState.isLoggedIn());

    appState.setSessionId(QString("session-123"));
    appState.setUsername(QString("player"));
    QVERIFY(!appState.getSessionId().isEmpty());
    QVERIFY(!appState.getUsername().isEmpty());
    QVERIFY(appState.isLoggedIn());
}

void TestUtils::testAppStateSingleton()
{
    AppState &appState1 = AppState::getInstance();
    AppState &appState2 = AppState::getInstance();

    appState1.setSessionId("singleton-test");

    QCOMPARE(appState2.getSessionId(), QString("singleton-test"));
}

QTEST_MAIN(TestUtils)
#include "test_utils.moc"
