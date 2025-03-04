#include <cstdio>
#include <cstring>
#include "well.h"
#include "database.h"

using namespace std;
#define LOG_FILE_MSG "[database well.cpp]: "
/**
 * @brief 插入一条井数据到井表(所有数据)
 * 
 * @param well_info 井数据
 * @return true 插入成功
 * @return false 插入失败
 */
bool insert_well_info(well_t& well_info)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query, "insert into Well(ID, WellId, WellName, LayerNumber, PowerStatus, StatusFlag, OperatingMode, ControlTraffic, ControlPressure, ControlOpening, ControlStatus, TrafficSetting) "
            "values(%u, %u, '%s', %u, %u, %u, %u, %f, %u, %u, %u, %f)", well_info.id, 
            well_info.well_id, well_info.name, well_info.layer_number, 
            well_info.power_status, well_info.status_flag, well_info.operating_mode, well_info.control_traffic, well_info.control_pressure, well_info.control_opening, well_info.control_status, well_info.traffic_setting);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}向well表中插入数据失败！",LOG_FILE_MSG);
      return false;
    }
    db.close();
    return true;
}

bool insert_wrecord_info(well_t& well_info, bool is_sync)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query, "insert into WellRecord(DeviceID, WellId, WellName, LayerNumber, PowerStatus, StatusFlag, OperatingMode, ControlTraffic, ControlPressure, ControlOpening, ControlStatus, TrafficSetting, isSync) "
            "values(%u, %u, '%s', %u, %u, %u, %u, %f, %u, %u, %u, %f, %u)", well_info.id, 
            well_info.well_id, well_info.name, well_info.layer_number, 
            well_info.power_status, well_info.status_flag, well_info.operating_mode, well_info.control_traffic, well_info.control_pressure, well_info.control_opening, well_info.control_status, well_info.traffic_setting, is_sync);
    if(!db.sql_query(query, strlen(query)))
    {
      db.close();
      logger->error("{}向WellRecord表中插入数据失败！",LOG_FILE_MSG);
      return false;
    } 
    db.close();
    return true;
}

/**
 * @brief 查询井表的所有数据
 * 
 * @return std::vector<well_t> 查询到的井表数据
 */
std::vector<well_t> search_well_info()
{
    std::vector<well_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from Well;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        well_t temp = {0};
        temp.id = char_to_uint16(row[0]);
        temp.well_id = char_to_uint16(row[1]);
        memcpy(temp.name, row[2], strlen(row[2]));
        temp.layer_number = char_to_uint16(row[3]);
        temp.power_status = char_to_uint16(row[4]);
        temp.status_flag = char_to_uint16(row[5]);
        temp.operating_mode = char_to_uint16(row[6]);
        temp.control_traffic = atof(row[7]);
        temp.control_pressure = char_to_uint16(row[8]);
        temp.control_opening = char_to_uint16(row[9]);
        temp.control_status = char_to_uint16(row[10]);
        temp.traffic_setting = atof(row[11]);
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}

bool search_well_not_info(uint16_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[150] = {0};
    sprintf(query,"select * from Well where Id = %u",id);
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
 * @brief 根据ID（Modbus地址）更新井表数据
 * 
 * @param well_info 井数据
 * @return true 更新成功
 * @return false 更新失败
 */
bool update_well_info(wellrecord_t& well_info,uint16_t id)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query,"update Well set WellID = %u, WellName = '%s',LayerNumber = %u, PowerStatus = %u, StatusFlag = %u, OperatingMode = %u, ControlTraffic = %f, ControlPressure = %u, ControlOpening = %u, ControlStatus = %u, TrafficSetting = %f  where ID = %u", well_info.well_id, well_info.name, well_info.layer_number, well_info.power_status, well_info.status_flag, well_info.operating_mode, well_info.control_traffic, well_info.control_pressure, well_info.control_opening, well_info.control_status, well_info.traffic_setting, id);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}向Well表中更新数据失败！",LOG_FILE_MSG);
      return false;

    }
    db.close();
    return true;
}

/**
 * @brief 按id来删除数据
 * 
 * @param id 
 * @return true 
 * @return false 
 */
bool delete_well_info(uint16_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[50] = {0};
    sprintf(query,"delete from Well where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 查询表中未上传的数据
 * 
 * @return std::vector<well_t> 
 */
std::vector<well_t> search_well_not_upload_info()
{
    std::vector<well_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from Well where isSync = false;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        well_t temp = {0};
        temp.id = char_to_uint16(row[0]);
        temp.well_id = char_to_uint16(row[1]);
        memcpy(temp.name, row[2], strlen(row[2]));
        temp.layer_number = char_to_uint16(row[3]);
        temp.power_status = char_to_uint16(row[4]);
        temp.status_flag = char_to_uint16(row[5]);
        temp.operating_mode = char_to_uint16(row[6]);
        temp.control_traffic = atof(row[7]);
        temp.control_pressure = char_to_uint16(row[8]);
        temp.control_opening = char_to_uint16(row[9]);
        temp.control_status = char_to_uint16(row[10]);
        temp.traffic_setting = atof(row[11]);
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
bool update_well_info_upload(uint16_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"update Well set isSync = true where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 插入所有数据
 * 
 * @param well_info 
 * @return true 
 * @return false 
 */
bool insert_all_well_info(well_t& well_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query, "insert into Well(ID, WellId, WellName, LayerNumber, PowerStatus, StatusFlag, OperatingMode, ControlTraffic, ControlPressure, ControlOpening, ControlStatus, TrafficSetting)values(%u, %u, '%s', %u, %u, %u, %u, %f, %u, %u, %u, %f)", well_info.id, well_info.well_id, well_info.name, well_info.layer_number, well_info.power_status, well_info.status_flag, well_info.operating_mode, well_info.control_traffic, well_info.control_pressure, well_info.control_opening, well_info.control_status, well_info.traffic_setting);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}