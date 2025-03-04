#include <cstdio>
#include <cstring>
#include "useraction.h"
#include "database.h"
#include <iostream>


using namespace std;
/**
 * @brief 插入一条用户操作数据到用户操作表(不包括Timestamp)
 * 
 * @param useraction_info 用户操作数据
 * @return true 插入成功
 * @return false 插入失败
 */
bool insert_useraction_info(useraction_t& useraction_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query, "insert into UserAction(ActionId, UserID, ActionDetails) "
            "values(%u, %u, %u, '%s')", 
            useraction_info.action_id, useraction_info.user_id, useraction_info.actiondetails);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}

/**
 * @brief 查询用户操作表的所有数据
 * 
 * @return std::vector<useraction_t> 查询到的用户操作表数据
 */
std::vector<useraction_t> search_useraction_info()
{
    std::vector<useraction_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from UserAction;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        useraction_t temp = {0};
        temp.action_id = char_to_uint16(row[0]);
        temp.user_id = char_to_uint32(row[1]);
        memcpy(temp.actiondetails, row[2], strlen(row[2]));
        temp.timestamp = row[3];
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
/**
 * @brief 根据ActionID更新一条用户操作数据到用户操作表
 * 
 * @param useraction_info 用户操作数据
 * @return true 更新成功
 * @return false 更新失败
 */
bool update_useraction_info(useraction_t& useraction_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update UserAction set UserID = %u, ActionDetails = '%s' where ActionID = %u",useraction_info.user_id, useraction_info.actiondetails, useraction_info.action_id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 按id来删除数据
 * 
 * @param action_id 
 * @return true 删除成功
 * @return false 删除失败
 */
bool delete_useraction_info(uint32_t action_id)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from UserAction where ActionID = %u", action_id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 查询表中未上传的数据
 * 
 * @return std::vector<useraction_t> 
 */
std::vector<useraction_t> search_useraction_not_upload_info()
{
    std::vector<useraction_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[100] = "select * from UserAction where isSync = false;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        useraction_t temp = {0};
        temp.action_id = char_to_uint16(row[0]);
        temp.user_id = char_to_uint32(row[1]);
        memcpy(temp.actiondetails, row[2], strlen(row[2]));
        temp.timestamp = row[3];
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
/**
 * @brief 根据action_id修改isSync为true，表示数据已经上传了
 * 
 * @param action_id 
 * @return true 
 * @return false 
 */
bool update_useraction_info_upload(uint32_t action_id)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update UserAction set isSync = true where ActionID = %u", action_id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 插入所有数据，包括timestamp
 * 
 * @param useraction_info 
 * @return true 
 * @return false 
 */
bool insert_all_useraction_info(useraction_t& useraction_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query, "insert into UserAction(ActionId, UserID, ActionDetails, Timestamp) "
            "values(%u, %u, %u, '%s', '%s')", 
            useraction_info.action_id, useraction_info.user_id, useraction_info.actiondetails, useraction_info.timestamp);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}