#ifndef TEST_NETWORK_H
#define TEST_NETWORK_H

#include <QObject>
#include <QtTest>

class TestNetwork : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // TcpClient tests
    void testTcpClientCreation();

    // ApiService tests
    void testApiServiceInitialization();
    void testApiServiceLogin();
};

#endif // TEST_NETWORK_H
