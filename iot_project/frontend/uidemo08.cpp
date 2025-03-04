#pragma execution_character_set("utf-8")

#include "uidemo08.h"
#include "ui_uidemo08.h"
#include "iconhelper.h"
using namespace std;

#define LOG_FILE_MSG "[frontend uidemo08.cpp]: "
UIDemo08::UIDemo08(QWidget *parent) : QWidget(parent),
                                      ui(new Ui::UIDemo08)
{
  ui->setupUi(this);
  // start_backend_thread();
  logger = spdlog::get("project_log");
  QDesktopWidget *desktop = QApplication::desktop();
  move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
  this->initForm();
  this->initLeftMain();
  this->initLeftConfig();
}

UIDemo08::~UIDemo08()
{
  delete ui;
}
void UIDemo08::initForm()
{
  this->setProperty("form", true);
  this->setProperty("canMove", true);
  this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

  IconHelper::Instance()->setIcon(ui->labIco, QChar(0xf073), 30);
  IconHelper::Instance()->setIcon(ui->btnMenu_Min, QChar(0xf068));
  IconHelper::Instance()->setIcon(ui->btnMenu_Max, QChar(0xf067));
  IconHelper::Instance()->setIcon(ui->btnMenu_Close, QChar(0xf00d));

  // ui->widgetMenu->setVisible(false);
  ui->widgetTitle->setProperty("form", "title");
  ui->widgetTop->setProperty("nav", "top");
  ui->labTitle->setText("管理平台");
  ui->labTitle->setFont(QFont("Microsoft Yahei", 20));
  // ui->label->setFont(QFont("Microsoft Yahei", 11));
  this->setWindowTitle(ui->labTitle->text());

  // ui->stackedWidget->setStyleSheet("QLabel{font:60pt;}");
  ui->stackedWidget->setStyleSheet("QLabel{font:11pt;}");
  QSize icoSize(32, 32);
  int icoWidth = 85;

  // 设置顶部导航按钮
  QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
  foreach (QToolButton *btn, tbtns)
  {
    btn->setIconSize(icoSize);
    btn->setMinimumWidth(icoWidth);
    btn->setCheckable(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(buttonClick()));
  }

  ui->btnMain->click();

  ui->widgetLeftMain->setProperty("flag", "left");
  ui->widgetLeftConfig->setProperty("flag", "left");
  ui->page1->setStyleSheet(QString("QWidget[flag=\"left\"] QAbstractButton{min-height:%1px;max-height:%1px;}").arg(60));
  ui->page2->setStyleSheet(QString("QWidget[flag=\"left\"] QAbstractButton{min-height:%1px;max-height:%1px;}").arg(60));
}

void UIDemo08::buttonClick()
{
  QToolButton *b = (QToolButton *)sender();
  QString name = b->text();

  QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
  foreach (QToolButton *btn, tbtns)
  {
    if (btn == b)
    {
      btn->setChecked(true);
    }
    else
    {
      btn->setChecked(false);
    }
  }

  if (name == "管理界面")
  {
    ui->stackedWidget->setCurrentIndex(0);
  }
  else if (name == "查询界面")
  {
    ui->stackedWidget->setCurrentIndex(1);
  }
}

void UIDemo08::initLeftMain()
{
  pixCharMain << 0xf030 << 0xf03e << 0xf247;
  btnsMain << ui->tbtnMain1 << ui->tbtnMain2;

  int count = btnsMain.count();
  for (int i = 0; i < count; i++)
  {
    btnsMain.at(i)->setCheckable(true);
    btnsMain.at(i)->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnsMain.at(i), SIGNAL(clicked(bool)), this, SLOT(leftMainClick()));
  }

  IconHelper::Instance()->setStyle(ui->widgetLeftMain, btnsMain, pixCharMain, 15, 35, 25, "left", 4);

  ui->tbtnMain1->click();
}

void UIDemo08::initLeftConfig()
{
  pixCharConfig << 0xf031 << 0xf036 << 0xf249 << 0xf055 << 0xf05a << 0xf249;
  btnsConfig << ui->tbtnConfig1 << ui->tbtnConfig2;

  int count = btnsConfig.count();
  for (int i = 0; i < count; i++)
  {
    btnsConfig.at(i)->setCheckable(true);
    btnsConfig.at(i)->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(btnsConfig.at(i), SIGNAL(clicked(bool)), this, SLOT(leftConfigClick()));
  }

  IconHelper::Instance()->setStyle(ui->widgetLeftConfig, btnsConfig, pixCharConfig, 10, 20, 15, "left", 5);

  ui->tbtnConfig1->click();
}

void UIDemo08::leftMainClick()
{
  QToolButton *b = (QToolButton *)sender();
  QString name = b->text();

  int count = btnsMain.count();
  for (int i = 0; i < count; i++)
  {
    if (btnsMain.at(i) == b)
    {
      btnsMain.at(i)->setChecked(true);
      btnsMain.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btnsMain.at(i), false)));
    }
    else
    {
      btnsMain.at(i)->setChecked(false);
      btnsMain.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btnsMain.at(i), true)));
    }
  }
  if (name == "管理井")
  {
    ui->stackedWidget_2->setCurrentIndex(0);
  }
  else if (name == "管理工作筒")
  {
    ui->stackedWidget_2->setCurrentIndex(1);
  }
  // ui->lab1->setText(name);
}

void UIDemo08::leftConfigClick()
{
  QToolButton *b = (QToolButton *)sender();
  QString name = b->text();

  int count = btnsConfig.count();
  for (int i = 0; i < count; i++)
  {
    if (btnsConfig.at(i) == b)
    {
      btnsConfig.at(i)->setChecked(true);
      btnsConfig.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btnsConfig.at(i), false)));
    }
    else
    {
      btnsConfig.at(i)->setChecked(false);
      btnsConfig.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btnsConfig.at(i), true)));
    }
  }
  if (name == "当前数据")
  {
    ui->stackedWidget_3->setCurrentIndex(0);
  }
  else if (name == "历史数据")
  {
    ui->stackedWidget_3->setCurrentIndex(1);
  }
}

void UIDemo08::on_btnMenu_Min_clicked()
{
  showMinimized();
}

void UIDemo08::on_btnMenu_Max_clicked()
{
  static bool max = false;
  static QRect location = this->geometry();
  if (max)
  {
    this->setGeometry(location);
  }
  else
  {
    location = this->geometry();
    this->setGeometry(qApp->desktop()->availableGeometry());
  }

  this->setProperty("canMove", max);
  max = !max;
}

void UIDemo08::on_btnMenu_Close_clicked()
{
  QCloseEvent *event;
  int r = QMessageBox::question(this, tr("退出程序"), tr("是否需要退出程序？"), QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape);
  if (r == QMessageBox::Yes)
  {
    this->setVisible(false);
    stop_thread();
    event->accept();
  }
  else
  {
    event->ignore();
  }
}
void UIDemo08::onTimeOut()
{
  if (recv_package.find(rec_num) != recv_package.end())
  {
    char temp[500];
    memset(temp, '\0', sizeof(temp));
    string str;
    shared_ptr<Package> package = recv_package.at(rec_num);
    if (ui->comboBox_6->currentIndex() == 0)
    {
      if (int(AV_RL16(package->get_data())) == 0)
        str = "断电";
      else
        str = "上电";
      sprintf(temp, "当前井%s号%s层%s的查询信息为:%s", ui->comboBox_4->currentText().toStdString().data(), ui->comboBox_5->currentText().toStdString().data(), ui->comboBox_6->currentText().toStdString().data(), str.c_str());
      QMessageBox::information(this, "查询信息", temp);
    }
    if (ui->comboBox_6->currentIndex() == 1)
    {
      sprintf(temp, "当前井%s号%s层%s的查询信息为:%f", ui->comboBox_4->currentText().toStdString().data(), ui->comboBox_5->currentText().toStdString().data(), ui->comboBox_6->currentText().toStdString().data(), AV_RB16(package->get_data())/10.0);
      QMessageBox::information(this, "查询信息", temp);
    }
    if (ui->comboBox_6->currentIndex() == 2)
    {
      sprintf(temp, "当前井%s号%s层%s的查询信息为:%f", ui->comboBox_4->currentText().toStdString().data(), ui->comboBox_5->currentText().toStdString().data(), ui->comboBox_6->currentText().toStdString().data(), AV_RB16(package->get_data())/10.0);
      QMessageBox::information(this, "查询信息", temp);
    }
    selnum = 0;
    recv_package.erase(rec_num);
    tim->stop();
  }
  else
  {
    int msgid;
    struct my_msg_st data;
    char buf[BUFSIZ];
    msgid = msgget((key_t)FRONTEND_SEND_TO_BACKEND_MSG_KEY, 0664 | IPC_CREAT);
    if (msgid == -1)
    {
      logger->error("{}获取前端发送后端消息队列失败！", LOG_FILE_MSG);
    }
    else
    {
      if (selnum <= 0)
      {
        shared_ptr<Package> package = sent_package.at(rec_num);
        data.my_msg_type = rand() % 3 + 1;
        memcpy(data.anytext, package->get_message(), package->get_length());
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
          logger->error("{}从前端向后端发送消息失败！", LOG_FILE_MSG);
        }
        else
        {
          selnum++;
        }
      }
      else
      {
        char tmp[500] = {0};
        memset(tmp, '\0', sizeof(tmp));
        selnum = 0;
        QMessageBox::warning(this, "警告", "当前网络状态不好,请稍后再试！");
        shared_ptr<Package> package = sent_package.at(rec_num);
        sprintf(tmp, "设备井%s号%s层(%d)连接状态不好，无法收到回应！", ui->comboBox_4->currentText().toStdString().data(), ui->comboBox_5->currentText().toStdString().data(), package->get_dest_id());
        shared_ptr<Package> alarm_package = send_alarm_msg(package, tmp);
        sent_package.erase(rec_num);
        data.my_msg_type = rand() % 3 + 1;
        memcpy(data.anytext, alarm_package->get_message(), alarm_package->get_length());
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
          logger->error("{}从前端向后端发送查询报警消息失败！", LOG_FILE_MSG);
        }
        shared_ptr<Package> pkg(new Package(0, CHANGE_MODE));
        pkg->set_dest_id(0);
        pkg->set_sequence_number(0);
        pkg->set_commu_type(false);
        data.my_msg_type = rand() % 3 + 1;
        memcpy(data.anytext, pkg->get_message(), pkg->get_length());
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
          logger->error("{}从前端向后端发送通信切换方式消息失败！", LOG_FILE_MSG);
        }
        tim->stop();
    }
      }
    }
  }

void UIDemo08::onTimeOut_mandrel()
{
  if (recv_package.find(rec_num) != recv_package.end())
  {
    QMessageBox::information(this, "修改", "修改成功！");
    selnum = 0;
    recv_package.erase(rec_num);
    tim->stop();
  }
  else
  {
    int msgid;
    struct my_msg_st data;
    char buf[BUFSIZ];
    msgid = msgget((key_t)FRONTEND_SEND_TO_BACKEND_MSG_KEY, 0664 | IPC_CREAT);
    if (msgid == -1)
    {
      logger->error("{}获取前端发送后端消息队列失败！", LOG_FILE_MSG);
    }
    else
    {
      if (selnum <= 0)
      {
        shared_ptr<Package> package = sent_package.at(rec_num);
        data.my_msg_type = rand() % 3 + 1;
        memcpy(data.anytext, package->get_message(), package->get_length());
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
          logger->error("{}从前端向后端发送消息失败！", LOG_FILE_MSG);
        }
        else
        {
          selnum++;
        }
      }
      else
      {
        char tmp[500] = {0};
        memset(tmp, '\0', sizeof(tmp));
        selnum = 0;
        QMessageBox::warning(this, "警告", "当前网络状态不好,请稍后再试！");
        shared_ptr<Package> package = sent_package.at(rec_num);
        sprintf(tmp, "设备工作筒第%s层(%d)连接状态不好，无法收到回应！", ui->comboBox_3->currentText().toStdString().data(), package->get_dest_id());
        shared_ptr<Package> alarm_package = send_alarm_msg(package, tmp);
        sent_package.erase(rec_num);
        data.my_msg_type = rand() % 3 + 1;
        memcpy(data.anytext, alarm_package->get_message(), alarm_package->get_length());
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
          logger->error("{}从前端向后端发送工作筒报警消息失败！", LOG_FILE_MSG);
        }
        shared_ptr<Package> pkg(new Package(0, CHANGE_MODE));
        pkg->set_dest_id(0);
        pkg->set_sequence_number(0);
        pkg->set_commu_type(false);
        data.my_msg_type = rand() % 3 + 1;
        memcpy(data.anytext, pkg->get_message(), pkg->get_length());
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
          logger->error("{}从前端向后端发送通信切换方式消息失败！", LOG_FILE_MSG);
        }
        tim->stop();
      }
    }
  }
}
void UIDemo08::onTimeOut_well()
{
  if (recv_package.find(rec_num) != recv_package.end())
  {
    QMessageBox::information(this, "修改", "修改成功！");
    selnum = 0;
    recv_package.erase(rec_num);
    tim->stop();
  }
  else
  {
    int msgid;
    struct my_msg_st data;
    char buf[BUFSIZ];
    msgid = msgget((key_t)FRONTEND_SEND_TO_BACKEND_MSG_KEY, 0664 | IPC_CREAT);
    if (msgid == -1)
    {
      logger->error("{}获取前端发送后端消息队列失败！", LOG_FILE_MSG);
    }
    else
    {
      if (selnum <= 0)
      {
        shared_ptr<Package> package = sent_package.at(rec_num);
        data.my_msg_type = rand() % 3 + 1;
        memcpy(data.anytext, package->get_message(), package->get_length());
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
          logger->error("{}从前端向后端发送消息失败！", LOG_FILE_MSG);
        }
        else
        {
          selnum++;
        }
      }
      else
      {
        char tmp[500] = {0};
        memset(tmp, '\0', sizeof(tmp));
        selnum = 0;
        QMessageBox::warning(this, "警告", "当前网络状态不好,请稍后再试！");
        // shared_ptr<Package> pkg(new Package(0, CHANGE_MODE));
        // pkg->set_dest_id(2);
        shared_ptr<Package> package = sent_package.at(rec_num);
        sprintf(tmp, "设备井(%d)连接状态不好，无法收到回应！", package->get_dest_id());
        shared_ptr<Package> alarm_package = send_alarm_msg(package, tmp);
        sent_package.erase(rec_num);
        data.my_msg_type = rand() % 3 + 1;
        // memcpy(data.anytext, pkg->get_message(), pkg->get_length());
        memcpy(data.anytext, alarm_package->get_message(), alarm_package->get_length());
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
          logger->error("{}从前端向后端发送井报警消息失败！", LOG_FILE_MSG);
        }
        shared_ptr<Package> pkg(new Package(0, CHANGE_MODE));
        pkg->set_dest_id(0);
        pkg->set_sequence_number(0);
        pkg->set_commu_type(false);
        data.my_msg_type = rand() % 3 + 1;
        memcpy(data.anytext, pkg->get_message(), pkg->get_length());
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
          logger->error("{}从前端向后端发送通信切换方式消息失败！", LOG_FILE_MSG);
        }
        tim->stop();
      }
    }
  }
}
void UIDemo08::check_radioButton(shared_ptr<Package> package, uint16_t address)
{
  uint32_t num = 0;
  if((!ui->radioButton->isChecked())&&(!ui->radioButton_2->isChecked())&&(!ui->radioButton_3->isChecked())&&(!ui->radioButton_4->isChecked())&&(!ui->radioButton_5->isChecked())&&(!ui->radioButton_6->isChecked())&&(!ui->radioButton_7->isChecked()))
  {
    QMessageBox::warning(this,"警告","未选中内容！");
  }
  else{
    if (ui->radioButton->isChecked())
  {
    num = get_sequence_number();
    make_mandrel_downcontrol_one_package(package, address, num, 0);
  }
  if (ui->radioButton_2->isChecked())
  {
    num = get_sequence_number();
    make_mandrel_downcontrol_one_package(package, address, num, 1);
  }
  if (ui->radioButton_3->isChecked())
  {
    num = get_sequence_number();
    make_mandrel_downcontrol_one_package(package, address, num, 2);
  }
  if (ui->radioButton_4->isChecked())
  {
    num = get_sequence_number();
    make_mandrel_downcontrol_one_package(package, address, num, 3);
  }
  if (ui->radioButton_5->isChecked())
  {
    num = get_sequence_number();
    make_mandrel_downcontrol_one_package(package, address, num, 4);
  }
  if (ui->radioButton_6->isChecked())
  {
    num = get_sequence_number();
    make_mandrel_downcontrol_one_package(package, address, num, 5);
  }
  if (ui->radioButton_7->isChecked())
  {
    num = get_sequence_number();
    make_mandrel_downcontrol_one_package(package, address, num, 6);
  }
  rec_num = num;
  tim = new QTimer();
  tim->setInterval(3000);
  connect(tim, SIGNAL(timeout()), this, SLOT(onTimeOut_mandrel()));
  tim->start();
  }
}
void UIDemo08::check_spinBox(shared_ptr<Package> package, uint16_t address, uint16_t cont)
{
  uint32_t num = get_sequence_number();
  make_mandrel_downcontrol_one_package(package, address, num, cont);
  rec_num = num;
  tim = new QTimer();
  tim->setInterval(3000);
  connect(tim, SIGNAL(timeout()), this, SLOT(onTimeOut_mandrel()));
  tim->start();
}
void UIDemo08::check_DoublespinBox(shared_ptr<Package> package, uint16_t address, float cont)
{
  uint32_t num = get_sequence_number();
  make_mandrel_downcontrol_two_package(package, address, num, cont);
  rec_num = num;
  tim = new QTimer();
  tim->setInterval(3000);
  connect(tim, SIGNAL(timeout()), this, SLOT(onTimeOut_mandrel()));
  tim->start();
}

void UIDemo08::on_selcurrButton_clicked()
{
  ui->selcurrButton->setEnabled(0);
  QTimer::singleShot(4000, this, [=]
                     {
                       ui->selcurrButton->setEnabled(1); // 3000ms后使能按钮，此时可点
                     });
  uint32_t query_len = 8;
  shared_ptr<Package> package(new Package(query_len, DOWN_QUERY));
  if (ui->comboBox_5->currentText() == "01")
  {
    package->set_dest_id(1107);
    if (ui->comboBox_6->currentIndex() == 0)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1115, rec_num);
    }
    else if (ui->comboBox_6->currentIndex() == 1)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1118, rec_num);
    }
    else if (ui->comboBox_6->currentIndex() == 2)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1119, rec_num);
    }
  }
  if (ui->comboBox_5->currentText() == "02")
  {
    package->set_dest_id(1207);
    if (ui->comboBox_6->currentIndex() == 0)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1215, rec_num);
    }
    if (ui->comboBox_6->currentIndex() == 1)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1218, rec_num);
    }
    if (ui->comboBox_6->currentIndex() == 2)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1219, rec_num);
    }
  }
  if (ui->comboBox_5->currentText() == "03")
  {
    package->set_dest_id(1307);
    if (ui->comboBox_6->currentIndex() == 0)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1315, rec_num);
    }
    if (ui->comboBox_6->currentIndex() == 1)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1318, rec_num);
    }
    if (ui->comboBox_6->currentIndex() == 2)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1319, rec_num);
    }
  }
  if (ui->comboBox_5->currentText() == "04")
  {
    package->set_dest_id(1407);
    if (ui->comboBox_6->currentIndex() == 0)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1415, rec_num);
    }
    if (ui->comboBox_6->currentIndex() == 1)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1418, rec_num);
    }
    if (ui->comboBox_6->currentIndex() == 2)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1419, rec_num);
    }
  }
  if (ui->comboBox_5->currentText() == "05")
  {
    package->set_dest_id(1507);
    if (ui->comboBox_6->currentIndex() == 0)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1515, rec_num);
    }
    if (ui->comboBox_6->currentIndex() == 1)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1518, rec_num);
    }
    if (ui->comboBox_6->currentIndex() == 2)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1519, rec_num);
    }
  }
  if (ui->comboBox_5->currentText() == "06")
  {
    package->set_dest_id(1607);
    if (ui->comboBox_6->currentIndex() == 0)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1615, rec_num);
    }
    if (ui->comboBox_6->currentIndex() == 1)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1618, rec_num);
    }
    if (ui->comboBox_6->currentIndex() == 2)
    {
      rec_num = get_sequence_number();
      make_selcurr_package(package, 1619, rec_num);
    }
  }
  tim = new QTimer();
  tim->setInterval(3000);
  connect(tim, SIGNAL(timeout()), this, SLOT(onTimeOut()));
  selnum = 0;
  tim->start();
}

void UIDemo08::on_onWellButton_clicked()
{

  if(ui->wname_lEdit->text().isEmpty())
  {
      QMessageBox::warning(this,"警告","输入为空字符串，无法修改");
  }
  if(ui->wname_lEdit->text().length()>20)
  {
      QMessageBox::warning(this,"警告","字数超过20个字节，无法修改");
  }
  else
  {
      ui->onWellButton->setEnabled(0);
      QTimer::singleShot(4000, this, [=]
                         {
                           ui->onWellButton->setEnabled(1); // 4000ms后使能按钮，此时可点
                         });
      uint32_t query_len = 29;
      shared_ptr<Package> package(new Package(query_len, DOWN_CONTROL));
      package->set_dest_id(999);
      rec_num = get_sequence_number();
      make_well_downcontrol_package(package, 1000, rec_num, ui->wname_lEdit->text().toStdString());
      selnum = 0;
      tim = new QTimer();
      tim->setInterval(3000);
      connect(tim, SIGNAL(timeout()), this, SLOT(onTimeOut_well()));
      selnum = 0;
      tim->start();

  }
}

void UIDemo08::on_pushButton_2_clicked()
{
  ui->pushButton_2->setEnabled(0);
  QTimer::singleShot(4000, this, [=]
                     {
                       ui->pushButton_2->setEnabled(1); // 4000ms后使能按钮，此时可点
                     });
  uint32_t query_len = 8;
  shared_ptr<Package> package(new Package(query_len, DOWN_CONTROL));
  package->set_dest_id(999);
  rec_num = get_sequence_number();
  make_mandrel_downcontrol_one_package(package, 1010, rec_num, ui->well_layer_box->currentText().toInt());
  tim = new QTimer();
  tim->setInterval(3000);
  connect(tim, SIGNAL(timeout()), this, SLOT(onTimeOut_well()));
  tim->start();
}

void UIDemo08::on_onMandrelButton_clicked()
{
  ui->onMandrelButton->setEnabled(0);
  QTimer::singleShot(4000, this, [=]
                     {
                       ui->onMandrelButton->setEnabled(1); // 3000ms后使能按钮，此时可点
                     });
  uint32_t query_len = 8;
  shared_ptr<Package> package(new Package(query_len, DOWN_CONTROL));
  if (ui->comboBox_3->currentText() == "01")
  {
    package->set_dest_id(1107);
    this->check_radioButton(package, 1120);
  }
  if (ui->comboBox_3->currentText() == "02")
  {
    package->set_dest_id(1207);
    this->check_radioButton(package, 1220);
  }
  if (ui->comboBox_3->currentText() == "03")
  {
    package->set_dest_id(1307);
    this->check_radioButton(package, 1320);
  }
  if (ui->comboBox_3->currentText() == "04")
  {
    package->set_dest_id(1407);
    this->check_radioButton(package, 1420);
  }
  if (ui->comboBox_3->currentText() == "05")
  {
    package->set_dest_id(1507);
    this->check_radioButton(package, 1520);
  }
  if (ui->comboBox_3->currentText() == "06")
  {
    package->set_dest_id(1607);
    this->check_radioButton(package, 1620);
  }
}

void UIDemo08::on_ondoubleButton_clicked()
{
  if(ui->doubleSpinBox->value()<0||ui->doubleSpinBox->value()>5000)
  {
      QMessageBox::warning(this,"警告","输入错误！配注量的范围是0-5000");
  }
  else
  {
  ui->ondoubleButton->setEnabled(0);
  QTimer::singleShot(4000, this, [=]
                     {
                       ui->ondoubleButton->setEnabled(1); // 3000ms后使能按钮，此时可点
                     });
  uint32_t query_len = 13;
  shared_ptr<Package> package(new Package(query_len, DOWN_CONTROL));
  float f = static_cast<float>(ui->doubleSpinBox->value());
  if (ui->comboBox_3->currentText() == "01")
  {
    package->set_dest_id(1107);
    this->check_DoublespinBox(package, 1800, f);
  }
  if (ui->comboBox_3->currentText() == "02")
  {
    package->set_dest_id(1207);
    this->check_DoublespinBox(package, 1806, f);
  }
  if (ui->comboBox_3->currentText() == "03")
  {
    package->set_dest_id(1307);
    this->check_DoublespinBox(package, 1812, f);
  }
  if (ui->comboBox_3->currentText() == "04")
  {
    package->set_dest_id(1407);
    this->check_DoublespinBox(package, 1818, f);
  }
  if (ui->comboBox_3->currentText() == "05")
  {
    package->set_dest_id(1507);
    this->check_DoublespinBox(package, 1824, f);
  }
  if (ui->comboBox_3->currentText() == "06")
  {
    package->set_dest_id(1607);
    this->check_DoublespinBox(package, 1830, f);
  }
  }
}

void UIDemo08::on_spinButton_clicked()
{
    if( ui->spinBox->value()<0|| ui->spinBox->value()>10000)
    {
        QMessageBox::warning(this,"警告","输入错误！调节阀开度的范围是0-10000");
    }
    else
    {

  ui->spinButton->setEnabled(0);
  QTimer::singleShot(4000, this, [=]
                     {
                       ui->spinButton->setEnabled(1); // 3000ms后使能按钮，此时可点
                     });
  uint32_t query_len = 8;
  shared_ptr<Package> package(new Package(query_len, DOWN_CONTROL));
  int currentValue = ui->spinBox->value();
  if (ui->comboBox_3->currentText() == "01")
  {
    package->set_dest_id(1107);
    this->check_spinBox(package, 1113, currentValue);
  }
  if (ui->comboBox_3->currentText() == "02")
  {
    package->set_dest_id(1207);
    this->check_spinBox(package, 1213, currentValue);
  }
  if (ui->comboBox_3->currentText() == "03")
  {
    package->set_dest_id(1307);
    this->check_spinBox(package, 1313, currentValue);
  }
  if (ui->comboBox_3->currentText() == "04")
  {
    package->set_dest_id(1407);
    this->check_spinBox(package, 1413, currentValue);
  }
  if (ui->comboBox_3->currentText() == "05")
  {
    package->set_dest_id(1507);
    this->check_spinBox(package, 1513, currentValue);
  }
  if (ui->comboBox_3->currentText() == "06")
  {
    package->set_dest_id(1607);
    this->check_spinBox(package, 1613, currentValue);
  }
    }
}

void UIDemo08::on_controllerButton_history_clicked()
{
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStandardItemModel *m_tableModel = new QStandardItemModel(ui->tableView);
  m_tableModel->setHorizontalHeaderItem(0, new QStandardItem("ID"));
  m_tableModel->setHorizontalHeaderItem(1, new QStandardItem("地址 ID"));
  m_tableModel->setHorizontalHeaderItem(2, new QStandardItem("控制器编号"));
  m_tableModel->setHorizontalHeaderItem(3, new QStandardItem("控制器型号"));
  m_tableModel->setHorizontalHeaderItem(4, new QStandardItem("控制器输出电压"));
  m_tableModel->setHorizontalHeaderItem(5, new QStandardItem("控制器输出电流"));
  m_tableModel->setHorizontalHeaderItem(6, new QStandardItem("连接井数"));
  m_tableModel->setHorizontalHeaderItem(7, new QStandardItem("年月"));
  m_tableModel->setHorizontalHeaderItem(8, new QStandardItem("日时"));
  m_tableModel->setHorizontalHeaderItem(9, new QStandardItem("分秒"));
  m_tableModel->setHorizontalHeaderItem(10, new QStandardItem("时间戳"));
  ui->tableView->setModel(m_tableModel);
  std::vector<controllerrecord_t> ret;
  ret = search_controllerrecord_info();
  for (int i = 0; i < ret.size(); i++)
  {
    m_tableModel->setItem(i, 0, new QStandardItem(QString::number(ret[i].id)));
    m_tableModel->setItem(i, 1, new QStandardItem(QString::number(ret[i].device_id)));
    m_tableModel->setItem(i, 2, new QStandardItem(QString::number(ret[i].controller_id)));
    m_tableModel->setItem(i, 3, new QStandardItem(QString::number(ret[i].controllertype)));
    m_tableModel->setItem(i, 4, new QStandardItem(QString::number(static_cast<float>(ret[i].outputvoltage/10.0),'f',2)));
    m_tableModel->setItem(i, 5, new QStandardItem(QString::number(static_cast<float>(ret[i].outputcurrent/10.0),'f',2)));
    m_tableModel->setItem(i, 6, new QStandardItem(QString::number(ret[i].wellnum)));
    m_tableModel->setItem(i, 7, new QStandardItem(QString::number(ret[i].datetime1)));
    m_tableModel->setItem(i, 8, new QStandardItem(QString::number(ret[i].datetime2)));
    m_tableModel->setItem(i, 9, new QStandardItem(QString::number(ret[i].datetime3)));
    m_tableModel->setItem(i, 10, new QStandardItem(QString::fromStdString(ret[i].timestamp)));
  }
  
}

void UIDemo08::on_wellButton_history_clicked()
{
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStandardItemModel *m_tableModel = new QStandardItemModel(ui->tableView);
  m_tableModel->setHorizontalHeaderItem(0, new QStandardItem("ID"));
  m_tableModel->setHorizontalHeaderItem(1, new QStandardItem("地址 ID"));
  m_tableModel->setHorizontalHeaderItem(2, new QStandardItem("井号"));
  m_tableModel->setHorizontalHeaderItem(3, new QStandardItem("井名称"));
  m_tableModel->setHorizontalHeaderItem(4, new QStandardItem("井下注采层数"));
  m_tableModel->setHorizontalHeaderItem(5, new QStandardItem("井下供电状态"));
  m_tableModel->setHorizontalHeaderItem(6, new QStandardItem("运行状态标识"));
  m_tableModel->setHorizontalHeaderItem(7, new QStandardItem("工作模式"));
  m_tableModel->setHorizontalHeaderItem(8, new QStandardItem("自控仪流量"));
  m_tableModel->setHorizontalHeaderItem(9, new QStandardItem("自控仪压力"));
  m_tableModel->setHorizontalHeaderItem(10, new QStandardItem("自控仪阀开度"));
  m_tableModel->setHorizontalHeaderItem(11, new QStandardItem("自控仪阀状态"));
  m_tableModel->setHorizontalHeaderItem(12, new QStandardItem("设置自控仪流量"));
  m_tableModel->setHorizontalHeaderItem(13, new QStandardItem("时间戳"));
  ui->tableView->setModel(m_tableModel);
  std::vector<wellrecord_t> ret;
  ret = search_wellrecord_info();
  for (int i = 0; i < ret.size(); i++)
  {
    m_tableModel->setItem(i, 0, new QStandardItem(QString::number(ret[i].id)));
    m_tableModel->setItem(i, 1, new QStandardItem(QString::number(ret[i].device_id)));
    m_tableModel->setItem(i, 2, new QStandardItem(QString::number(ret[i].well_id)));
    m_tableModel->setItem(i, 3, new QStandardItem(QString::fromUtf8(ret[i].name, sizeof(ret[i].name))));
    m_tableModel->setItem(i, 4, new QStandardItem(QString::number(ret[i].layer_number)));
    m_tableModel->setItem(i, 5, new QStandardItem(QString::number(ret[i].power_status)));
    m_tableModel->setItem(i, 6, new QStandardItem(QString::number(ret[i].status_flag)));
    m_tableModel->setItem(i, 7, new QStandardItem(QString::number(ret[i].operating_mode)));
    m_tableModel->setItem(i, 8, new QStandardItem(QString::number(ret[i].control_traffic)));
    m_tableModel->setItem(i, 9, new QStandardItem(QString::number(static_cast<float>(ret[i].control_pressure/100.0),'f',2)));
    m_tableModel->setItem(i, 10, new QStandardItem(QString::number(ret[i].control_opening)));
    m_tableModel->setItem(i, 11, new QStandardItem(QString::number(ret[i].control_status)));
    m_tableModel->setItem(i, 12, new QStandardItem(QString::number(ret[i].traffic_setting)));
    m_tableModel->setItem(i, 13, new QStandardItem(QString::fromStdString(ret[i].timestamp)));
  }
}

void UIDemo08::on_mandrelButton_history_clicked()
{
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStandardItemModel *m_tableModel = new QStandardItemModel(ui->tableView);
  m_tableModel->setHorizontalHeaderItem(0, new QStandardItem("ID"));
  m_tableModel->setHorizontalHeaderItem(1, new QStandardItem("地址 ID"));
  m_tableModel->setHorizontalHeaderItem(2, new QStandardItem("所属井号"));
  m_tableModel->setHorizontalHeaderItem(3, new QStandardItem("层号"));
  m_tableModel->setHorizontalHeaderItem(4, new QStandardItem("流量"));
  m_tableModel->setHorizontalHeaderItem(5, new QStandardItem("温度"));
  m_tableModel->setHorizontalHeaderItem(6, new QStandardItem("内压"));
  m_tableModel->setHorizontalHeaderItem(7, new QStandardItem("外压"));
  m_tableModel->setHorizontalHeaderItem(8, new QStandardItem("调节阀开度精度"));
  m_tableModel->setHorizontalHeaderItem(9, new QStandardItem("含水率"));
  m_tableModel->setHorizontalHeaderItem(10, new QStandardItem("工作筒状态"));
  m_tableModel->setHorizontalHeaderItem(11, new QStandardItem("电机状态"));
  m_tableModel->setHorizontalHeaderItem(12, new QStandardItem("电机电流"));
  m_tableModel->setHorizontalHeaderItem(13, new QStandardItem("层电压"));
  m_tableModel->setHorizontalHeaderItem(14, new QStandardItem("层电流"));
  m_tableModel->setHorizontalHeaderItem(15, new QStandardItem("工作模式"));
  m_tableModel->setHorizontalHeaderItem(16, new QStandardItem("调节阀状态"));
  m_tableModel->setHorizontalHeaderItem(17, new QStandardItem("配注/产控制"));
  m_tableModel->setHorizontalHeaderItem(18, new QStandardItem("配注/产量"));
  m_tableModel->setHorizontalHeaderItem(19, new QStandardItem("流量上偏差"));
  m_tableModel->setHorizontalHeaderItem(20, new QStandardItem("流量下偏差"));
  m_tableModel->setHorizontalHeaderItem(21, new QStandardItem("调节时间"));
  m_tableModel->setHorizontalHeaderItem(22, new QStandardItem("恢复测试控制"));
  m_tableModel->setHorizontalHeaderItem(23, new QStandardItem("测试时间"));
  m_tableModel->setHorizontalHeaderItem(24, new QStandardItem("吸水测试控制"));
  m_tableModel->setHorizontalHeaderItem(25, new QStandardItem("水嘴开启关闭保持状态"));
  m_tableModel->setHorizontalHeaderItem(26, new QStandardItem("时间戳"));
  ui->tableView->setModel(m_tableModel);
  std::vector<mandrelrecord_t> ret;
  ret = search_mandrelrecord_info();
  for (int i = 0; i < ret.size(); i++)
  {
    m_tableModel->setItem(i, 0, new QStandardItem(QString::number(ret[i].id)));
    m_tableModel->setItem(i, 1, new QStandardItem(QString::number(ret[i].device_id)));
    m_tableModel->setItem(i, 2, new QStandardItem(QString::number(ret[i].wellid)));
    m_tableModel->setItem(i, 3, new QStandardItem(QString::number(ret[i].layer)));
    m_tableModel->setItem(i, 4, new QStandardItem(QString::number(ret[i].traffic)));
    m_tableModel->setItem(i, 5, new QStandardItem(QString::number(static_cast<float>(ret[i].temperature/100.0),'f',2)));
    m_tableModel->setItem(i, 6, new QStandardItem(QString::number(static_cast<float>(ret[i].internalpressure/100.0),'f',2)));
    m_tableModel->setItem(i, 7, new QStandardItem(QString::number(static_cast<float>(ret[i].externalpressure/100.0),'f',2)));
    m_tableModel->setItem(i, 8, new QStandardItem(QString::number(static_cast<float>(ret[i].openingaccuracy/100.0),'f',2)));
    m_tableModel->setItem(i, 9, new QStandardItem(QString::number(static_cast<float>(ret[i].moisturepercentage/100.0),'f',2)));
    m_tableModel->setItem(i, 10, new QStandardItem(QString::number(ret[i].status)));
    m_tableModel->setItem(i, 11, new QStandardItem(QString::number(ret[i].motorstatus)));
    m_tableModel->setItem(i, 12, new QStandardItem(QString::number(static_cast<float>(ret[i].motorcurrent/10.0),'f',2)));
    m_tableModel->setItem(i, 13, new QStandardItem(QString::number(static_cast<float>(ret[i].voltage/10.0),'f',2)));
    m_tableModel->setItem(i, 14, new QStandardItem(QString::number(static_cast<float>(ret[i].current/10.0),'f',2)));
    m_tableModel->setItem(i, 15, new QStandardItem(QString::number(ret[i].operatingmode)));
    m_tableModel->setItem(i, 16, new QStandardItem(QString::number(ret[i].controlstatus)));
    m_tableModel->setItem(i, 17, new QStandardItem(QString::number(ret[i].yieldcontrol)));
    m_tableModel->setItem(i, 18, new QStandardItem(QString::number(ret[i].yieldvalue)));
    m_tableModel->setItem(i, 19, new QStandardItem(QString::number(static_cast<float>(ret[i].upflowdeviation/10.0),'f',2)));
    m_tableModel->setItem(i, 20, new QStandardItem(QString::number(static_cast<float>(ret[i].downflowdeviation/10.0),'f',2)));
    m_tableModel->setItem(i, 21, new QStandardItem(QString::number(ret[i].adjustmentminutes)));
    m_tableModel->setItem(i, 22, new QStandardItem(QString::number(ret[i].pressuretestcontrol)));
    m_tableModel->setItem(i, 23, new QStandardItem(QString::number(ret[i].pressuretesthour)));
    m_tableModel->setItem(i, 24, new QStandardItem(QString::number(ret[i].waterabsorptiontest)));
    m_tableModel->setItem(i, 25, new QStandardItem(QString::number(ret[i].faucetflag)));
    m_tableModel->setItem(i, 26, new QStandardItem(QString::fromStdString(ret[i].timestamp)));
  }
}

void UIDemo08::on_exportButton_clicked()
{
  if(ui->tableView->model()==nullptr)
  {
      QMessageBox::warning(this,"警告","未查询数据，无法导出");
  }
  else{

      //获取创建的csv文件名
        QModelIndex index;
        QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), "", tr("Files (*.csv)"));
        if (fileName.isEmpty())
          return;
        // 打开.csv文件
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
          qDebug("Cannot open file for writing: %s", qPrintable(file.errorString()));
          return;
        }
        QTextStream out(&file);
        int row = ui->tableView->model()->rowCount();
        int col = ui->tableView->model()->columnCount();
      // 写表头
      for (int i = 0; i < col; i++)
      {
        out << ui->tableView->model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() << ",";
        if (i == (col - 1))
        {
          out << "\n";
        }
      }
      // 写数据
      for (int i = 0; i < row; i++)
      {
        for (int j = 0; j < col; j++)
        {
          index = ui->tableView->model()->index(i, j);
          out << ui->tableView->model()->data(index).toString() << ",";
          if (j == (col - 1))
          {
            out << "\n";
          }
        }
      }
      QMessageBox::information(this, tr("导出数据成功"), tr("信息已保存在%1！").arg(fileName), tr("确定"));
      file.close();

  }
}

void UIDemo08::on_remainButton_clicked()
{
  QDateTime time = QDateTime::currentDateTime(); // 获取系统现在的时间
  QString BeforeDaystr;
  string BeforeDay;
  if (ui->one_month->isChecked())
  {
    BeforeDaystr = time.addMonths(-1).toString("yyyy-MM-dd HH:mm:ss.zzz"); // 获取前30天时间
    BeforeDay = BeforeDaystr.toStdString();
    if (detete_controllerrecord_info(BeforeDay.c_str())&&delete_mandrelrecord_info(BeforeDay.c_str())&&delete_wellrecord_info(BeforeDay.c_str())&&detete_status_alarm_info(BeforeDay.c_str())&&delete_actionrecord_info(BeforeDay.c_str()))
      QMessageBox::information(this, "删除", "成功删除一个月之前数据!");
    else
      QMessageBox::warning(this, "删除", "删除失败!");
  }
  if (ui->three_month->isChecked())
  {
    BeforeDaystr = time.addMonths(-3).toString("yyyy-MM-dd HH:mm:ss.zzz"); // 获取前90天时间
    BeforeDay = BeforeDaystr.toStdString();
    if (detete_controllerrecord_info(BeforeDay.c_str())&&delete_mandrelrecord_info(BeforeDay.c_str())&&delete_wellrecord_info(BeforeDay.c_str())&&detete_status_alarm_info(BeforeDay.c_str())&&delete_actionrecord_info(BeforeDay.c_str()))
      QMessageBox::information(this, "删除", "成功删除三个月之前数据!");
    else
      QMessageBox::warning(this, "删除", "删除失败!");
  }
  if (ui->six_month->isChecked())
  {
    BeforeDaystr = time.addDays(-6).toString("yyyy-MM-dd HH:mm:ss.zzz"); // 获取前180天时间
    BeforeDay = BeforeDaystr.toStdString();
    if (detete_controllerrecord_info(BeforeDay.c_str())&&delete_mandrelrecord_info(BeforeDay.c_str())&&delete_wellrecord_info(BeforeDay.c_str())&&detete_status_alarm_info(BeforeDay.c_str())&&delete_actionrecord_info(BeforeDay.c_str()))
      QMessageBox::information(this, "删除", "成功删除六个月之前数据!");
    else
      QMessageBox::warning(this, "删除", "删除失败!");
  }
  if(!(ui->one_month->isChecked())&&!(ui->three_month->isChecked())&&!(ui->six_month->isChecked()))
  {
    QMessageBox::warning(this, "警告", "请确定保留时间范围!");
  }

}
