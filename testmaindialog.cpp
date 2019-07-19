#include "testmaindialog.h"
#include "ui_testmaindialog.h"
#include <QDebug>

TestMainDialog::TestMainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestMainDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_testFunc,&QPushButton::clicked,this,&TestMainDialog::slotToRunTestFunc);

}

TestMainDialog::~TestMainDialog()
{
    delete ui;
}

void TestMainDialog::toRegisterAccount()
{
    qDebug()<<"robin:not finished";

}

void TestMainDialog::slotToRunTestFunc()
{
    qDebug()<<"robin:run TestMainDialog::slotToRunTestFunc";
    CMultiSipManager::GetInstance()->testFunc();


}
