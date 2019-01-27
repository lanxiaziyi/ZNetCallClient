#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QPoint>
#include <QPixmap>
#include <QString>

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);

    /**
     * @brief 设置标题栏标题
     * @param title，参数，设置的标题
     */
    void setWindowTitle(const QString& title);
    /**
     * @brief 设置标题栏的图标
     * @param iconPath，参数，图标的路径
     */
    void SetTitleBarIcon(const QString& iconPath);
protected:
    /**
     * @brief 鼠标双击事件处理函数
     * @param event，参数，事件
     * @note 双击标题栏进行界面的最大化/还原
     */
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    /**
     * @brief 鼠标按下事件处理函数
     * @param event，参数，事件
     * @note 按下鼠标左键
     */
    virtual void mousePressEvent(QMouseEvent *event);
    /**
     * @brief 鼠标移动事件处理函数
     * @param event，参数，事件
     * @note 移动鼠标
     */
    virtual void mouseMoveEvent(QMouseEvent *event);
    /**
     * @brief 鼠标释放事件处理函数
     * @param event，参数，事件
     * @note 释放鼠标
     */
    virtual void mouseReleaseEvent(QMouseEvent *event);

    /**
     * @brief 事件过滤处理器
     * @param obj，参数
     * @param event，参数，事件
     * @return 成功返回true，失败返回false
     * @note 设置标题、图标
     */
    virtual bool eventFilter(QObject *obj, QEvent *event);

    /**
     * @brief 最大化/还原
     */
    void updateMaximize();

protected slots:
    /**
     * @brief 最小化、最大化/还原、关闭按钮点击时响应的槽函数
     */
    void onClicked();

    void paintEvent(QPaintEvent*);
private:
    QLabel* m_iconLabel;
    QLabel* m_titleLabel;
    QPushButton* m_minimizeButton;
    QPushButton* m_maximizeButton;
    QPushButton* m_closeButton;
    QPoint m_start;//起始点
    QPoint m_end;//结束点
    bool m_leftButtonPressed;//鼠标左键按下标记


signals:

public slots:
};

#endif // TITLEBAR_H
