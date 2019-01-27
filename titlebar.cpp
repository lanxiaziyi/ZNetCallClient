#include "titlebar.h"
#include <QDebug>
TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));

    setFixedHeight(30);
    setWindowFlags(Qt::FramelessWindowHint);
    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(20, 20);
    m_iconLabel->setScaledContents(true);

    m_titleLabel = new QLabel(this);
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_minimizeButton = new QPushButton(this);
    m_minimizeButton->setFixedSize(27, 22);
    m_minimizeButton->setObjectName("minimizeButton");

    m_maximizeButton = new QPushButton(this);
    m_maximizeButton->setFixedSize(27, 22);
    m_maximizeButton->setObjectName("maximizeButton");

    m_closeButton = new QPushButton(this);
    m_closeButton->setFixedSize(27, 22);
    m_closeButton->setObjectName("closeButton");

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_iconLabel);
    layout->addStretch(1);
    layout->addWidget(m_titleLabel);
    layout->addStretch(1);
    layout->addWidget(m_minimizeButton);
    layout->addWidget(m_maximizeButton);
    layout->addWidget(m_closeButton);
    setLayout(layout);

    setProperty("titleBar", true);
    setObjectName("titleBar");

    connect(m_minimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_maximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_closeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

void TitleBar::setWindowTitle(const QString &title)
{
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setText(title);
}

void TitleBar::SetTitleBarIcon(const QString &iconPath)
{
    QPixmap map(iconPath);
    m_iconLabel->setPixmap(map);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_maximizeButton->click();
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    // 鼠标左键按下事件
    if (event->button() == Qt::LeftButton)
    {
        // 记录鼠标左键状态
        m_leftButtonPressed = true;
        //记录鼠标在屏幕中的位置
        m_start = event->globalPos();
        //qDebug()<<"robin:mouse begin:"<<m_start;

    }

}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    // 持续按住才做对应事件
    if(m_leftButtonPressed)
    {
        //将父窗体移动到父窗体原来的位置加上鼠标移动的位置：event->globalPos()-m_start
        //QPoint t_newPoint = parentWidget()->geometry().topLeft() +
        //        event->globalPos() - m_start;
        //
        QPoint t_newPoint = parentWidget()->pos() + event->globalPos() - m_start;
        //qDebug()<<"robin:mouseMoveEvent:t_newPoint:"<<parentWidget()->pos() <<t_newPoint;

        parentWidget()->move(t_newPoint);
        //将鼠标在屏幕中的位置替换为新的位置
        m_start = event->globalPos();
       //  qDebug()<<"robin:mouseMoveEvent:m_startPoint:"<<m_start;
    }

}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    // 鼠标左键释放
    if (event->button() == Qt::LeftButton)
    {
        // 记录鼠标状态
        m_leftButtonPressed = false;
    }
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
    //设置标题
    case QEvent::WindowTitleChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            m_titleLabel->setText(pWidget->windowTitle());
            return true;
        }
    }
    //设置图标
    case QEvent::WindowIconChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            QIcon icon = pWidget->windowIcon();
            m_iconLabel->setPixmap(icon.pixmap(m_iconLabel->size()));
            return true;
        }
    }
    // 窗口状态变化、窗口大小变化
    case QEvent::WindowStateChange:
    case QEvent::Resize:
        updateMaximize();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void TitleBar::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_maximizeButton->setToolTip(tr("Restore"));
            m_maximizeButton->setProperty("maximizeProperty", "restore");
        }
        else
        {
            m_maximizeButton->setProperty("maximizeProperty", "maximize");
            m_maximizeButton->setToolTip(tr("Maximize"));
        }

        m_maximizeButton->setStyle(QApplication::style());
    }
}

void TitleBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == m_minimizeButton)
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_maximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_closeButton)
        {
            pWindow->close();
        }
    }
}

