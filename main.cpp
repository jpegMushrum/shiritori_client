#include <QApplication>
#include "src/application.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Application w;
    w.show();
    return QCoreApplication::exec();
}
