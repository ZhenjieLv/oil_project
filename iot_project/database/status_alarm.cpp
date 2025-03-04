#include <cstdio>
#include <cstring>
#include "status_alarm.h"
#include "database.h"
#define LOG_FILE_MSG "[database status_alarm.cpp]: "
using namespace std;

/**
 * @brief 插入一条设备状态报警数据到设备状态报警表
 * 
 * @param status_alarm_info 状态数据
 * @return true 插入成功
 * @return false 插入失败
 */
bool insert_status_alarm_info(uint16_t device_id,char *reason,bool is_sync)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query, "insert into StatusAlarm(DeviceID, Reason, isSync) "
            "values(%u, '%s',%u)", device_id, reason, is_sync);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}

bool insert_status_alarm_info(uint16_t device_id,char *reason)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query, "insert into StatusAlarm(DeviceID, Reason) "
            "values(%u, '%s')", device_id, reason);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}

/**
 * @brief 查询设备状态报警表的所有数据
 * 
 * @return std::vector<status_alarm_t> 查询到的状态表数据
 */
std::vector<status_alarm_t> search_status_alarm_info()
{
    std::vector<status_alarm_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from StatusAlarm;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        status_alarm_t temp = {0};
        temp.alarm_id = char_to_uint32(row[0]);
        temp.timestamp = row[1];
        temp.device_id = char_to_uint16(row[2]);
        memcpy(temp.reason, row[3], strlen(row[3]));
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
/**
 * @brief 更新一条设备状态报警数据到设备状态报警表
 * 
 * @param status_alarm_info 设备状态报警数据
 * @return true 更新成功
 * @return false 更新失败
 */
bool update_status_alarm_info(status_alarm_t& status_alarm_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update StatusAlarm set DeviceID = %u,Reason = '%s' where AlarmID = %u",status_alarm_info.device_id, status_alarm_info.reason, status_alarm_info.alarm_id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 删除一条设备状态报警数据
 * 
 * @param status_alarm_info 设备状态报警数据
 * @return true 删除成功
 * @return false 删除失败
 */
bool delete_status_alarm_info(uint32_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from StatusAlarm where AlarmID = %u", id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 查询表中未上传的数据
 * 
 * @return std::vector<status_alarm_t> 
 */
std::vector<status_alarm_t> search_status_alarm_not_upload_info()
{
    std::vector<status_alarm_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from StatusAlarm where isSync = false;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        status_alarm_t temp = {0};
        temp.alarm_id = char_to_uint32(row[0]);
        temp.timestamp = row[1];
        temp.device_id = char_to_uint16(row[2]);
        memcpy(temp.reason, row[3], strlen(row[3]));
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}

std::vector<status_alarm_t> search_alarm_id()
{
    std::vector<status_alarm_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[100] = "select * from StatusAlarm where AlarmID = (select max(AlarmID) from StatusAlarm);";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        status_alarm_t temp = {0};
        temp.alarm_id = char_to_uint32(row[0]);
        temp.timestamp = row[1];
        temp.device_id = char_to_uint16(row[2]);
        memcpy(temp.reason, row[3], strlen(row[3]));
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
/**
 * @brief 根据id修改isSync为true，表示数据已经上传了
 * 
 * @param id 
 * @return true 
 * @return false 
 */
bool update_status_alarm_info_upload(uint32_t id)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update StatusAlarm set isSync = true where AlarmID = %u", id);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}将StatusAlarm表中isSync更新为true操作失败！但该数据已经成功发送至云端！",LOG_FILE_MSG);
      return false;
    }
    db.close();
    return true;
}
/**
 * @brief 插入所有数据，包括timestamp
 * 
 * @param status_alarm_info
 * @return true 
 * @return false 
 */
bool insert_all_status_alarm_info(status_alarm_t& status_alarm_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query, "insert into StatusAlarm(AlarmID, Timestamp, DeviceID, Reason) "
            "values(%u, '%s', %u, '%s')", status_alarm_info.alarm_id, status_alarm_info.timestamp, 
            status_alarm_info.device_id, status_alarm_info.reason);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
bool detete_status_alarm_info(const char* end_time){
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from StatusAlarm where Timestamp<='%s'", end_time);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;

}