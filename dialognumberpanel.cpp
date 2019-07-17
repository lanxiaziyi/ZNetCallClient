#include "dialognumberpanel.h"
#include "ui_dialognumberpanel.h"

DialogNumberPanel::DialogNumberPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogNumberPanel)
{
    ui->setupUi(this);
}

DialogNumberPanel::~DialogNumberPanel()
{
    delete ui;
}
