#ifndef CALLINGDIALOGNUMBERPANEL_H
#define CALLINGDIALOGNUMBERPANEL_H

#include <QWidget>

namespace Ui {
class CallIngDialogNumberPanel;
}

class CallIngDialogNumberPanel : public QWidget
{
    Q_OBJECT

public:
    explicit CallIngDialogNumberPanel(QWidget *parent = nullptr);
    ~CallIngDialogNumberPanel();

private:
    Ui::CallIngDialogNumberPanel *ui;
};

#endif // CALLINGDIALOGNUMBERPANEL_H
