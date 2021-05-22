#include "paint.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Paint w;
    w.setFixedSize(400,400);
    w.show();

    return a.exec();
}
