#include "dialogsnumber.h"
#include "ui_dialogsnumber.h"

DialogsNumber::DialogsNumber(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogsNumber)
{
    ui->setupUi(this);
}

DialogsNumber::~DialogsNumber()
{
    delete ui;
}
