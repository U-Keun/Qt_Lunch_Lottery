#include <QApplication>
#include "mainwidget.h"
#include "imagewidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWidget program;
    program.show();

    return QApplication::exec();
}
