#include "testmaindialog.h"
#include "ui_testmaindialog.h"
#include <QDebug>

TestMainDialog::TestMainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestMainDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_testFunc,&QPushButton::clicked,this,&TestMainDialog::slotToRunTestFunc);
    CMultiSipManager::GetInstance();

    connect(CMultiSipManager::GetInstance(),&CMultiSipManager::sigIncomingInfo,ui->textEdit_incomingCallInfo,&QTextEdit::append);
    connect(CMultiSipManager::GetInstance(),&CMultiSipManager::sigSipInfo,ui->textEdit_sipInfo,&QTextEdit::append);
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
   // CMultiSipManager::GetInstance()->testFunc();

    CMultiSipManager::GetInstance()->resetAudioCodecPriority();
}

void TestMainDialog::on_pushButton_testRegister_clicked()
{
    CMultiSipManager::GetInstance()->testRegisterAccount();
}

void TestMainDialog::on_pushButton_testUnRegister_clicked()
{
    CMultiSipManager::GetInstance()->testUnRegisterAccount();
}

void TestMainDialog::on_pushButton_hangupCall_clicked()
{
    int  t_iCallId = ui->lineEdit_callId->text().toInt();

    CMultiSipManager::GetInstance()->toHangUpCall(t_iCallId);
}

void TestMainDialog::on_pushButton_answerCall_clicked()
{
    int  t_iCallId = ui->lineEdit_callId->text().toInt();

    CMultiSipManager::GetInstance()->toAnswerCall(t_iCallId);
}

void TestMainDialog::on_pushButton_callOut_clicked()
{
    QString t_accountId = ui->lineEdit_callOutAccountId->text();
    QString t_callOutNumber = ui->lineEdit_callOutNumber->text();

    QMap<QString,QString> t_customMapHeader;
    t_customMapHeader.insert("X-CallHead","1656339602313745");
    CMultiSipManager::GetInstance()->toMakeACall(t_accountId.toInt(),t_callOutNumber,t_customMapHeader);
}
