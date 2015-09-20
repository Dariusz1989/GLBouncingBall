#include "bouncingball.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BouncingBall w;

    w.show();

    return a.exec();
}
