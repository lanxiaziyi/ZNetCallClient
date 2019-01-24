#ifndef ZSETTINGPAGE_H
#define ZSETTINGPAGE_H

#include <QWidget>

namespace Ui {
class ZSettingPage;
}

class ZSettingPage : public QWidget
{
    Q_OBJECT

public:
    explicit ZSettingPage(QWidget *parent = nullptr);
    ~ZSettingPage();

private:
    Ui::ZSettingPage *ui;
};

#endif // ZSETTINGPAGE_H
