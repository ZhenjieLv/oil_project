#include <cstdio>
#include <cstring>
#include "controller.h"
#include "database.h"
using namespace std;
#define LOG_FILE_MSG "[database controller.cpp]: "
/**
 * @brief 插入一条控制器数据（所有数据）到控制器表
 * 
 * @param controller_info 控制器数据
 * @return true 插入成功
 * @return false 插入失败
 */
bool insert_controller_info(controller_t& controller_info)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[300] = {0};
    sprintf(query, "insert into Controller(ID, ControllerID, Controllertype, OutputVoltage, OutputCurrent, WellNum, datetime1, datetime2, datetime3) values(%u, %u, %u, %u, %u, %u, %u, %u, %u)", 
            controller_info.id, controller_info.controller_id, controller_info.controllertype, controller_info.outputvoltage, controller_info.outputcurrent, controller_info.wellnum, controller_info.datetime1, controller_info.datetime2, controller_info.datetime3);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}向Controller表中插入数据失败！",LOG_FILE_MSG);
      return false;

    }
    db.close();
    return true;
}
/**
 * @brief 插入一条控制器历史信息到ControllerRecord表
 * 
 * @param controller_info 控制器数据
 * @return true 插入成功
 * @return false 插入失败
 */
bool insert_crecord_info(controller_t& controller_info,bool is_sync)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[300] = {0};
    sprintf(query, "insert into ControllerRecord(DeviceID, ControllerID, Controllertype, OutputVoltage, OutputCurrent, WellNum, datetime1, datetime2, datetime3, isSync) values(%u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", 
            controller_info.id, controller_info.controller_id, controller_info.controllertype, controller_info.outputvoltage, controller_info.outputcurrent, controller_info.wellnum, controller_info.datetime1, controller_info.datetime2, controller_info.datetime3, is_sync);
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
 * @return std::vector<controller_t> 查询到的控制器表数据
 */
std::vector<controller_t> search_controller_info()
{
    std::vector<controller_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from Controller;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        controller_t temp = {0};
        temp.id = char_to_uint16(row[0]);
        temp.controller_id = char_to_uint16(row[1]);
        temp.controllertype = char_to_uint16(row[2]);
        temp.outputvoltage = char_to_uint16(row[3]);
        temp.outputcurrent = char_to_uint16(row[4]);
        temp.wellnum = char_to_uint16(row[5]);
        temp.datetime1 = char_to_uint16(row[6]);
        temp.datetime2 = char_to_uint16(row[7]);
        temp.datetime3 = char_to_uint16(row[8]);
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
/**
 * @brief 根据ID(Modbus地址)更新一条控制器数据到控制器表
 * 
 * @param controller_info 控制器数据
 * @return true 更新成功
 * @return false 更新失败
 */
bool update_controller_info(controllerrecord_t& controller_info,uint16_t id)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update Controller set ControllerID = %u, Controllertype = %u,OutputVoltage = %u, OutputCurrent = %u, WellNum = %u, datetime1 = %u, datetime2 = %u, datetime3 = %u  where ID = %u", controller_info.controller_id, controller_info.controllertype, controller_info.outputvoltage, controller_info.outputcurrent, controller_info.wellnum, controller_info.datetime1, controller_info.datetime2, controller_info.datetime3, id);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}更新Controller表失败！",LOG_FILE_MSG);
      return false;

    }
    db.close();
    return true;
}

/**
 * @brief 按id（Modbus地址）来删除数据
 * 
 * @param controller_info 控制器数据
 * @return true 删除成功
 * @return false 删除失败
 */
bool delete_controller_info(uint32_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from Controller where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 查询表中未上传的数据
 * 
 * @return std::vector<controller_t> 
 */
std::vector<controller_t> search_controller_not_upload_info()
{
    std::vector<controller_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[150] = "select * from Controller where isSync = false;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        controller_t temp = {0};
        temp.id = char_to_uint16(row[0]);
        temp.controller_id = char_to_uint16(row[1]);
        temp.controllertype = char_to_uint16(row[2]);
        temp.outputvoltage = char_to_uint16(row[3]);
        temp.outputcurrent = char_to_uint16(row[4]);
        temp.wellnum = char_to_uint16(row[5]);
        temp.datetime1 = char_to_uint16(row[6]);
        temp.datetime2 = char_to_uint16(row[7]);
        temp.datetime3 = char_to_uint16(row[8]);
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}

/**
 * @brief 查询表中是不是没有指定id数据，没有返回true
 * 
 * @return bool
 */

bool search_controller_not_info(uint16_t id)
{
  Database db;
  if(!db.connect()) return false;
  char query[150] = {0};
  sprintf(query,"select * from Controller where Id = %u", id);
  int result;
  result = db.check_search_not_info(query, strlen(query));
   // MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == -1) {
        db.close();
        return true;
    }
  //  mysql_free_result(result);
  db.close();
  return false;
}
/**
 * @brief 根据id修改isSync为true，表示数据已经上传了
 * 
 * @param id 
 * @return true 
 * @return false 
 */
bool update_controller_info_upload(uint32_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update Controller set isSync = true where ID = %u", id);
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
bool insert_all_controller_info(controller_t& controller_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[300] = {0};
    sprintf(query, "insert into Controller(ID, ControllerID, Controllertype, OutputVoltage, OutputCurrent, WellNum, datetime1, datetime2, datetime3) values(%u, %u, %u, %u, %u, %u, %u, %u, %u)", controller_info.id, controller_info.controller_id, controller_info.controllertype, controller_info.outputvoltage, controller_info.outputcurrent, controller_info.wellnum, controller_info.datetime1, controller_info.datetime2, controller_info.datetime3);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
