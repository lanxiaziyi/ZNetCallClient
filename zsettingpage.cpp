#include "zsettingpage.h"
#include "ui_zsettingpage.h"

ZSettingPage::ZSettingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZSettingPage)
{
    ui->setupUi(this);

}

ZSettingPage::~ZSettingPage()
{
    delete ui;
}
