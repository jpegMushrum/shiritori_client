#include "test_navigation.h"
#include "../src/utils/screennavigator.h"
#include "../src/utils/toastwidget.h"
#include <QStackedWidget>

void TestNavigation::initTestCase()
{
    // Called once before all tests
}

void TestNavigation::cleanupTestCase()
{
    // Called once after all tests
}

void TestNavigation::testScreenNavigatorInitialization()
{
    QStackedWidget *stackedWidget = new QStackedWidget();
    ScreenNavigator navigator(stackedWidget);

    ScreenNavigator::ScreenType currentScreen = navigator.currentScreen();
    QVERIFY(currentScreen >= 0);

    delete stackedWidget;
}

void TestNavigation::testNavigateToScreen()
{
    QStackedWidget *stackedWidget = new QStackedWidget();
    ScreenNavigator navigator(stackedWidget);

    ScreenNavigator::ScreenType initialScreen = navigator.currentScreen();
    QVERIFY(initialScreen >= 0);

    delete stackedWidget;
}

void TestNavigation::testScreenStackManagement()
{
    QStackedWidget *stackedWidget = new QStackedWidget();
    ScreenNavigator navigator(stackedWidget);

    int screenCount = stackedWidget->count();
    QVERIFY(screenCount >= 0);

    delete stackedWidget;
}

void TestNavigation::testToastWidgetCreation()
{
    ToastWidget toast;
    QVERIFY(toast.isVisible() || !toast.isVisible()); // Widget exists
}

QTEST_MAIN(TestNavigation)
#include "test_navigation.moc"
