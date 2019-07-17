#ifndef SIPACCOUNTINFO_H
#define SIPACCOUNTINFO_H

#include <QWidget>

namespace Ui {
class SipAccountInfo;
}

class SipAccountInfo : public QWidget
{
    Q_OBJECT

public:
    explicit SipAccountInfo(QWidget *parent = nullptr);
    ~SipAccountInfo();

private:
    Ui::SipAccountInfo *ui;
};

#endif // SIPACCOUNTINFO_H
