#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "register_info.h"
#include "uidemo08.h"
#include "package_process.h"
#include <QDesktopWidget>
namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private slots:

    void on_offButton_clicked();

private:
    Ui::login *ui;
    UIDemo08 *demo;
    register_info *reg;
};

#endif // LOGIN_H
