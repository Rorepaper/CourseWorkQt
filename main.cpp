#include <QApplication>
#include <QTranslator>
#include "animation.h"
#include "name.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QIcon(":/resources/pics/oilPlatform.png"));
    Name name;

    Animation an;
    // Здесь окно не показывается. Оно откроется в конструкторе класса
    QObject::connect(&an, &Animation::signalRequestQuit, &a, &QApplication::quit);
    return QApplication::exec();
}
