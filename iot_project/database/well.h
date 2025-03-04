#ifndef _WELL_H
#define _WELL_H
#include <cstdint>
#include <string>
#include <vector>
#include "wellrecord.h"
using namespace std;

typedef struct well
{
    uint16_t id;
    uint16_t well_id;
    char name[40];
    uint16_t layer_number;
    uint16_t power_status;
    uint16_t status_flag;
    uint16_t operating_mode;
    float control_traffic;
    uint16_t control_pressure;
    uint16_t control_opening;
    uint16_t control_status;
    float traffic_setting;
} well_t;

bool insert_well_info(well_t& well_info);
bool insert_wrecord_info(well_t& well_info,bool is_sync);
std::vector<well_t> search_well_info();
bool search_well_not_info(uint16_t id);
bool update_well_info(wellrecord_t& well_info,uint16_t id);
bool delete_well_info(uint16_t id);
std::vector<well_t> search_well_not_upload_info();
bool update_well_info_upload(uint16_t id);
bool insert_all_well_info(well_t& well_info);
#endif