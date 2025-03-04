#include <cstdio>
#include <cstring>
#include "mandrelrecord.h"
#include "database.h"
using namespace std;
#define LOG_FILE_MSG "[database mandrelrecord.cpp]: "
/**
 * @brief 插入一条历史工作筒数据到工作筒记录表（不包括ID和Timestamp）
 * 
 * @param mandrelrecord_info 工作筒历史记录数据
 * @return true 插入成功
 * @return false 插入失败
 */
bool insert_mandrelrecord_info(mandrelrecord_t& mandrelrecord_info,bool is_sync) 
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query, "insert into MandrelRecord(DeviceID, WellID, Layer, Traffic, Temperature, InternalPressure, ExternalPressure, OpeningAccuracy, MoisturePercentage, Status, MotorStatus, MotorCurrent, Voltage, Current, OperatingMode, ControlStatus, YieldControl, YieldValue, UpFlowDeviation, DownFlowDeviation, AdjustmentMinutes, PressureTestControl, PressureTestHour, WaterAbsorptionTest, FaucetFlag, isSync) values(%u, %u, %u, %f, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %f, %u, %u, %u, %u, %u, %u, %u, %u)", mandrelrecord_info.device_id, mandrelrecord_info.wellid, mandrelrecord_info.layer, mandrelrecord_info.traffic, mandrelrecord_info.temperature, mandrelrecord_info.internalpressure, mandrelrecord_info.externalpressure, mandrelrecord_info.openingaccuracy, mandrelrecord_info.moisturepercentage, mandrelrecord_info.status,mandrelrecord_info.motorstatus, mandrelrecord_info.motorcurrent, mandrelrecord_info.voltage, mandrelrecord_info.current, mandrelrecord_info.operatingmode, mandrelrecord_info.controlstatus, mandrelrecord_info.yieldcontrol, mandrelrecord_info.yieldvalue, mandrelrecord_info.upflowdeviation, mandrelrecord_info.downflowdeviation, mandrelrecord_info.adjustmentminutes, mandrelrecord_info.pressuretestcontrol, mandrelrecord_info.pressuretesthour, mandrelrecord_info.waterabsorptiontest, mandrelrecord_info.faucetflag, is_sync);//%u格式化输出无符号整数类型的格式控制符
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}向MandrelRecord中插入数据失败！",LOG_FILE_MSG);
      return false;

    }
    db.close();
    return true;
}

/**
 * @brief 查询工作筒表的所有历史数据
 * 
 * @return std::vector<mandrelrecord_t> 查询到的工作筒表历史数据
 */

std::vector<mandrelrecord_t> search_mandrelrecord_info()
{
    std::vector<mandrelrecord_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from MandrelRecord;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        mandrelrecord_t temp = {0};
        temp.id = char_to_uint32(row[0]);
        temp.device_id = char_to_uint16(row[1]);
        temp.wellid = char_to_uint16(row[2]);
        temp.layer = char_to_uint16(row[3]);
        temp.traffic = atof(row[4]);
        temp.temperature = char_to_uint16(row[5]);
        temp.internalpressure = char_to_uint16(row[6]);
        temp.externalpressure = char_to_uint16(row[7]);
        temp.openingaccuracy = char_to_uint16(row[8]);
        temp.moisturepercentage = char_to_uint16(row[9]);
        temp.status = char_to_uint16(row[10]);
        temp.motorstatus = char_to_uint16(row[11]);
        temp.motorcurrent = char_to_uint16(row[12]);
        temp.voltage = char_to_uint16(row[13]);
        temp.current = char_to_uint16(row[14]);
        temp.operatingmode = char_to_uint16(row[15]);
        temp.controlstatus = char_to_uint16(row[16]);
        temp.yieldcontrol = char_to_uint16(row[17]);
        temp.yieldvalue = atof(row[18]);
        temp.upflowdeviation = char_to_uint16(row[19]);
        temp.downflowdeviation = char_to_uint16(row[20]);
        temp.adjustmentminutes = char_to_uint16(row[21]);
        temp.pressuretestcontrol = char_to_uint16(row[22]);
        temp.pressuretesthour = char_to_uint16(row[23]);
        temp.waterabsorptiontest = char_to_uint16(row[24]);
        temp.faucetflag = char_to_uint16(row[25]);
        temp.timestamp = row[26];
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
/**
 * @brief 根据ID更新工作筒数据
 * 
 * @param mandrelrecord_info 工作筒历史数据
 * @return true 更新成功
 * @return false 更新失败
 */

bool update_mandrelrecord_info(mandrelrecord_t& mandrelrecord_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query,"update MandrelRecord set DeviceID = %u, WellID = %u, Layer = %u, Traffic = %f, Temperature = %u, InternalPressure = %u, ExternalPressure = %u, OpeningAccuracy = %u, MoisturePercentage = %u, Status = %u, MotorStatus = %u, MotorCurrent = %u, Voltage = %u, Current = %u, OperatingMode = %u, ControlStatus = %u, YieldControl = %u, YieldValue = %f, UpFlowDeviation = %u, DownFlowDeviation = %u, AdjustmentMinutes = %u, PressureTestControl = %u, PressureTestHour = %u, WaterAbsorptionTest = %u, FaucetFlag = %u, Timestamp = '%s' where ID = %u",  mandrelrecord_info.device_id, mandrelrecord_info.wellid, mandrelrecord_info.layer, mandrelrecord_info.traffic, mandrelrecord_info.temperature, mandrelrecord_info.internalpressure, mandrelrecord_info.externalpressure, mandrelrecord_info.openingaccuracy, mandrelrecord_info.moisturepercentage, mandrelrecord_info.status, mandrelrecord_info.motorstatus, mandrelrecord_info.motorcurrent, mandrelrecord_info.voltage, mandrelrecord_info.current, mandrelrecord_info.operatingmode, mandrelrecord_info.controlstatus, mandrelrecord_info.yieldcontrol, mandrelrecord_info.yieldvalue, mandrelrecord_info.upflowdeviation, mandrelrecord_info.downflowdeviation, mandrelrecord_info.adjustmentminutes, mandrelrecord_info.pressuretestcontrol, mandrelrecord_info.pressuretesthour, mandrelrecord_info.waterabsorptiontest, mandrelrecord_info.faucetflag, mandrelrecord_info.timestamp, mandrelrecord_info.id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 按id来删除工作筒数据
 * 
 * @param mandrelrecord_info 工作筒数据
 * @return true 删除成功
 * @return false 删除失败
 */
bool delete_mandrelrecord_info(uint32_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[50] = {0};
    sprintf(query,"delete from MandrelRecord where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}

bool delete_mandrelrecord_info(const char* end_time)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from MandrelRecord where Timestamp<='%s'", end_time);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 查询表中未上传的数据
 * 
 * @return std::vector<mandrelrecord_t> 
 */
std::vector<mandrelrecord_t> search_mandrelrecord_not_upload_info()
{
    std::vector<mandrelrecord_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from MandrelRecord where isSync = false;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        mandrelrecord_t temp = {0};
        temp.id = char_to_uint32(row[0]);
        temp.device_id = char_to_uint16(row[1]);
        temp.wellid = char_to_uint16(row[2]);
        temp.layer = char_to_uint16(row[3]);
        temp.traffic = atof(row[4]);
        temp.temperature = char_to_uint16(row[5]);
        temp.internalpressure = char_to_uint16(row[6]);
        temp.externalpressure = char_to_uint16(row[7]);
        temp.openingaccuracy = char_to_uint16(row[8]);
        temp.moisturepercentage = char_to_uint16(row[9]);
        temp.status = char_to_uint16(row[10]);
        temp.motorstatus = char_to_uint16(row[11]);
        temp.motorcurrent = char_to_uint16(row[12]);
        temp.voltage = char_to_uint16(row[13]);
        temp.current = char_to_uint16(row[14]);
        temp.operatingmode = char_to_uint16(row[15]);
        temp.controlstatus = char_to_uint16(row[16]);
        temp.yieldcontrol = char_to_uint16(row[17]);
        temp.yieldvalue = atof(row[18]);
        temp.upflowdeviation = char_to_uint16(row[19]);
        temp.downflowdeviation = char_to_uint16(row[20]);
        temp.adjustmentminutes = char_to_uint16(row[21]);
        temp.pressuretestcontrol = char_to_uint16(row[22]);
        temp.pressuretesthour = char_to_uint16(row[23]);
        temp.waterabsorptiontest = char_to_uint16(row[24]);
        temp.faucetflag = char_to_uint16(row[25]);
        temp.timestamp = row[26];
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
bool update_mandrelrecord_info_upload(uint16_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update MandrelRecord set isSync = true where DeviceID = %u", id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 根据自增编号来删除数据
*/
bool update_mandrelrecord_upload(uint32_t id)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update MandrelRecord set isSync = true where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}将MandrelRecord表中isSync更新为true操作失败！但该数据已经成功发送至云端！",LOG_FILE_MSG);
      return false;
    }
    db.close();
    return true;
}
/**
 * @brief 插入所有数据
 * 
 * @param mandrelrecord_info 
 * @return true 
 * @return false 
 */
bool insert_all_mandrelrecord_info(mandrelrecord_t& mandrelrecord_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query, "insert into MandrelRecord(ID, DeviceID, WellID, Layer, Traffic, Temperature, InternalPressure, ExternalPressure, OpeningAccuracy, MoisturePercentage, Status, MotorStatus, MotorCurrent, Voltage, Current, OperatingMode, ControlStatus, YieldControl, YieldValue, UpFlowDeviation, DownFlowDeviation, AdjustmentMinutes, PressureTestControl, PressureTestHour, WaterAbsorptionTest, FaucetFlag, Timestamp) values(%u, %u, %u, %u, %f, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %f, %u, %u, %u, %u, %u, %u, %u, '%s')", mandrelrecord_info.id, mandrelrecord_info.device_id, mandrelrecord_info.wellid, mandrelrecord_info.layer, mandrelrecord_info.traffic, mandrelrecord_info.temperature, mandrelrecord_info.internalpressure, mandrelrecord_info.externalpressure, mandrelrecord_info.openingaccuracy, mandrelrecord_info.moisturepercentage, mandrelrecord_info.status,mandrelrecord_info.motorstatus, mandrelrecord_info.motorcurrent, mandrelrecord_info.voltage, mandrelrecord_info.current, mandrelrecord_info.operatingmode, mandrelrecord_info.controlstatus, mandrelrecord_info.yieldcontrol, mandrelrecord_info.yieldvalue, mandrelrecord_info.upflowdeviation, mandrelrecord_info.downflowdeviation, mandrelrecord_info.adjustmentminutes, mandrelrecord_info.pressuretestcontrol, mandrelrecord_info.pressuretesthour, mandrelrecord_info.waterabsorptiontest, mandrelrecord_info.faucetflag, mandrelrecord_info.timestamp);//%u格式化输出无符号整数类型的格式控制符
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}