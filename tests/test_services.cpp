#include "test_services.h"
#include "../src/services/apiservice.h"
#include "../src/services/serverprotocol.h"
#include "../src/services/tcpclient.h"

TestServices::TestServices()
    : m_apiService(nullptr), m_tcpClient(nullptr)
{
}

void TestServices::initTestCase()
{
    // Create TCP client mock/instance
    m_tcpClient = new TcpClient(this);

    // Create API service with TCP client
    m_apiService = new ApiService(m_tcpClient, this);
}

void TestServices::cleanupTestCase()
{
    if (m_apiService)
    {
        delete m_apiService;
        m_apiService = nullptr;
    }
    if (m_tcpClient)
    {
        delete m_tcpClient;
        m_tcpClient = nullptr;
    }
}

// ==================== ApiService Tests ====================

void TestServices::testApiServiceLoginAsync()
{
    // Test that loginAsync emits loginError when tcpClient is nullptr
    ApiService apiService(nullptr, this);

    QSignalSpy loginErrorSpy(&apiService, &ApiService::loginError);

    apiService.loginAsync("testuser");

    // Should emit loginError because TCP client is not set
    QCOMPARE(loginErrorSpy.count(), 1);
    QVERIFY(loginErrorSpy.at(0).at(0).toString().contains("TCP client"));
}

void TestServices::testApiServiceLogoutAsync()
{
    // Test that logoutAsync can be called
    ApiService apiService(nullptr, this);

    // logoutAsync should not crash even without TCP client
    apiService.logoutAsync("test-session-id");

    // No signals expected for logout without TCP client
    QVERIFY(true);
}

void TestServices::testApiServiceGetUserInfoAsync()
{
    // Test that getUserInfoAsync emits userInfoError when tcpClient is nullptr
    ApiService apiService(nullptr, this);

    QSignalSpy userInfoErrorSpy(&apiService, &ApiService::userInfoError);

    apiService.getUserInfoAsync("test-session-id");

    // Should emit userInfoError because TCP client is not set
    QCOMPARE(userInfoErrorSpy.count(), 1);
    QVERIFY(userInfoErrorSpy.at(0).at(0).toString().contains("TCP client"));
}

void TestServices::testApiServiceStartNewGameAsync()
{
    // Test that startNewGameAsync emits gameStartError when tcpClient is nullptr
    ApiService apiService(nullptr, this);

    QSignalSpy gameStartErrorSpy(&apiService, &ApiService::gameStartError);

    apiService.startNewGameAsync("test-session-id");

    // Should emit gameStartError because TCP client is not set
    QCOMPARE(gameStartErrorSpy.count(), 1);
    QVERIFY(gameStartErrorSpy.at(0).at(0).toString().contains("TCP client"));
}

void TestServices::testApiServiceStartNewGameAsyncEmptySessionId()
{
    // Test that startNewGameAsync emits gameStartError for empty session ID
    ApiService apiService(nullptr, this);

    QSignalSpy gameStartErrorSpy(&apiService, &ApiService::gameStartError);

    apiService.startNewGameAsync("");

    // Should emit gameStartError because both conditions fail
    QVERIFY(gameStartErrorSpy.count() >= 1);
}

void TestServices::testApiServiceGetGameInfoAsync()
{
    // Test that getGameInfoAsync emits gameInfoError when tcpClient is nullptr
    ApiService apiService(nullptr, this);

    QSignalSpy gameInfoErrorSpy(&apiService, &ApiService::gameInfoError);

    apiService.getGameInfoAsync(123);

    // Should emit gameInfoError because TCP client is not set
    QCOMPARE(gameInfoErrorSpy.count(), 1);
    QVERIFY(gameInfoErrorSpy.at(0).at(0).toString().contains("TCP client"));
}

void TestServices::testApiServiceHandleWordAsync()
{
    // Test that handleWordAsync emits wordHandleError when tcpClient is nullptr
    ApiService apiService(nullptr, this);

    QSignalSpy wordHandleErrorSpy(&apiService, &ApiService::wordHandleError);

    apiService.handleWordAsync(123, "session-id", "テスト");

    // Should emit wordHandleError because TCP client is not set
    QCOMPARE(wordHandleErrorSpy.count(), 1);
    QVERIFY(wordHandleErrorSpy.at(0).at(0).toString().contains("TCP client"));
}

void TestServices::testApiServiceHandleWordAsyncEmptySessionId()
{
    // Test that handleWordAsync emits wordHandleError for empty session ID
    ApiService apiService(nullptr, this);

    QSignalSpy wordHandleErrorSpy(&apiService, &ApiService::wordHandleError);

    apiService.handleWordAsync(123, "", "テスト");

    // Should emit wordHandleError because session ID is empty
    QVERIFY(wordHandleErrorSpy.count() >= 1);
}

void TestServices::testApiServiceHandleWordAsyncEmptyWord()
{
    // Test that handleWordAsync emits wordHandleError for empty word
    ApiService apiService(nullptr, this);

    QSignalSpy wordHandleErrorSpy(&apiService, &ApiService::wordHandleError);

    apiService.handleWordAsync(123, "session-id", "");

    // Should emit wordHandleError because word is empty
    QVERIFY(wordHandleErrorSpy.count() >= 1);
}

void TestServices::testApiServiceSubscribeOnGameAsync()
{
    // Test that subscribeOnGameAsync emits subscribeError when tcpClient is nullptr
    ApiService apiService(nullptr, this);

    QSignalSpy subscribeErrorSpy(&apiService, &ApiService::subscribeError);

    apiService.subscribeOnGameAsync(123);

    // Should emit subscribeError because TCP client is not set
    QCOMPARE(subscribeErrorSpy.count(), 1);
    QVERIFY(subscribeErrorSpy.at(0).at(0).toString().contains("TCP client"));
}

// ==================== ServerProtocol Tests ====================

void TestServices::testServerProtocolUserInfoParsing()
{
    // Test UserInfo structure
    UserInfo userInfo;
    userInfo.userId = 123;
    userInfo.nickname = "TestUser";
    userInfo.averageWordsPerGame = 5.5;

    QCOMPARE(userInfo.userId, 123ULL);
    QCOMPARE(userInfo.nickname, QString("TestUser"));
    QCOMPARE(userInfo.averageWordsPerGame, 5.5);
}

void TestServices::testServerProtocolGameContextParsing()
{
    // Test GameContext structure
    GameContext gameContext;
    gameContext.gameId = 456;
    gameContext.wordsCount = 10;
    gameContext.playersCount = 2;
    gameContext.adminId = 123;
    gameContext.lastPlayerId = 124;
    gameContext.lastKana = "う";

    QCOMPARE(gameContext.gameId, 456ULL);
    QCOMPARE(gameContext.wordsCount, 10);
    QCOMPARE(gameContext.playersCount, 2);
    QCOMPARE(gameContext.adminId, 123ULL);
    QCOMPARE(gameContext.lastPlayerId, 124ULL);
    QCOMPARE(gameContext.lastKana, QString("う"));
}

void TestServices::testServerProtocolNewWordUpdateParsing()
{
    // Test NewWordUpdate structure
    NewWordUpdate update;
    update.gameId = 456;
    update.kanji = "試";
    update.readings << "し" << "ため";
    update.meanings << "test" << "trial";
    update.partsOfSpeech << "noun";

    QCOMPARE(update.gameId, 456ULL);
    QCOMPARE(update.kanji, QString("試"));
    QCOMPARE(update.readings.length(), 2);
    QCOMPARE(update.meanings.length(), 2);
    QCOMPARE(update.partsOfSpeech.length(), 1);
}

void TestServices::testServerProtocolHandleWordStatusParsing()
{
    // Test HandleWordStatus enum values
    HandleWordStatus status = HandleWordStatus::OK;
    QCOMPARE(static_cast<int>(status), 0);

    status = HandleWordStatus::WRONG_ORDER;
    QCOMPARE(static_cast<int>(status), 2);

    status = HandleWordStatus::GAME_STOPPED;
    QCOMPARE(static_cast<int>(status), 10);
}

void TestServices::testGameEventCreation()
{
    // Test GameStoppedEvent structure
    GameStoppedEvent event;
    event.type = GameEventType::GameStopped;
    event.gameId = 456;
    event.scores.userId = 123;
    event.scores.score = 10;

    QCOMPARE(event.type, GameEventType::GameStopped);
    QCOMPARE(event.gameId, 456ULL);
    QCOMPARE(event.scores.userId, 123ULL);
    QCOMPARE(event.scores.score, 10);
}

QTEST_MAIN(TestServices)
#include "test_services.moc"
