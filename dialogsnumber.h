#ifndef DIALOGSNUMBER_H
#define DIALOGSNUMBER_H

#include <QWidget>

namespace Ui {
class DialogsNumber;
}

class DialogsNumber : public QWidget
{
    Q_OBJECT

public:
    explicit DialogsNumber(QWidget *parent = nullptr);
    ~DialogsNumber();

private:
    Ui::DialogsNumber *ui;
};

#endif // DIALOGSNUMBER_H
