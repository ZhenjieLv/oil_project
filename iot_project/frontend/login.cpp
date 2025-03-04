#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include "user.h"
#include <QMessageBox>
login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
      ui->setupUi(this);
      QDesktopWidget *desktop = QApplication::desktop();
      move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
      connect(ui->onButton,&QPushButton::clicked,this,[=](){
      QString user;
      QString pwd;
      size_t i = 0;
      std::vector<user_t> ret;
      ret = search_user_info();
      user = ui->IDlineEdit->text();
      pwd = ui->pwdlineEdit->text();
      if(user=="")
          QMessageBox::warning(this,"警告","用户名不能为空！");
      else if(pwd=="") QMessageBox::warning(this,"警告","密码不能为空！");
                  else {
          for(i=0;i<ret.size();i++)
          {
              if(QString::number(ret[i].user_id)==user&&ret[i].password==pwd)
              {
                  demo = new UIDemo08;
                  demo ->show();
                  start_thread();
                  this -> close();       
                  break;
              }
            }
            if(i==ret.size())
              {
                QMessageBox::information(this,"登录","用户名或密码错误！");
                ui->IDlineEdit->clear();
                ui->pwdlineEdit->clear();
                ui->IDlineEdit->setFocus();
                }
      }
  });
//  connect(ui->offButton,&QPushButton::clicked,this,[=](){
//    reg = new register_info;
//    reg->show();
//  });
}

login::~login()
{
    delete ui;
}


void login::on_offButton_clicked()
{
    this->close();
}
