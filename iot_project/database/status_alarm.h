#ifndef _STATUS_ALARM_H
#define _STATUS_ALARM_H
#include <cstdint>
#include <string>
#include <vector>
using namespace std;

typedef struct status_alarm
{
    uint32_t alarm_id;
    std::string timestamp;
    uint16_t device_id;
    char reason[200];
} status_alarm_t;

bool insert_status_alarm_info(uint16_t device_id,char *reason,bool is_sync);
std::vector<status_alarm_t> search_status_alarm_info();
std::vector<status_alarm_t> search_alarm_id();
bool update_status_alarm_info(status_alarm_t& status_alarm_info);
bool delete_status_alarm_info(uint32_t id);
std::vector<status_alarm_t> search_status_alarm_not_upload_info();
bool update_status_alarm_info_upload(uint32_t id);
bool insert_all_status_alarm_info(status_alarm_t& status_alarm_info);
bool insert_status_alarm_info(uint16_t device_id,char *reason);
bool detete_status_alarm_info(const char* end_time);
#endif