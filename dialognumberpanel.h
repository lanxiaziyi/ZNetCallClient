#ifndef DIALOGNUMBERPANEL_H
#define DIALOGNUMBERPANEL_H

#include <QWidget>

namespace Ui {
class DialogNumberPanel;
}

class DialogNumberPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DialogNumberPanel(QWidget *parent = nullptr);
    ~DialogNumberPanel();

private:
    Ui::DialogNumberPanel *ui;
};

#endif // DIALOGNUMBERPANEL_H
