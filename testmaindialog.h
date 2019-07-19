#ifndef TESTMAINDIALOG_H
#define TESTMAINDIALOG_H

#include <QDialog>
#include "cmultisipmanager.h"


namespace Ui {
class TestMainDialog;
}

class TestMainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestMainDialog(QWidget *parent = nullptr);
    ~TestMainDialog();

public slots:
    //void add
    void toRegisterAccount();

    void slotToRunTestFunc();

private:
    Ui::TestMainDialog *ui;
};

#endif // TESTMAINDIALOG_H
