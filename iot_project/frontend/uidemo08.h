#ifndef UIDEMO08_H
#define UIDEMO08_H

#include <QWidget>
#include <QTimer>
#include "wellrecord.h"
#include "mandrelrecord.h"
#include "controllerrecord.h"
#include "status_alarm.h"
#include "actionrecord.h"
#include <QMessageBox>
#include  <QFile>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include "dealpk.h"
#include "package_process.h"
#include "sync_cloud_msg.h"
#include <QDesktopWidget>
using namespace std;
class QToolButton;

namespace Ui {
class UIDemo08;
}

class UIDemo08 : public QWidget
{
    Q_OBJECT

public:
    explicit UIDemo08(QWidget *parent = 0);
    ~UIDemo08();

private:
    Ui::UIDemo08 *ui;
    QTimer *tim;
    std::shared_ptr<spdlog::logger> logger;


    QList<int> pixCharMain;
    QList<QToolButton *> btnsMain;

    QList<int> pixCharConfig;
    QList<QToolButton *> btnsConfig;

private slots:
    void initForm();
    void buttonClick();
    void initLeftMain();
    void initLeftConfig();
    void leftMainClick();
    void leftConfigClick();
private:void check_radioButton(shared_ptr<Package> package,uint16_t address);
private:void check_spinBox(shared_ptr<Package> package,uint16_t address,uint16_t cont);
private:void check_DoublespinBox(shared_ptr<Package> package,uint16_t address, float cont);
private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void onTimeOut(); 
    void onTimeOut_mandrel();
    void onTimeOut_well();
    void on_selcurrButton_clicked();
    void on_onMandrelButton_clicked();
    void on_exportButton_clicked();
    void on_ondoubleButton_clicked();
    void on_spinButton_clicked();
    void on_controllerButton_history_clicked();
    void on_mandrelButton_history_clicked();
    void on_wellButton_history_clicked();
    void on_onWellButton_clicked();
    void on_pushButton_2_clicked();
    void on_remainButton_clicked();
};
#endif // UIDEMO08_H
