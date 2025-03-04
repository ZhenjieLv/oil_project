#ifndef _WELLRECORD_H
#define _WELLRECORD_H
#include <cstdint>
#include <string>
#include <vector>
using namespace std;

typedef struct wellrecord
{
    uint32_t id;
    uint16_t device_id;
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
    std::string timestamp;
} wellrecord_t;

bool insert_wellrecord_info(wellrecord_t& well_info,bool is_sync);
std::vector<wellrecord_t> search_wellrecord_info();
bool update_wellrecord_info(wellrecord_t& well_info);
bool delete_wellrecord_info(uint32_t id);
bool delete_wellrecord_info(const char* end_time);
std::vector<wellrecord_t> search_wellrecord_not_upload_info();
bool update_wellrecord_info_upload(uint16_t id);
bool update_wellrecord_upload(uint32_t id);
bool insert_all_wellrecord_info(wellrecord_t& well_info);
#endif