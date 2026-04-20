#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <QObject>
#include <QtTest>

class TestUtils : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // AppState tests
    void testAppStateSessionId();
    void testAppStateUsername();
    void testAppStateServerAddress();
    void testAppStateLoginLogout();
    void testAppStateSingleton();
};

#endif // TEST_UTILS_H
