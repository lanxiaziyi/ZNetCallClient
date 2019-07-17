#include "sipaccountinfo.h"
#include "ui_sipaccountinfo.h"

SipAccountInfo::SipAccountInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SipAccountInfo)
{
    ui->setupUi(this);
}

SipAccountInfo::~SipAccountInfo()
{
    delete ui;
}
