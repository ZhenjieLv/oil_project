#ifndef _ACTIONRECORD_H
#define _ACTIONRECORD_H
#include <cstdint>
#include <string>
#include <vector>
using namespace std;

typedef struct actionrecord
{
    uint32_t id;
    std::string timestamp;
    uint16_t device_id;
    bool issucceed;
    char package[200];
} actionrecord_t;

int insert_actionrecord_info(uint16_t device_id,string action_record_msg);
bool update_actionrecord_info_succeed(uint32_t id);
std::vector<actionrecord_t> search_actionrecord_not_upload_info();
bool update_actionrecord_info_upload(uint32_t id);
bool delete_actionrecord_info(const char* end_time);
#endif