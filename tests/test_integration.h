#ifndef TEST_INTEGRATION_H
#define TEST_INTEGRATION_H

#include <QObject>
#include <QtTest>

class TestIntegration : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Integration tests
    void testGameLifecycle();
    void testUserLoginLogoutCycle();
    void testGameCreationAndStart();
    void testScoreCalculation();
    void testConnectionReconnection();
    void testMultiplePlayersInteraction();
};

#endif // TEST_INTEGRATION_H
