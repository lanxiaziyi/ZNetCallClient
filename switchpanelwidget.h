#ifndef SWITCHPANELWIDGET_H
#define SWITCHPANELWIDGET_H

#include <QWidget>

namespace Ui {
class SwitchPanelWidget;
}

class SwitchPanelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchPanelWidget(QWidget *parent = nullptr);
    ~SwitchPanelWidget();

private:
    Ui::SwitchPanelWidget *ui;
};

#endif // SWITCHPANELWIDGET_H
