#include <cstdio>
#include <cstring>
#include "controllerrecord.h"
#include "database.h"
using namespace std;
#define LOG_FILE_MSG "[database controllerrecord.cpp]: "
/**
 * @brief 插入一条控制器数据到控制器表（除ID和Timestamp）
 * 
 * @param controller_info 控制器数据
 * @return true 插入成功
 * @return false 插入失败
 */
bool insert_controllerrecord_info(controllerrecord_t& controller_info,bool is_sync)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[300] = {0};
    sprintf(query, "insert into ControllerRecord(DeviceID, ControllerID, Controllertype, OutputVoltage, OutputCurrent, WellNum, datetime1, datetime2, datetime3, isSync) values(%u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", controller_info.device_id, controller_info.controller_id, controller_info.controllertype, controller_info.outputvoltage, controller_info.outputcurrent, controller_info.wellnum, controller_info.datetime1, controller_info.datetime2, controller_info.datetime3, is_sync);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}向ControllerRecord表中插入数据失败！",LOG_FILE_MSG);
      return false;
    }
    db.close();
    return true;
}

/**
 * @brief 查询控制器表的所有数据
 * 
 * @return std::vector<controllerrecord_t> 查询到的控制器表数据
 */
std::vector<controllerrecord_t> search_controllerrecord_info()
{
    std::vector<controllerrecord_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from ControllerRecord;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        controllerrecord_t temp = {0};
        temp.id = char_to_uint32(row[0]);
        temp.device_id = char_to_uint16(row[1]);
        temp.controller_id = char_to_uint16(row[2]);
        temp.controllertype = char_to_uint16(row[3]);
        temp.outputvoltage = char_to_uint16(row[4]);
        temp.outputcurrent = char_to_uint16(row[5]);
        temp.wellnum = char_to_uint16(row[6]);
        temp.datetime1 = char_to_uint16(row[7]);
        temp.datetime2 = char_to_uint16(row[8]);
        temp.datetime3 = char_to_uint16(row[9]);
        temp.timestamp = row[10];
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
/**
 * @brief 根据ID更新一条控制器数据到控制器表
 * 
 * @param controller_info 控制器数据
 * @return true 更新成功
 * @return false 更新失败
 */
bool update_controllerrecord_info(controllerrecord_t& controller_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update ControllerRecord set DeviceID = %u, ControllerID = %u, Controllertype = %u,OutputVoltage = %u, OutputCurrent = %u, WellNum = %u, datetime1 = %u, datetime2 = %u, datetime3 = %u where ID = %u", controller_info.device_id, controller_info.controller_id, controller_info.controllertype, controller_info.outputvoltage, controller_info.outputcurrent, controller_info.wellnum, controller_info.datetime1, controller_info.datetime2, controller_info.datetime3, controller_info.id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}

/**
 * @brief 按id来删除数据
 * 
 * @param id 
 * @return true 删除成功
 * @return false 删除失败
 */
bool delete_controllerrecord_info(uint32_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from ControllerRecord where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
bool detete_controllerrecord_info(const char* end_time){
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from ControllerRecord where Timestamp<='%s'", end_time);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;

}
/**
 * @brief 查询表中未上传的数据
 * 
 * @return std::vector<controllerrecord_t> 
 */
std::vector<controllerrecord_t> search_controllerrecord_not_upload_info()
{
    std::vector<controllerrecord_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[150] = "select * from ControllerRecord where isSync = false;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        controllerrecord_t temp = {0};
        temp.id = char_to_uint32(row[0]);
        temp.device_id = char_to_uint16(row[1]);
        temp.controller_id = char_to_uint16(row[2]);
        temp.controllertype = char_to_uint16(row[3]);
        temp.outputvoltage = char_to_uint16(row[4]);
        temp.outputcurrent = char_to_uint16(row[5]);
        temp.wellnum = char_to_uint16(row[6]);
        temp.datetime1 = char_to_uint16(row[7]);
        temp.datetime2 = char_to_uint16(row[8]);
        temp.datetime3 = char_to_uint16(row[9]);
        temp.timestamp = row[10];
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
bool update_controllerrecord_info_upload(uint16_t device_id)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update ControllerRecord set isSync = true where DeviceID = %u", device_id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 插入所有数据
 * 
 * @param controller_info 
 * @return true 
 * @return false 
 */
bool insert_all_controllerrecord_info(controllerrecord_t& controller_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[300] = {0};
    sprintf(query, "insert into Controller(ID, AddressID, ControllerID, Controllertype, OutputVoltage, OutputCurrent, WellNum, datetime1, datetime2, datetime3, Timestamp) values(%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s')", 
            controller_info.id, controller_info.device_id, controller_info.controller_id, controller_info.controllertype, controller_info.outputvoltage, controller_info.outputcurrent, controller_info.wellnum, controller_info.datetime1, controller_info.datetime2, controller_info.datetime3, controller_info.timestamp);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 根据自增编号来删除数据
*/
bool update_controllerrecord_upload(uint32_t id)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update ControllerRecord set isSync = true where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}将ControllerRecord表中isSync更新为true操作失败！但该数据已经成功发送至云端！",LOG_FILE_MSG);
      return false;
    }
    db.close();
    return true;

}
