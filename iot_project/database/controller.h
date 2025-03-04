#ifndef _CONTROLLER_H
#define _CONTROLLER_H
#include <cstdint>
#include <string>
#include <vector>
#include "controllerrecord.h"
using namespace std;

typedef struct controller
{
    uint16_t id;
    uint16_t controller_id;
    uint16_t controllertype;
    uint16_t outputvoltage;
    uint16_t outputcurrent;
    uint16_t wellnum;
    uint16_t datetime1;
    uint16_t datetime2;
    uint16_t datetime3;
} controller_t;

bool insert_controller_info(controller_t& controller_info);
bool insert_crecord_info(controller_t& controller_info,bool is_sync);
std::vector<controller_t> search_controller_info();
bool update_controller_info(controllerrecord_t& controller_info,uint16_t id);
bool delete_controller_info(uint32_t id);
bool search_controller_not_info(uint16_t id);
std::vector<controller_t> search_controller_not_upload_info();
bool update_controller_info_upload(uint32_t id);
bool insert_all_controller_info(controller_t& controller_info);
#endif