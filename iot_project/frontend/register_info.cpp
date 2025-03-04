#include "register_info.h"
#include "ui_register_info.h"
#include <QMessageBox>
#include <QByteArray>
#include "user.h"
register_info::register_info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::register_info)
{
    ui->setupUi(this);
    connect(ui->onButton,&QPushButton::clicked,this,[=](){
        QString id = ui->IDlineEdit ->text();
        QString name = ui->n_lineEdit ->text();
        QString depart = ui->dp_lineEdit ->text();
        QString phone = ui->ph_lineEdit ->text();
        QString password = ui->pwd_lineEdit ->text();
        QString ag_pwd = ui->agpwd_lineEdit ->text();
        string sname = name.toStdString();
        string sdepart = depart.toStdString();
        string sphone = phone.toStdString();
        string spassword = password.toStdString();
        user_t users={0};
        users.user_id= id.toInt();
        memcpy(users.name, sname.c_str(), sname.size());
        memcpy(users.depart, sdepart.c_str(), sdepart.size());
        memcpy(users.phone, sphone.c_str(),sphone.size());
        memcpy(users.password, spassword.c_str(), spassword.size());
       if(password == ag_pwd)
       {
          bool ret = insert_user_info(users);
          if(ret){
            QMessageBox::information(this,"提示","注册成功！");
            this->close();
          }
          else
          {
             QMessageBox::warning(this,"警告","注册失败，请重新注册！");
             ui->IDlineEdit->clear();
             ui->n_lineEdit->clear();
             ui->dp_lineEdit->clear();
             ui->ph_lineEdit->clear();
             ui->pwd_lineEdit->clear();
             ui->agpwd_lineEdit->clear();
             ui->IDlineEdit->setFocus();
          }
       }
       else
       { 
          QMessageBox::information(this,"提示","核对确认密码！");
          ui->pwd_lineEdit->clear();
          ui->agpwd_lineEdit->clear();
          ui->pwd_lineEdit->setFocus();
       }
    });
    connect(ui->offButton,&QPushButton::clicked,this,[=](){
        this->close();
    });
}

register_info::~register_info()
{
    delete ui;
}
