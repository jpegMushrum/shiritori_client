#ifndef TEST_NAVIGATION_H
#define TEST_NAVIGATION_H

#include <QObject>
#include <QtTest>

class TestNavigation : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // ScreenNavigator tests
    void testScreenNavigatorInitialization();
    void testNavigateToScreen();
    void testScreenStackManagement();

    // Toast widget tests
    void testToastWidgetCreation();
};

#endif // TEST_NAVIGATION_H
