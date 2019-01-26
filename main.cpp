#include "widget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap t_pixmap("://lingEr2.jpeg");
    QSplashScreen splash(t_pixmap);
    splash.show();

    QThread::sleep(3);

    Widget w;
    w.show();

    splash.finish(&w);

    return a.exec();
}
