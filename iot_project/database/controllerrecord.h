#ifndef _CONTROLLERRECORD_H
#define _CONTROLLERRECORD_H
#include <cstdint>
#include <string>
#include <vector>
using namespace std;

typedef struct controllerrecord
{
    uint32_t id;
    uint16_t device_id;
    uint16_t controller_id;
    uint16_t controllertype;
    uint16_t outputvoltage;
    uint16_t outputcurrent;
    uint16_t wellnum;
    uint16_t datetime1;
    uint16_t datetime2;
    uint16_t datetime3;
    std::string timestamp;
} controllerrecord_t;

bool insert_controllerrecord_info(controllerrecord_t& controller_info,bool is_sync);
std::vector<controllerrecord_t> search_controllerrecord_info();
bool update_controllerrecord_info(controllerrecord_t& controller_info);
bool delete_controllerrecord_info(uint32_t id);
bool detete_controllerrecord_info(const char* end_time);
std::vector<controllerrecord_t> search_controllerrecord_not_upload_info();
bool update_controllerrecord_info_upload(uint16_t device_id);
bool update_controllerrecord_upload(uint32_t id);
bool insert_all_controllerrecord_info(controllerrecord_t& controllerrecord_info);
#endif