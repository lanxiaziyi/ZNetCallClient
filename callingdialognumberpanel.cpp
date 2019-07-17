#include "callingdialognumberpanel.h"
#include "ui_callingdialognumberpanel.h"

CallIngDialogNumberPanel::CallIngDialogNumberPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallIngDialogNumberPanel)
{
    ui->setupUi(this);
}

CallIngDialogNumberPanel::~CallIngDialogNumberPanel()
{
    delete ui;
}
