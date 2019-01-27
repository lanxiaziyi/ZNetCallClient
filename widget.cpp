#include "widget.h"
#include "ui_widget.h"
#include "zsettingpage.h"
#include <QHBoxLayout>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
#ifdef Q_OS_WIN
    this->setWindowFlag(Qt::SplashScreen);
#endif
//    ZSettingPage *t_pPage = new ZSettingPage(this);
//    //t_pPage->resize(100,1200);
//    //t_pPage->setGeometry(QRect(0,0,100,1200));
//    t_pPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    t_pPage->setMinimumHeight(1200);
//    t_pPage->setMinimumWidth(600);
     //QScrollArea *const scroll(new QScrollArea);
   // QHBoxLayout *const t_layout(new QHBoxLayout(ui->scrollArea));
    //t_layout->addWidget(t_pPage);

    //ui->scrollArea->setWidget(t_pPage);
    //QSize t_size =  t_pPage->size();


    int t_i = 0;

}

Widget::~Widget()
{
    delete ui;
}
