#include "start_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    start_widget w;
    w.show();
    w.setWindowTitle("中国象棋");

    return a.exec();
}
