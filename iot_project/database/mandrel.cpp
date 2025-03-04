#include <cstdio>
#include <cstring>
#include "mandrel.h"
#include "database.h"
using namespace std;
#define LOG_FILE_MSG "[database mandrel.cpp]: "
/**
 * @brief 插入一条工作筒数据到工作筒表
 * 
 * @param mandrel_info 工作筒数据
 * @return true 插入成功
 * @return false 插入失败
 */
bool insert_mandrel_info(mandrel_t& mandrel_info) //插入工作筒内全部数据
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[1500] = {0};
    sprintf(query, "insert into Mandrel(ID, WellID, Layer, Traffic, Temperature, InternalPressure, ExternalPressure, OpeningAccuracy, MoisturePercentage, Status, MotorStatus, MotorCurrent, Voltage, Current, OperatingMode, ControlStatus, YieldControl, YieldValue, UpFlowDeviation, DownFlowDeviation, AdjustmentMinutes, PressureTestControl, PressureTestHour, WaterAbsorptionTest, FaucetFlag) values(%u, %u, %u, %f, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %f, %u, %u, %u, %u, %u, %u, %u)", mandrel_info.id, mandrel_info.wellid, mandrel_info.layer, mandrel_info.traffic, mandrel_info.temperature, mandrel_info.internalpressure, mandrel_info.externalpressure, mandrel_info.openingaccuracy, mandrel_info.moisturepercentage, mandrel_info.status, mandrel_info.motorstatus, mandrel_info.motorcurrent, mandrel_info.voltage, mandrel_info.current, mandrel_info.operatingmode, mandrel_info.controlstatus, mandrel_info.yieldcontrol, mandrel_info.yieldvalue, mandrel_info.upflowdeviation, mandrel_info.downflowdeviation, mandrel_info.adjustmentminutes, mandrel_info.pressuretestcontrol, mandrel_info.pressuretesthour, mandrel_info.waterabsorptiontest, mandrel_info.faucetflag);//%u格式化输出无符号整数类型的格式控制符
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}向Mandrel表中插入数据失败!",LOG_FILE_MSG);
      return false;
    }
    db.close();
    return true;
}

bool insert_mrecord_info(mandrel_t& mandrel_info, bool is_sync)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[1500] = {0};
    sprintf(query, "insert into MandrelRecord(DeviceID, WellID, Layer, Traffic, Temperature, InternalPressure, ExternalPressure, OpeningAccuracy, MoisturePercentage, Status, MotorStatus, MotorCurrent, Voltage, Current, OperatingMode, ControlStatus, YieldControl, YieldValue, UpFlowDeviation, DownFlowDeviation, AdjustmentMinutes, PressureTestControl, PressureTestHour, WaterAbsorptionTest, FaucetFlag, isSync) values(%u, %u, %u, %f, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %f, %u, %u, %u, %u, %u, %u, %u, %u)", mandrel_info.id, mandrel_info.wellid, mandrel_info.layer, mandrel_info.traffic, mandrel_info.temperature, mandrel_info.internalpressure, mandrel_info.externalpressure, mandrel_info.openingaccuracy, mandrel_info.moisturepercentage, mandrel_info.status, mandrel_info.motorstatus, mandrel_info.motorcurrent, mandrel_info.voltage, mandrel_info.current, mandrel_info.operatingmode, mandrel_info.controlstatus, mandrel_info.yieldcontrol, mandrel_info.yieldvalue, mandrel_info.upflowdeviation, mandrel_info.downflowdeviation, mandrel_info.adjustmentminutes, mandrel_info.pressuretestcontrol, mandrel_info.pressuretesthour, mandrel_info.waterabsorptiontest, mandrel_info.faucetflag, is_sync);//%u格式化输出无符号整数类型的格式控制符
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}向MandrelRecord表中插入数据失败!",LOG_FILE_MSG);
      return false;

    }
    db.close();
    return true;
}

/**
 * @brief 查询工作筒表的所有数据
 * 
 * @return std::vector<mandrel_t> 查询到的工作筒表数据
 */

std::vector<mandrel_t> search_mandrel_info()//查询工作筒内全部数据
{
    std::vector<mandrel_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from Mandrel;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        mandrel_t temp = {0};
        temp.id = char_to_uint16(row[0]);
        temp.wellid = char_to_uint16(row[1]);
        temp.layer = char_to_uint16(row[2]);
        temp.traffic = atof(row[3]);
        temp.temperature = char_to_uint16(row[4]);
        temp.internalpressure = char_to_uint16(row[5]);
        temp.externalpressure = char_to_uint16(row[6]);
        temp.openingaccuracy = char_to_uint16(row[7]);
        temp.moisturepercentage = char_to_uint16(row[8]);
        temp.status = char_to_uint16(row[9]);
        temp.motorstatus = char_to_uint16(row[10]);
        temp.motorcurrent = char_to_uint16(row[11]);
        temp.voltage = char_to_uint16(row[12]);
        temp.current = char_to_uint16(row[13]);
        temp.operatingmode = char_to_uint16(row[14]);
        temp.controlstatus = char_to_uint16(row[15]);
        temp.yieldcontrol = char_to_uint16(row[16]);
        temp.yieldvalue = atof(row[17]);
        temp.upflowdeviation = char_to_uint16(row[18]);
        temp.downflowdeviation = char_to_uint16(row[19]);
        temp.adjustmentminutes = char_to_uint16(row[20]);
        temp.pressuretestcontrol = char_to_uint16(row[21]);
        temp.pressuretesthour = char_to_uint16(row[22]);
        temp.waterabsorptiontest = char_to_uint16(row[23]);
        temp.faucetflag = char_to_uint16(row[24]);
        ret.push_back(temp);
    }
    mysql_free_result(result);
    db.close();
    return ret;
}
bool search_mandrel_not_info(uint16_t id)//查询工作筒内全部数据
{
    Database db;
    if(!db.connect()) return false;
    char query[1500] = {0};
    sprintf(query,"select * from Mandrel where ID = %u",id);
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
 * @brief 根据ID（Modbus地址）更新工作筒数据
 * 
 * @param mandrel_info 工作筒数据
 * @return true 更新成功
 * @return false 更新失败
 */
bool update_mandrel_info(mandrelrecord_t& mandrel_info,uint16_t id)
{
    Database db;
    if(!db.connect()) return false;
    auto logger = spdlog::get("project_log");
    char query[800] = {0};
    sprintf(query,"update Mandrel set WellID = %u, Layer = %u, Traffic = %f, Temperature = %u, InternalPressure = %u, ExternalPressure = %u, OpeningAccuracy = %u, MoisturePercentage = %u, Status = %u, MotorStatus = %u, MotorCurrent = %u, Voltage = %u, Current = %u, OperatingMode = %u, ControlStatus = %u, YieldControl = %u, YieldValue = %f, UpFlowDeviation = %u, DownFlowDeviation = %u, AdjustmentMinutes = %u, PressureTestControl = %u, PressureTestHour = %u, WaterAbsorptionTest = %u, FaucetFlag = %u where ID = %u",  mandrel_info.wellid, mandrel_info.layer, mandrel_info.traffic, mandrel_info.temperature, mandrel_info.internalpressure, mandrel_info.externalpressure, mandrel_info.openingaccuracy, mandrel_info.moisturepercentage, mandrel_info.status, mandrel_info.motorstatus, mandrel_info.motorcurrent, mandrel_info.voltage, mandrel_info.current, mandrel_info.operatingmode, mandrel_info.controlstatus, mandrel_info.yieldcontrol, mandrel_info.yieldvalue, mandrel_info.upflowdeviation, mandrel_info.downflowdeviation, mandrel_info.adjustmentminutes, mandrel_info.pressuretestcontrol, mandrel_info.pressuretesthour, mandrel_info.waterabsorptiontest, mandrel_info.faucetflag, id);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}向Mandrel表中更新数据失败！",LOG_FILE_MSG);
      return false;
    }
    db.close();
    return true;
}
/**
 * @brief 按id来删除工作筒数据
 * 
 * @param mandrel_info 工作筒数据
 * @return true 删除成功
 * @return false 删除失败
 */
bool delete_mandrel_info(uint32_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from Mandrel where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 查询表中未上传的数据
 * 
 * @return std::vector<mandrel_t> 
 */
std::vector<mandrel_t> search_mandrel_not_upload_info()
{
    std::vector<mandrel_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[100] = "select * from Mandrel where isSync = false;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        mandrel_t temp = {0};
        temp.id = char_to_uint16(row[0]);
        temp.wellid = char_to_uint16(row[1]);
        temp.layer = char_to_uint16(row[2]);
        temp.traffic = atof(row[3]);
        temp.temperature = char_to_uint16(row[4]);
        temp.internalpressure = char_to_uint16(row[5]);
        temp.externalpressure = char_to_uint16(row[6]);
        temp.openingaccuracy = char_to_uint16(row[7]);
        temp.moisturepercentage = char_to_uint16(row[8]);
        temp.status = char_to_uint16(row[9]);
        temp.motorstatus = char_to_uint16(row[10]);
        temp.motorcurrent = char_to_uint16(row[11]);
        temp.voltage = char_to_uint16(row[12]);
        temp.current = char_to_uint16(row[13]);
        temp.operatingmode = char_to_uint16(row[14]);
        temp.controlstatus = char_to_uint16(row[15]);
        temp.yieldcontrol = char_to_uint16(row[16]);
        temp.yieldvalue = atof(row[17]);
        temp.upflowdeviation = char_to_uint16(row[18]);
        temp.downflowdeviation = char_to_uint16(row[19]);
        temp.adjustmentminutes = char_to_uint16(row[20]);
        temp.pressuretestcontrol = char_to_uint16(row[21]);
        temp.pressuretesthour = char_to_uint16(row[22]);
        temp.waterabsorptiontest = char_to_uint16(row[23]);
        temp.faucetflag = char_to_uint16(row[24]);
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
bool update_mandrel_info_upload(uint32_t id)
{
    Database db;
    if(!db.connect()) return false;
    char query[60] = {0};
    sprintf(query,"update Mandrel set isSync = true where ID = %u", id);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}
/**
 * @brief 插入所有数据
 * 
 * @param mandrel_info 
 * @return true 
 * @return false 
 */
bool insert_all_mandrel_info(mandrel_t& mandrel_info)
{
    Database db;
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query, "insert into Mandrel(ID, WellID, Layer, Traffic, Temperature, InternalPressure, ExternalPressure, OpeningAccuracy, MoisturePercentage, Status, MotorStatus, MotorCurrent, Voltage, Current, OperatingMode, ControlStatus, YieldControl, YieldValue, UpFlowDeviation, DownFlowDeviation, AdjustmentMinutes, PressureTestControl, PressureTestHour, WaterAbsorptionTest, FaucetFlag) values(%u, %u, %u, %f, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %f, %u, %u, %u, %u, %u, %u, %u)", mandrel_info.id, mandrel_info.wellid, mandrel_info.layer, mandrel_info.traffic, mandrel_info.temperature, mandrel_info.internalpressure, mandrel_info.externalpressure, mandrel_info.openingaccuracy, mandrel_info.moisturepercentage, mandrel_info.status, mandrel_info.motorstatus, mandrel_info.motorcurrent, mandrel_info.voltage, mandrel_info.current, mandrel_info.operatingmode, mandrel_info.controlstatus, mandrel_info.yieldcontrol, mandrel_info.yieldvalue, mandrel_info.upflowdeviation, mandrel_info.downflowdeviation, mandrel_info.adjustmentminutes, mandrel_info.pressuretestcontrol, mandrel_info.pressuretesthour, mandrel_info.waterabsorptiontest, mandrel_info.faucetflag);//%u格式化输出无符号整数类型的格式控制符
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}