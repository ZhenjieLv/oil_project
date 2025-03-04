#ifndef _USERACTION_H
#define _USERACTION_H
#include <cstdint>
#include <string>
#include <vector>
using namespace std;
typedef struct useraction
{
    uint16_t action_id;
    uint32_t user_id;
    char actiondetails[200];
    std::string timestamp;
} useraction_t;

bool insert_useraction_info(useraction_t& useraction_info);
std::vector<useraction_t> search_useraction_info();
bool update_useraction_info(useraction_t& useraction_info);
bool delete_useraction_info(uint32_t action_id);
std::vector<useraction_t> search_useraction_not_upload_info();
bool update_useraction_info_upload(uint32_t action_id);
bool insert_all_useraction_info(useraction_t& useraction_info);
#endif