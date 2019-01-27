#include "zmainwidget.h"

ZMainWidget::ZMainWidget(QWidget *parent)
        :QFrame(parent)

{
   // setWindowFlags(Qt::Window|Qt::FramelessWindowHint |Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);//去掉标题栏，边框保留
    setWindowFlags(Qt::CustomizeWindowHint);

    this->setFrameShadow(QFrame::Sunken);

    m_pTitleBar = new TitleBar(this);
    m_pContenWidget = new QWidget(this);
    m_pContenWidget->setObjectName("Contents");
    m_pContenWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));

    m_pLayout = new QVBoxLayout(this);
    m_pLayout->addWidget(m_pTitleBar);
    m_pLayout->addWidget(m_pContenWidget);
    m_pLayout->setSpacing(0);
    m_pLayout->setContentsMargins(0,0,0,0);
    this->setLayout(m_pLayout);

    this->resize(300,600);
}

ZMainWidget::~ZMainWidget()
{

}
