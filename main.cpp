#include "widget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QThread>
#include <QDebug>

#include "zmainwidget.h"
#include "testmaindialog.h"

/*
 * 软电话重写
 *
*/


int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);

    if(argc > 1)
    {
        for (int i = 1;i < argc;i++)
        {
            qDebug()<<"rboin:app argvs:"<< i << "=:"<< argv[i];
        }
    }

    QPixmap t_pixmap("://skin/loading.jpg");
    QSplashScreen splash(t_pixmap);
    splash.show();

   // QThread::sleep(3);

    //Widget w;
    //w.show();

   // ZMainWidget w;
   // w.show();

    TestMainDialog t_dialog;
    t_dialog.show();


    splash.finish(&t_dialog);

    return a.exec();
}
