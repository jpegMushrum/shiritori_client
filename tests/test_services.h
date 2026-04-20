#ifndef TEST_SERVICES_H
#define TEST_SERVICES_H

#include <QObject>
#include <QtTest>
#include <QSignalSpy>

class ApiService;
class TcpClient;

class TestServices : public QObject
{
    Q_OBJECT

public:
    explicit TestServices();

private slots:
    void initTestCase();
    void cleanupTestCase();

    // ApiService tests - Authentication
    void testApiServiceLoginAsync();
    void testApiServiceLogoutAsync();
    void testApiServiceGetUserInfoAsync();

    // ApiService tests - Game Management
    void testApiServiceStartNewGameAsync();
    void testApiServiceStartNewGameAsyncEmptySessionId();
    void testApiServiceGetGameInfoAsync();
    void testApiServiceHandleWordAsync();
    void testApiServiceHandleWordAsyncEmptySessionId();
    void testApiServiceHandleWordAsyncEmptyWord();

    // ApiService tests - Subscription
    void testApiServiceSubscribeOnGameAsync();

    // ServerProtocol tests
    void testServerProtocolUserInfoParsing();
    void testServerProtocolGameContextParsing();
    void testServerProtocolNewWordUpdateParsing();
    void testServerProtocolHandleWordStatusParsing();
    void testGameEventCreation();

private:
    ApiService *m_apiService;
    TcpClient *m_tcpClient;
};

#endif // TEST_SERVICES_H
