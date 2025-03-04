#include <cstdio>
#include <cstring>
#include <iostream>
#include "user.h"
#include "database.h"
using namespace std;

/**
 * @brief 插入一条用户数据到用户表(所有数据)
 * 
 * @param user_info 用户数据
 * @return true 插入成功
 * @return false 插入失败
 */
bool insert_user_info(user_t& user_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[250] = {0};
    sprintf(query, "insert into User(UserID, Username, Password, Department, Phone) values(%u, '%s', '%s', '%s', '%s')", user_info.user_id, user_info.name, user_info.password, user_info.depart, user_info.phone);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}

/**
 * @brief 查询用户表的所有数据
 * 
 * @return std::vector<user_t> 查询到的用户表数据
 */
std::vector<user_t> search_user_info()
{
    std::vector<user_t> ret;
    Database db;
    if(!db.connect()) 
    return ret;
    //cout<<"fail";
    char query[50] = "select * from User;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        user_t temp = {0};
        temp.user_id = char_to_uint32(row[0]);
        memcpy(temp.name, row[1], strlen(row[1]));
        memcpy(temp.password, row[2], strlen(row[2]));
        memcpy(temp.depart, row[3], strlen(row[3]));
        memcpy(temp.phone, row[4], strlen(row[4]));
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
/**
 * @brief 根据id更新用户数据到用户表
 * 
 * @param user_info 用户数据
 * @return true 更新成功
 * @return false 更新失败
 */
bool update_user_info(user_t& user_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update User set Username = '%s',Password = '%s', Department = '%s', Phone = '%s' where UserID = %u",user_info.name, user_info.password, user_info.depart, user_info.phone, user_info.user_id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 从用户表中删除一条用户数据
 * 
 * @param user_id
 * @return true 删除成功
 * @return false 删除失败
 */
bool delete_user_info(uint32_t user_id)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from User where UserID = %u", user_id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 查询表中未上传的数据
 * 
 * @return std::vector<user_t> 
 */
std::vector<user_t> search_user_not_upload_info()
{
    std::vector<user_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from User where isSync = false;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        user_t temp = {0};
        temp.user_id = char_to_uint32(row[0]);
        memcpy(temp.name, row[1], strlen(row[1]));
        memcpy(temp.password, row[2], strlen(row[2]));
        memcpy(temp.depart, row[3], strlen(row[3]));
        memcpy(temp.phone, row[4], strlen(row[4]));
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
/**
 * @brief 根据user_id修改isSync为true，表示数据已经上传了
 * 
 * @param user_id 
 * @return true 
 * @return false 
 */
bool update_user_info_upload(uint32_t user_id)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update User set isSync = true where UserID = %u",user_id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 插入所有数据，包括user_id
 * 
 * @param user_info 
 * @return true 
 * @return false 
 */
bool insert_all_user_info(user_t& user_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[250] = {0};
    sprintf(query, "insert into User(UserID, Username, Password, Department, Phone) values(%u, '%s', '%s', '%s', '%s')", user_info.user_id, user_info.name, user_info.password, user_info.depart, user_info.phone);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
