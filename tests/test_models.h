#ifndef TEST_MODELS_H
#define TEST_MODELS_H

#include <QObject>
#include <QtTest>

class TestModels : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // User model tests
    void testUserCreation();
    void testUserProperties();
    void testUserEquality();

    // Game model tests
    void testGameCreation();
    void testGameProperties();
    void testGameValidation();
};

#endif // TEST_MODELS_H
