#ifndef REGISTER_INFO_H
#define REGISTER_INFO_H

#include <QWidget>

namespace Ui {
class register_info;
}

class register_info : public QWidget
{
    Q_OBJECT

public:
    explicit register_info(QWidget *parent = 0);
    ~register_info();

private:
    Ui::register_info *ui;
};

#endif // REGISTER_INFO_H
