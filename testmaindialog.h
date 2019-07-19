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

private slots:
    void on_pushButton_testRegister_clicked();

    void on_pushButton_testUnRegister_clicked();

private:
    Ui::TestMainDialog *ui;
};

#endif // TESTMAINDIALOG_H
