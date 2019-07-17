#include "zmainwidget.h"
#include <QStyleOption>
#include <QPainter>

ZMainWidget::ZMainWidget(QWidget *parent)
        :QFrame(parent)

{

#ifdef Q_OS_MAC

    this->setWindowFlags(Qt::CustomizeWindowHint);

#elif defined Q_OS_WIN
    // setWindowFlags(Qt::Window|Qt::FramelessWindowHint |Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);//去掉标题栏，边框保留
     setWindowFlags(Qt::CustomizeWindowHint);
   // setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowTitleHint);
   // setWindowFlags(Qt::FramelessWindowHint | windowFlags());
     //this->setFrameShadow(QFrame::Plain);
#endif

    m_pTitleBar = new TitleBar(this);
    m_pContenWidget = new QWidget(this);
    m_pContenWidget->setObjectName("Contents");
    m_pContenWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));

    m_pSipAccountInfoUi = new SipAccountInfo(this);
    m_pSipAccountInfoUi->setFixedSize(400,100);

    m_pSwitchPanelWidget = new SwitchPanelWidget(this);
    m_pSwitchPanelWidget->setFixedSize(400,50);


    m_pLayout = new QVBoxLayout(this);
    m_pLayout->addWidget(m_pTitleBar);
    m_pLayout->addWidget(m_pSipAccountInfoUi);
    m_pLayout->addWidget(m_pSwitchPanelWidget);
    m_pLayout->addWidget(m_pContenWidget);
    m_pLayout->setSpacing(0);
    m_pLayout->setContentsMargins(0,0,0,0);



    this->setLayout(m_pLayout);
    this->resize(300,600);

    m_pTitleBar->SetTitleBarIcon(":/skin/pc/app_icon.png");
    m_pTitleBar->setWindowTitle(tr("Enterprise Communication"));

    //setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
   // this->setStyleSheet(QString::fromUtf8("border:1px solid green"));
   // this->setStyleSheet("QFrame{border: 0px;}");
}

ZMainWidget::~ZMainWidget()
{

}

//void ZMainWidget::paintEvent(QPaintEvent *)
//{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}
