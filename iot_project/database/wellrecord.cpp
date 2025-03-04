#include <cstdio>
#include <cstring>
#include "wellrecord.h"
#include "database.h"
#define LOG_FILE_MSG "[database wellrecord.cpp]: "
using namespace std;

/**
 * @brief 插入一条井数据到井表
 * 
 * @param well_info 井数据
 * @return true 插入成功
 * @return false 插入失败
 */
bool insert_wellrecord_info(wellrecord_t& well_info,bool is_sync)
{
    Database db;
    if(!db.connect()) return false;
     auto logger = spdlog::get("project_log"); 
    char query[800] = {0};
    sprintf(query, "insert into WellRecord(DeviceID, WellId, WellName, LayerNumber, PowerStatus, StatusFlag, OperatingMode, ControlTraffic, ControlPressure, ControlOpening, ControlStatus, TrafficSetting, isSync) "
            "values(%u, %u, '%s', %u, %u, %u, %u, %f, %u, %u, %u, %f, %u)", well_info.device_id, well_info.well_id, well_info.name, well_info.layer_number, well_info.power_status, well_info.status_flag, well_info.operating_mode, well_info.control_traffic, well_info.control_pressure, well_info.control_opening, well_info.control_status, well_info.traffic_setting, is_sync);
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
 * @return std::vector<wellrecord_t> 查询到的井表数据
 */
std::vector<wellrecord_t> search_wellrecord_info()
{
    std::vector<wellrecord_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from WellRecord;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        wellrecord_t temp = {0};
        temp.id = char_to_uint32(row[0]);
        temp.device_id = char_to_uint16(row[1]);
        temp.well_id = char_to_uint16(row[2]);
        memcpy(temp.name, row[3], strlen(row[3]));
        temp.layer_number = char_to_uint16(row[4]);
        temp.power_status = char_to_uint16(row[5]);
        temp.status_flag = char_to_uint16(row[6]);
        temp.operating_mode = char_to_uint16(row[7]);
        temp.control_traffic = atof(row[8]);
        temp.control_pressure = char_to_uint16(row[9]);
        temp.control_opening = char_to_uint16(row[10]);
        temp.control_status = char_to_uint16(row[11]);
        temp.traffic_setting = atof(row[12]);
        temp.timestamp = row[13];
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
/**
 * @brief 根据ID更新井表数据
 * 
 * @param well_info 井数据
 * @return true 更新成功
 * @return false 更新失败
 */
bool update_wellrecord_info(wellrecord_t& well_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query,"update WellRecord set DeviceID = %u, WellID = %u, WellName = '%s',LayerNumber = %u, PowerStatus = %u, StatusFlag = %u, OperatingMode = %u, ControlTraffic = %f, ControlPressure = %u, ControlOpening = %u, ControlStatus = %u, TrafficSetting = %f, Timestamp = '%s'  where ID = %u", well_info.device_id, well_info.well_id, well_info.name, well_info.layer_number, well_info.power_status, well_info.status_flag, well_info.operating_mode, well_info.control_traffic, well_info.control_pressure, well_info.control_opening, well_info.control_status, well_info.traffic_setting, well_info.timestamp, well_info.id);
    if(!db.sql_query(query, strlen(query))) return false;
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
bool delete_wellrecord_info(uint32_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[50] = {0};
    sprintf(query,"delete from WellRecord where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}

bool delete_wellrecord_info(const char* end_time)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from WellRecord where Timestamp<='%s'", end_time);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}

/**
 * @brief 查询表中未上传的数据
 * 
 * @return std::vector<wellrecord_t> 
 */
std::vector<wellrecord_t> search_wellrecord_not_upload_info()
{
    std::vector<wellrecord_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from WellRecord where isSync = false;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        wellrecord_t temp = {0};
        temp.id = char_to_uint32(row[0]);
        temp.device_id = char_to_uint16(row[1]);
        temp.well_id = char_to_uint16(row[2]);
        memcpy(temp.name, row[3], strlen(row[3]));
        temp.layer_number = char_to_uint16(row[4]);
        temp.power_status = char_to_uint16(row[5]);
        temp.status_flag = char_to_uint16(row[6]);
        temp.operating_mode = char_to_uint16(row[7]);
        temp.control_traffic = atof(row[8]);
        temp.control_pressure = char_to_uint16(row[9]);
        temp.control_opening = char_to_uint16(row[10]);
        temp.control_status = char_to_uint16(row[11]);
        temp.traffic_setting = atof(row[12]);
        temp.timestamp = row[13];
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
bool update_wellrecord_info_upload(uint16_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"update WellRecord set isSync = true where DeviceID = %u", id);
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
bool insert_all_wellrecord_info(wellrecord_t& well_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query, "insert into WellRecord(ID, DeviceID, WellId, WellName, LayerNumber, PowerStatus, StatusFlag, OperatingMode, ControlTraffic, ControlPressure, ControlOpening, ControlStatus, TrafficSetting, Timestamp)values(%u, %u, %u, '%s', %u, %u, %u, %u, %f, %u, %u, %u, %f, '%s')", well_info.id, well_info.device_id, well_info.well_id, well_info.name, well_info.layer_number, well_info.power_status, well_info.status_flag, well_info.operating_mode, well_info.control_traffic, well_info.control_pressure, well_info.control_opening, well_info.control_status, well_info.traffic_setting, well_info.timestamp);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 根据自增编号来删除数据
*/
bool update_wellrecord_upload(uint32_t id)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"update WellRecord set isSync = true where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}将WellRecord表中isSync更新为true操作失败！但该数据已经成功发送至云端！",LOG_FILE_MSG);
      return false;
    }
    db.close();
    return true;
}