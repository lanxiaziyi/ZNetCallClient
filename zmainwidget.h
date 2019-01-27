#ifndef ZMAINWIDGET_H
#define ZMAINWIDGET_H
#include <QWidget>
#include <QFrame>
#include "titlebar.h"
#include <QVBoxLayout>

class ZMainWidget : public QFrame
{
public:
    ZMainWidget(QWidget *parent = nullptr);
    ~ZMainWidget();
protected:
 //   void paintEvent(QPaintEvent*);
private:
    TitleBar *m_pTitleBar = nullptr;//标题栏
    QWidget *m_pContenWidget = nullptr;//内容组件
    QVBoxLayout *m_pLayout = nullptr;//布局管理器
};

#endif // ZMAINWIDGET_H
