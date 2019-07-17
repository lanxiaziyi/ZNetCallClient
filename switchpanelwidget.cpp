#include "switchpanelwidget.h"
#include "ui_switchpanelwidget.h"

SwitchPanelWidget::SwitchPanelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchPanelWidget)
{
    ui->setupUi(this);
}

SwitchPanelWidget::~SwitchPanelWidget()
{
    delete ui;
}
