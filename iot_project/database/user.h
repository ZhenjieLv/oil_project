#ifndef _USER_H
#define _USER_H
#include <cstdint>
#include <string>
#include <vector>
using namespace std;

typedef struct user
{
    uint32_t user_id;
    char name[20];
    char password[20];
    char depart[20];
    char phone[20];
} user_t;

bool insert_user_info(user_t& user_info);
std::vector<user_t> search_user_info();
bool update_user_info(user_t& user_info);
bool delete_user_info(uint32_t user_id);
std::vector<user_t> search_user_not_upload_info();
bool update_user_info_upload(uint32_t user_id);
bool insert_all_user_info(user_t& user_info);
#endif