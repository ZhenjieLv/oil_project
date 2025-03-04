#ifndef _DEALPK_H
#define _DEALPK_H
#include <iostream>
#include "util.h"
// #include "controller.h"
// #include "controllerrecord.h"
// #include "wellrecord.h"
// #include "mandrelrecord.h"
// #include "mandrel.h"
// #include "well.h"
#include "buffer_queue.h"
#include <string>
#include "variables.h"
#include "intreadwrite.h"
#include <map>
#include <vector>
#include "error_no.h"
#include <sys/msg.h>
using namespace std;
typedef union
{
    float a;
    unsigned int b;
}Data;
extern std::map<uint32_t,shared_ptr<Package>> sent_package;
extern std::map<uint32_t,shared_ptr<Package>> recv_package;
extern std::map<uint32_t,shared_ptr<Package>> cloud_action_record;
extern std::map<uint32_t,shared_ptr<Package>> cloud_package;
extern uint32_t rec_num;
extern int selnum;
string JsonAsString(const Json::Value &json);
// bool parseWell(shared_ptr<Package> package,bool is_sync);
// bool parseMandrel(shared_ptr<Package> package,bool is_sync);
// bool parseController(shared_ptr<Package> package,bool is_sync);
// bool parseControllerrecord(shared_ptr<Package> package,bool is_sync);
// bool parseMandrelrecord(shared_ptr<Package> package,bool is_sync);
// bool parseWellrecord(shared_ptr<Package> package,bool is_sync);
string parse_well_to_kafka(shared_ptr<Package> package);
string parse_mandrel_to_kafka(shared_ptr<Package> package);
string parse_controller_to_kafka(shared_ptr<Package> package);
string parse_alarm_to_kafka(shared_ptr<Package> package);
string parse_actionrecord_to_kafka(shared_ptr<Package> package, bool is_sync);
void make_selcurr_package(shared_ptr<Package> package,uint16_t address,uint32_t length);
void make_well_downcontrol_package(shared_ptr<Package> package,uint16_t address,uint32_t length,string s);
void make_mandrel_downcontrol_one_package(shared_ptr<Package> package,uint16_t address,uint32_t length, uint16_t cont);
void make_mandrel_downcontrol_two_package(shared_ptr<Package> package,uint16_t address,uint32_t length, float cont);
string convert_string(uint8_t *buffer,uint32_t length);
shared_ptr<Package> send_change_mode_msg(bool communication_mode);
shared_ptr<Package> send_alarm_msg(uint16_t device_id, char* tmp);
shared_ptr<Package> send_alarm_msg(shared_ptr<Package> pkg, char* tmp);
shared_ptr<Package> send_actionrecord_msg(shared_ptr<Package> pkg,bool is_sync);
shared_ptr<Package> convert_package(shared_ptr<Package> pkg);
int arrayToStr(unsigned char *buf, unsigned int buflen, char *out);
#endif
