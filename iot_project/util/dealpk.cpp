#include "backend.h"
#include <cstdio>
using namespace std;
#define LOG_FILE_MSG "[util dealpk.cpp]: "
map<uint32_t, shared_ptr<Package>> sent_package; // 发送一个查询/控制报文，就将其存到这个map里，用于查询/控制报文不到反馈时，再发一遍
map<uint32_t, shared_ptr<Package>> recv_package; // 接收反馈报文
uint32_t rec_num = 0;                            // 定义一个全局变量，用于记录报文序号
int selnum = 0;                                  // 定义一个全局变量，用于定时发送报文
map<uint32_t, shared_ptr<Package>> cloud_action_record;
map<uint32_t, shared_ptr<Package>> cloud_package;
string JsonAsString(const Json::Value &json)
{
    std::string result;
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = ""; // Optional
    result = Json::writeString(wbuilder, json);
    return result;
}
/**
 * 解析井的数据并将其插入到well表和wellrecord表中
 */
// bool parseWell(shared_ptr<Package> package, bool is_sync)
// {
//     well_t well_info;
//     uint8_t *ptr = package->get_data();
//     well_info.id = package->get_src_id();
//     well_info.well_id = AV_RL16(ptr);
//     string str(ptr + 2, ptr + 2 + 20);
//     strcpy(well_info.name, str.data());
//     // memcpy(&well_info.name, ptr + 2, 20);
//     well_info.layer_number = AV_RL16(ptr + 22);
//     well_info.power_status = AV_RL16(ptr + 24);
//     well_info.status_flag = AV_RL16(ptr + 26);
//     well_info.operating_mode = AV_RL16(ptr + 28);
//     well_info.control_traffic = *((float *)(ptr + 30));
//     well_info.control_pressure = AV_RL16(ptr + 34);
//     well_info.control_opening = AV_RL16(ptr + 36);
//     well_info.control_status = AV_RL16(ptr + 38);
//     well_info.traffic_setting = *(float *)(ptr + 40);
//     /*if (!insert_well_info(well_info)) return false;
//     return true;*/
//     if (insert_well_info(well_info) && insert_wrecord_info(well_info, is_sync))
//         return true;
//     return false;
// }
string parse_well_to_kafka(shared_ptr<Package> package)
{
    auto logger = spdlog::get("project_log");
    Json::Value root;
    Json::Value schema;
    Json::Value payload;
    Json::Value data1;
    uint8_t *data = nullptr;
    data=(uint8_t *)malloc(4*sizeof(uint8_t)); 
    uint8_t *digital = nullptr;
    digital=(uint8_t *)malloc(4*sizeof(uint8_t));
    schema["type"] = "struct";
    data1["field"] = "DeviceID";
    data1["type"] = "int32";
    Json::Value data2;
    data2["field"] = "WellID";
    data2["type"] = "int32";
    Json::Value data3;
    data3["field"] = "WellName";
    data3["type"] = "string";
    Json::Value data4;
    data4["field"] = "LayerNumber";
    data4["type"] = "int32";
    Json::Value data5;
    data5["field"] = "PowerStatus";
    data5["type"] = "int32";
    Json::Value data6;
    data6["field"] = "StatusFlag";
    data6["type"] = "int32";
    Json::Value data7;
    data7["field"] = "OperatingMode";
    data7["type"] = "int32";
    Json::Value data8;
    data8["field"] = "ControlTraffic";
    data8["type"] = "float";
    Json::Value data9;
    data9["field"] = "ControlPressure";
    data9["type"] = "int32";
    Json::Value data10;
    data10["field"] = "ControlOpening";
    data10["type"] = "int32";
    Json::Value data11;
    data11["field"] = "ControlStatus";
    data11["type"] = "int32";
    Json::Value data12;
    data12["field"] = "TrafficSetting";
    data12["type"] = "float";
    schema["fields"].append(data1);
    schema["fields"].append(data2);
    schema["fields"].append(data3);
    schema["fields"].append(data4);
    schema["fields"].append(data5);
    schema["fields"].append(data6);
    schema["fields"].append(data7);
    schema["fields"].append(data8);
    schema["fields"].append(data9);
    schema["fields"].append(data10);
    schema["fields"].append(data11);
    schema["fields"].append(data12);
    root["schema"] = schema;
    uint8_t *ptr = package->get_data();
    payload["DeviceID"] = package->get_src_id();
    payload["WellID"] = AV_RB16(ptr);
    string str(ptr + 2, ptr + 2 + 20);
    payload["WellName"] = str;
    payload["LayerNumber"] = AV_RB16(ptr + 22);
    payload["PowerStatus"] = AV_RB16(ptr + 24);
    payload["StatusFlag"] = AV_RB16(ptr + 26);
    payload["OperatingMode"] = AV_RB16(ptr + 28);
    data[0] = ptr[31];
    data[1] = ptr[30];
    data[2] =  ptr[33];
    data[3] = ptr[32];
    payload["ControlTraffic"] = *(float *)data;
    payload["ControlPressure"] = AV_RB16(ptr + 34);
    payload["ControlOpening"] = AV_RB16(ptr + 36);
    payload["ControlStatus"] = AV_RB16(ptr + 38);
    digital[0] = ptr[41];
    digital[1] = ptr[40];
    digital[2] =  ptr[43];
    digital[3] = ptr[42];
    payload["TrafficSetting"] = *(float *)digital;
    root["payload"] = payload;
    string s = JsonAsString(root);
    free(data);
    free(digital);
    data = nullptr;
    digital = nullptr;
    return s;
}
string parse_mandrel_to_kafka(shared_ptr<Package> package)
{
    Json::Value root;
    Json::Value schema;
    Json::Value payload;
    Json::Value data1;
    uint8_t *data;
    data = nullptr;
    data=(uint8_t *)malloc(4*sizeof(uint8_t)); 
    uint8_t *digital;
    digital = nullptr;
    digital=(uint8_t *)malloc(4*sizeof(uint8_t));
    schema["type"] = "struct";
    data1["field"] = "DeviceID";
    data1["type"] = "int32";
    Json::Value data2;
    data2["field"] = "WellID";
    data2["type"] = "int32";
    Json::Value data3;
    data3["field"] = "Layer";
    data3["type"] = "int32";
    Json::Value data4;
    data4["field"] = "Traffic";
    data4["type"] = "float";
    Json::Value data5;
    data5["field"] = "Temperature";
    data5["type"] = "int32";
    Json::Value data6;
    data6["field"] = "InternalPressure";
    data6["type"] = "int32";
    Json::Value data7;
    data7["field"] = "ExternalPressure";
    data7["type"] = "int32";
    Json::Value data8;
    data8["field"] = "OpeningAccuracy";
    data8["type"] = "int32";
    Json::Value data9;
    data9["field"] = "MoisturePercentage";
    data9["type"] = "int32";
    Json::Value data10;
    data10["field"] = "Status";
    data10["type"] = "int32";
    Json::Value data11;
    data11["field"] = "MotorStatus";
    data11["type"] = "int32";
    Json::Value data12;
    data12["field"] = "MotorCurrent";
    data12["type"] = "int32";
    Json::Value data13;
    data13["field"] = "Voltage";
    data13["type"] = "int32";
    Json::Value data14;
    data14["field"] = "Current";
    data14["type"] = "int32";
    Json::Value data15;
    data15["field"] = "OperatingMode";
    data15["type"] = "int32";
    Json::Value data16;
    data16["field"] = "ControlStatus";
    data16["type"] = "int32";
    Json::Value data17;
    data17["field"] = "YieldControl";
    data17["type"] = "int32";
    Json::Value data18;
    data18["field"] = "YieldValue";
    data18["type"] = "float";
    Json::Value data19;
    data19["field"] = "UpFlowDeviation";
    data19["type"] = "int32";
    Json::Value data20;
    data20["field"] = "DownFlowDeviation";
    data20["type"] = "int32";
    Json::Value data21;
    data21["field"] = "AdjustmentMinutes";
    data21["type"] = "int32";
    Json::Value data22;
    data22["field"] = "PressureTestControl";
    data22["type"] = "int32";
    Json::Value data23;
    data23["field"] = "PressureTestHour";
    data23["type"] = "int32";
    Json::Value data24;
    data24["field"] = "WaterAbsorptionTest";
    data24["type"] = "int32";
    Json::Value data25;
    data25["field"] = "FaucetFlag";
    data25["type"] = "int32";
    schema["fields"].append(data1);
    schema["fields"].append(data2);
    schema["fields"].append(data3);
    schema["fields"].append(data4);
    schema["fields"].append(data5);
    schema["fields"].append(data6);
    schema["fields"].append(data7);
    schema["fields"].append(data8);
    schema["fields"].append(data9);
    schema["fields"].append(data10);
    schema["fields"].append(data11);
    schema["fields"].append(data12);
    schema["fields"].append(data13);
    schema["fields"].append(data14);
    schema["fields"].append(data15);
    schema["fields"].append(data16);
    schema["fields"].append(data17);
    schema["fields"].append(data18);
    schema["fields"].append(data19);
    schema["fields"].append(data20);
    schema["fields"].append(data21);
    schema["fields"].append(data22);
    schema["fields"].append(data23);
    schema["fields"].append(data24);
    schema["fields"].append(data25);
    root["schema"] = schema;
    uint8_t *ptr = package->get_data();
    payload["DeviceID"] = package->get_src_id();
    payload["WellID"] = 999;
    payload["Layer"] = AV_RB16(ptr);
    data[0] = ptr[3];
    data[1] = ptr[2];
    data[2] =  ptr[5];
    data[3] = ptr[4];
    payload["Traffic"] = *(float *)data;
    payload["Temperature"] = AV_RB16(ptr + 6);
    payload["InternalPressure"] = AV_RB16(ptr + 8);
    payload["ExternalPressure"] = AV_RB16(ptr + 10);
    payload["OpeningAccuracy"] = AV_RB16(ptr + 12);
    payload["MoisturePercentage"] = AV_RB16(ptr + 14);
    payload["Status"] = AV_RB16(ptr + 16);
    payload["MotorStatus"] = AV_RB16(ptr + 18);
    payload["MotorCurrent"] = AV_RB16(ptr + 20);
    payload["Voltage"] = AV_RB16(ptr + 22);
    payload["Current"] = AV_RB16(ptr + 24);
    payload["OperatingMode"] = AV_RB16(ptr + 26);
    payload["ControlStatus"] = AV_RB16(ptr + 28);
    payload["YieldControl"] = AV_RB16(ptr + 30);
    digital[0] = ptr[33];
    digital[1] = ptr[32];
    digital[2] =  ptr[35];
    digital[3] = ptr[34];
    payload["YieldValue"] = *(float *)digital;
    payload["UpFlowDeviation"] = AV_RB16(ptr + 36);
    payload["DownFlowDeviation"] = AV_RB16(ptr + 38);
    payload["AdjustmentMinutes"] = AV_RB16(ptr + 40);
    payload["PressureTestControl"] = AV_RB16(ptr + 42);
    payload["PressureTestHour"] = AV_RB16(ptr + 44);
    payload["WaterAbsorptionTest"] = AV_RB16(ptr + 46);
    payload["FaucetFlag"] = AV_RB16(ptr + 48);
    root["payload"] = payload;
    string str = JsonAsString(root);
    free(data);
    free(digital);
    data = nullptr;
    digital = nullptr;
    return str;
}
string parse_controller_to_kafka(shared_ptr<Package> package)
{
    Json::Value root;
    Json::Value schema;
    Json::Value payload;
    Json::Value data1;
    schema["type"] = "struct";
    data1["field"] = "ControllerID";
    data1["type"] = "int32";
    Json::Value data2;
    data2["field"] = "Controllertype";
    data2["type"] = "int32";
    Json::Value data3;
    data3["field"] = "DeviceID";
    data3["type"] = "int32";
    Json::Value data4;
    data4["field"] = "OutputCurrent";
    data4["type"] = "int32";
    Json::Value data5;
    data5["field"] = "OutputVoltage";
    data5["type"] = "int32";
    Json::Value data6;
    data6["field"] = "datetime1";
    data6["type"] = "int32";
    Json::Value data7;
    data7["field"] = "WellNum";
    data7["type"] = "int32";
    Json::Value data8;
    data8["field"] = "datetime2";
    data8["type"] = "int32";
    Json::Value data9;
    data9["field"] = "datetime3";
    data9["type"] = "int32";
    schema["fields"].append(data1);
    schema["fields"].append(data2);
    schema["fields"].append(data3);
    schema["fields"].append(data4);
    schema["fields"].append(data5);
    schema["fields"].append(data6);
    schema["fields"].append(data7);
    schema["fields"].append(data8);
    schema["fields"].append(data9);
    root["schema"] = schema;
    uint8_t *ptr = package->get_data();
    payload["DeviceID"] = package->get_src_id();
    payload["ControllerID"] = AV_RB16(ptr);
    payload["Controllertype"] = AV_RB16(ptr + 2);
    payload["OutputVoltage"] = AV_RB16(ptr + 4);
    payload["OutputCurrent"] = AV_RB16(ptr + 6);
    payload["WellNum"] = AV_RB16(ptr + 8);
    payload["datetime1"] = ptr[10]*100+ptr[11];
    payload["datetime2"] = ptr[12]*100+ptr[13];
    payload["datetime3"] = ptr[14]*100+ptr[15];
    root["payload"] = payload;
    string str = JsonAsString(root);
    return str;
}
string parse_alarm_to_kafka(shared_ptr<Package> package)
{
    Json::Value root;
    Json::Value schema;
    Json::Value payload;
    Json::Value data1;
    schema["type"] = "struct";
    data1["field"] = "DeviceID";
    data1["type"] = "int32";
    Json::Value data2;
    data2["field"] = "Reason";
    data2["type"] = "string";
    schema["fields"].append(data1);
    schema["fields"].append(data2);
    root["schema"] = schema;
    uint8_t *ptr = package->get_data();
    payload["DeviceID"] = AV_RL16(ptr + 31);
    string str(ptr + 33, ptr + 33 + 200);
    payload["Reason"] = str;
    root["payload"] = payload;
    string s = JsonAsString(root);
    return s;
}
int arrayToStr(unsigned char *buf, unsigned int buflen, char *out)
  {
      char strBuf[buflen*2 + 1];
      memset(strBuf,0,buflen*2 + 1);
      char pbuf[buflen*2];
      int i;
      for(i = 0; i < buflen; i++)
      {
          sprintf(pbuf, "%02X", buf[i]);
          strncat(strBuf, pbuf, 2);
      }
      strncpy(out, strBuf, buflen * 2);
    //   printf("out = %s\n", out);
      return buflen * 2;
 }
string parse_actionrecord_to_kafka(shared_ptr<Package> package, bool is_sync)
{
    Json::Value root;
    Json::Value schema;
    Json::Value payload;
    Json::Value data1;
    schema["type"] = "struct";
    data1["field"] = "DeviceID";
    data1["type"] = "int32";
    Json::Value data2;
    data2["field"] = "IsSucceed";
    data2["type"] = "boolean";
    Json::Value data3;
    data3["field"] = "Package";
    data3["type"] = "string";
    schema["fields"].append(data1);
    schema["fields"].append(data2);
    schema["fields"].append(data3);
    root["schema"] = schema;
    payload["DeviceID"] = package->get_dest_id();
    payload["IsSucceed"] = is_sync;
    int length = package->get_length();
    uint8_t tmp[length];
    memset(tmp,0,length);
    memcpy(tmp,package->get_message(),length);
    char out[length*2 + 1];
    memset(out,0,length*2 + 1);
    arrayToStr(tmp, length, out);
    string str(out);
    payload["Package"] = str;
    root["payload"] = payload;
    string s = JsonAsString(root);
    return s;
}
// bool parseMandrel(shared_ptr<Package> package, bool is_sync)
// {
//     mandrel_t mandrel_info;
//     uint8_t *ptr = package->get_data();
//     mandrel_info.id = package->get_src_id();
//     mandrel_info.wellid = 999;
//     mandrel_info.layer = AV_RL16(ptr);
//     mandrel_info.traffic = *(float *)(ptr + 2);
//     mandrel_info.temperature = AV_RL16(ptr + 6);
//     mandrel_info.internalpressure = AV_RL16(ptr + 8);
//     mandrel_info.externalpressure = AV_RL16(ptr + 10);
//     mandrel_info.openingaccuracy = AV_RL16(ptr + 12);
//     mandrel_info.moisturepercentage = AV_RL16(ptr + 14);
//     mandrel_info.status = AV_RL16(ptr + 16);
//     mandrel_info.motorstatus = AV_RL16(ptr + 18);
//     mandrel_info.motorcurrent = AV_RL16(ptr + 20);
//     mandrel_info.voltage = AV_RL16(ptr + 22);
//     mandrel_info.current = AV_RL16(ptr + 24);
//     mandrel_info.operatingmode = AV_RL16(ptr + 26);
//     mandrel_info.controlstatus = AV_RL16(ptr + 28);
//     mandrel_info.yieldcontrol = AV_RL16(ptr + 30);
//     mandrel_info.yieldvalue = *(float *)(ptr + 32);
//     mandrel_info.upflowdeviation = AV_RL16(ptr + 36);
//     mandrel_info.downflowdeviation = AV_RL16(ptr + 38);
//     mandrel_info.adjustmentminutes = AV_RL16(ptr + 40);
//     mandrel_info.pressuretestcontrol = AV_RL16(ptr + 42);
//     mandrel_info.pressuretesthour = AV_RL16(ptr + 44);
//     mandrel_info.waterabsorptiontest = AV_RL16(ptr + 46);
//     mandrel_info.faucetflag = AV_RL16(ptr + 48);
//     /*  if(!insert_mandrel_info(mandrel_info)) return false;
//    return true;*/
//     if (insert_mandrel_info(mandrel_info) && insert_mrecord_info(mandrel_info, is_sync))
//         return true;
//     return false;
// }
// /**
//  * 解析控制器表并将其插入到controller和controllerrecord表中
//  */
// bool parseController(shared_ptr<Package> package, bool is_sync)
// {
//     controller_t controller_info;
//     uint8_t *ptr = package->get_data();
//     controller_info.id = package->get_src_id();
//     controller_info.controller_id = AV_RL16(ptr);
//     controller_info.controllertype = AV_RL16(ptr + 2);
//     controller_info.outputvoltage = AV_RL16(ptr + 4);
//     controller_info.outputcurrent = AV_RL16(ptr + 6);
//     controller_info.wellnum = AV_RL16(ptr + 8);
//     controller_info.datetime1 = AV_RL16(ptr + 10);
//     controller_info.datetime2 = AV_RL16(ptr + 12);
//     controller_info.datetime3 = AV_RL16(ptr + 14);
//     /*if(!insert_controller_info(controller_info)) return false;
//     return true;*/
//     if (insert_controller_info(controller_info) && insert_crecord_info(controller_info, is_sync))
//         return true;
//     return false;
// }
// /**
//  * 解析控制器数据，同时插入到controllerrecord表中，如果有更新，就同时更新controller表中
//  */
// bool parseControllerrecord(shared_ptr<Package> package, bool is_sync)
// {
//     controllerrecord_t controllerrecord_info;
//     uint8_t *ptr = package->get_data();
//     controllerrecord_info.device_id = package->get_src_id();
//     controllerrecord_info.controller_id = AV_RL16(ptr);
//     controllerrecord_info.controllertype = AV_RL16(ptr + 2);
//     controllerrecord_info.outputvoltage = AV_RL16(ptr + 4);
//     controllerrecord_info.outputcurrent = AV_RL16(ptr + 6);
//     controllerrecord_info.wellnum = AV_RL16(ptr + 8);
//     controllerrecord_info.datetime1 = AV_RL16(ptr + 10);
//     controllerrecord_info.datetime2 = AV_RL16(ptr + 12);
//     controllerrecord_info.datetime3 = AV_RL16(ptr + 14);
//     if (insert_controllerrecord_info(controllerrecord_info, is_sync) && update_controller_info(controllerrecord_info, package->get_src_id()))
//         return true;
//     return false;
// }
// /**
//  * 解析工作筒数据，同时插入到mandrelrecord表中，如果有更新，就插入到mandrel表中
//  */
// bool parseMandrelrecord(shared_ptr<Package> package, bool is_sync)
// {
//     mandrelrecord_t mandrel_info;
//     uint8_t *ptr = package->get_data();
//     mandrel_info.device_id = package->get_src_id();
//     mandrel_info.wellid = 999;
//     mandrel_info.layer = AV_RL16(ptr);
//     mandrel_info.traffic = *(float *)(ptr + 2);
//     mandrel_info.temperature = AV_RL16(ptr + 6);
//     mandrel_info.internalpressure = AV_RL16(ptr + 8);
//     mandrel_info.externalpressure = AV_RL16(ptr + 10);
//     mandrel_info.openingaccuracy = AV_RL16(ptr + 12);
//     mandrel_info.moisturepercentage = AV_RL16(ptr + 14);
//     mandrel_info.status = AV_RL16(ptr + 16);
//     mandrel_info.motorstatus = AV_RL16(ptr + 18);
//     mandrel_info.motorcurrent = AV_RL16(ptr + 20);
//     mandrel_info.voltage = AV_RL16(ptr + 22);
//     mandrel_info.current = AV_RL16(ptr + 24);
//     mandrel_info.operatingmode = AV_RL16(ptr + 26);
//     mandrel_info.controlstatus = AV_RL16(ptr + 28);
//     mandrel_info.yieldcontrol = AV_RL16(ptr + 30);
//     mandrel_info.yieldvalue = *(float *)(ptr + 32);
//     mandrel_info.upflowdeviation = AV_RL16(ptr + 36);
//     mandrel_info.downflowdeviation = AV_RL16(ptr + 38);
//     mandrel_info.adjustmentminutes = AV_RL16(ptr + 40);
//     mandrel_info.pressuretestcontrol = AV_RL16(ptr + 42);
//     mandrel_info.pressuretesthour = AV_RL16(ptr + 44);
//     mandrel_info.waterabsorptiontest = AV_RL16(ptr + 46);
//     mandrel_info.faucetflag = AV_RL16(ptr + 48);
//     if (insert_mandrelrecord_info(mandrel_info, is_sync) && update_mandrel_info(mandrel_info, package->get_src_id()))
//         return true;
//     return false;
// }
// /**
//  * 解析井数据，push到kafka中
//  */
// bool parseWellrecord(shared_ptr<Package> package, bool is_sync)
// {
//     wellrecord_t well_info;
//     uint8_t *ptr = package->get_data();
//     well_info.device_id = package->get_src_id();
//     well_info.well_id = AV_RL16(ptr);
//     string str(ptr + 2, ptr + 2 + 20);
//     strcpy(well_info.name, str.data());
//     // memcpy(&well_info.name, ptr + 2, 20);
//     well_info.layer_number = AV_RL16(ptr + 22);
//     well_info.power_status = AV_RL16(ptr + 24);
//     well_info.status_flag = AV_RL16(ptr + 26);
//     well_info.operating_mode = AV_RL16(ptr + 28);
//     well_info.control_traffic = *(float *)(ptr + 30);
//     well_info.control_pressure = AV_RL16(ptr + 34);
//     well_info.control_opening = AV_RL16(ptr + 36);
//     well_info.control_status = AV_RL16(ptr + 38);
//     well_info.traffic_setting = *(float *)(ptr + 40);
//     if (insert_wellrecord_info(well_info, is_sync) && update_well_info(well_info, package->get_src_id()))
//         return true;
//     return false;
// }
/**
 * @brief 将uint8_t[]数组的数据转string类型
 */
string convert_string(uint8_t *buffer, uint32_t length)
{
    string str(buffer, buffer + length);
    return str;
}

void make_selcurr_package(shared_ptr<Package> package, uint16_t address, uint32_t length) // 组织查询反馈报文
{
    auto logger = spdlog::get("project_log");
    uint8_t str[] = {0x01, 0x03, static_cast<uint8_t>(address >> 8), static_cast<uint8_t>(address),
                     0x00, 0x01};
    uint16_t crc;
    crc = mb_crc16(str, sizeof(str));
    uint8_t str2[] = {0x01, 0x03, static_cast<uint8_t>(address >> 8), static_cast<uint8_t>(address), 0x00, 0x01, static_cast<uint8_t>(crc), static_cast<uint8_t>(crc >> 8)};
    for (int i = 0; i < sizeof(str2); i++)
    {
        package->get_data()[i] = str2[i];
    }
    package->set_sequence_number(length);
    int msgid;
    struct my_msg_st data;
    char buf[BUFSIZ];                                                          // 设置缓存变量
    msgid = msgget((key_t)FRONTEND_SEND_TO_BACKEND_MSG_KEY, 0664 | IPC_CREAT); // 创建消息队列
    if (msgid == -1)
    {
        logger->error("{}前端消息队列获取失败！", LOG_FILE_MSG);
        // perror("msgget err");
    }
    else
    {
        data.my_msg_type = rand() % 3 + 1; // 随机获取消息类型
        memcpy(data.anytext, package->get_message(), package->get_length());
        // 发送消息
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
            logger->error("{}向前端消息队列发送消息失败！", LOG_FILE_MSG);
            // perror("msgsnd err");
        }
    }
    sent_package.emplace(package->get_sequence_number(), package);
}

void make_well_downcontrol_package(shared_ptr<Package> package, uint16_t address, uint32_t length, string s) // 组织井数据的下行控制报文，连续写十个寄存器
{
    auto logger = spdlog::get("project_log");
    uint8_t t[s.size()];
    for (int i = 0; i < s.size(); i++)
    {
        t[i] = (uint8_t)(s[i]);
    }
    uint8_t str1[7] = {0x01, 0x10, static_cast<uint8_t>(address >> 8), static_cast<uint8_t>(address), 0x00, 0x0A, 0x14};
    uint8_t str[27];
    memset(str, 0, sizeof(str));
    for (int i = 0; i < 7; i++)
    {
        str[i] = str1[i];
    }
    for (int i = 0; i < s.size(); i++)
    {
        str[7 + i] = t[i];
    }
    uint16_t crc;
    crc = mb_crc16(str, sizeof(str));
    for (int i = 0; i < sizeof(str); i++)
    {
        package->get_data()[i] = str[i];
    }
    package->get_message()[40] = static_cast<uint8_t>(crc);
    package->get_message()[41] = static_cast<uint8_t>(crc >> 8);
    package->set_sequence_number(length);
    int msgid;
    struct my_msg_st data;
    char buf[BUFSIZ];                                                          // 设置缓存变量
    msgid = msgget((key_t)FRONTEND_SEND_TO_BACKEND_MSG_KEY, 0664 | IPC_CREAT); // 创建消息队列
    if (msgid == -1)
    {
        logger->error("{}前端消息队列获取失败！", LOG_FILE_MSG);
        // perror("msgget err");
    }
    else
    {
        data.my_msg_type = rand() % 3 + 1; // 随机获取消息类型
        memcpy(data.anytext, package->get_message(), package->get_length());
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
            logger->error("{}向前端消息队列发送消息失败！", LOG_FILE_MSG);
            // perror("msgsnd err");
        }
    }
    sent_package.emplace(package->get_sequence_number(), package);
}
void make_mandrel_downcontrol_one_package(shared_ptr<Package> package, uint16_t address, uint32_t length, uint16_t cont) // 组织下行控制报文，连续写一个寄存器
{
    auto logger = spdlog::get("project_log");
    uint8_t str[] = {0x01, 0x06, static_cast<uint8_t>(address >> 8), static_cast<uint8_t>(address),
                     static_cast<uint8_t>(cont >> 8), static_cast<uint8_t>(cont)};
    uint16_t crc;
    crc = mb_crc16(str, sizeof(str));
    uint8_t str2[] = {0x01, 0x06, static_cast<uint8_t>(address >> 8), static_cast<uint8_t>(address),
                      static_cast<uint8_t>(cont >> 8), static_cast<uint8_t>(cont), static_cast<uint8_t>(crc),
                      static_cast<uint8_t>(crc >> 8)};
    for (int i = 0; i < sizeof(str2); i++)
    {
        package->get_data()[i] = str2[i];
    }
    package->set_sequence_number(length);
    int msgid;
    struct my_msg_st data;
    char buf[BUFSIZ];                                                          // 设置缓存变量
    msgid = msgget((key_t)FRONTEND_SEND_TO_BACKEND_MSG_KEY, 0664 | IPC_CREAT); // 创建消息队列
    if (msgid == -1)
    {
        logger->error("{}前端消息队列获取失败！", LOG_FILE_MSG);
        // perror("msgget err");
    }
    else
    {
        data.my_msg_type = rand() % 3 + 1; // 随机获取消息类型
        memcpy(data.anytext, package->get_message(), package->get_length());
        // 发送消息
        if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
        {
            logger->error("{}向前端消息队列发送消息失败！", LOG_FILE_MSG);
            // perror("msgsnd err");
        }
    }
    sent_package.emplace(package->get_sequence_number(), package);
}

void make_mandrel_downcontrol_two_package(shared_ptr<Package> package, uint16_t address, uint32_t length, float cont) // 组织下行控制报文，连续写两个寄存器
{
    auto logger = spdlog::get("project_log");
    Data data;
    data.a = cont;
    uint8_t str[] = {0x01, 0x10, static_cast<uint8_t>(address >> 8), static_cast<uint8_t>(address), 0x00, 0x02, 0x04,static_cast<uint8_t>(data.b >> 8),static_cast<uint8_t>(data.b), static_cast<uint8_t>(data.b>>24), static_cast<uint8_t>(data.b>>16)};
    uint16_t crc;
    crc = mb_crc16(str, sizeof(str));
    uint8_t str2[] = {0x01, 0x10, static_cast<uint8_t>(address >> 8), static_cast<uint8_t>(address), 0x00, 0x02, 0x04,
                     static_cast<uint8_t>(data.b >> 8),static_cast<uint8_t>(data.b), static_cast<uint8_t>(data.b>>24), static_cast<uint8_t>(data.b>>16),static_cast<uint8_t>(crc),
                      static_cast<uint8_t>(crc >> 8)};
    for (int i = 0; i < sizeof(str2); i++)
    {
        package->get_data()[i] = str2[i];
    }
    package->set_sequence_number(length);
    int msgid;
    struct my_msg_st msg_data;
    char buf[BUFSIZ];                                                          // 设置缓存变量
    msgid = msgget((key_t)FRONTEND_SEND_TO_BACKEND_MSG_KEY, 0664 | IPC_CREAT); // 创建消息队列
    if (msgid == -1)
    {
        logger->error("{}前端消息队列获取失败！", LOG_FILE_MSG);
        // perror("msgget err");
    }
    else
    {
        msg_data.my_msg_type = rand() % 3 + 1;
        memcpy(msg_data.anytext, package->get_message(), package->get_length());
        // 发送消息
        if (msgsnd(msgid, (void *)&msg_data, sizeof(msg_data), 0) == -1)
        {
            logger->error("{}向前端消息队列发送消息失败！", LOG_FILE_MSG);
            // perror("msgsnd err");
        }
    }
    sent_package.emplace(package->get_sequence_number(), package);
}

shared_ptr<Package> send_change_mode_msg(bool communication_mode)
{
    shared_ptr<Package> pkg(new Package(0, CHANGE_MODE));
    pkg->set_dest_id(0);
    pkg->set_sequence_number(0);
    pkg->set_commu_type(communication_mode);
    return pkg;
}
shared_ptr<Package> send_alarm_msg(shared_ptr<Package> pkg, char *tmp)
{
//   shared_ptr<Package> package(new Package(225, SYNC_DATA));
  shared_ptr<Package> package(new Package(233, SYNC_DATA));
  package->set_dest_id(3);
  package->set_sequence_number(get_sequence_number());
  package->set_db_type(4); // StatusAlarm表
  uint8_t *ptr = package->get_data();
  AV_WL64(ptr , 0);
  string current_time = GetCurrentTimeStamp();
  memcpy(ptr+8, current_time.c_str(), current_time.size());
  AV_WL16(ptr + 31, pkg->get_dest_id());
  string str(tmp);
  memcpy(ptr + 33, str.c_str(), str.size());
  // string msg(package->get_message(),package->get_message()+package->get_length());
  return package;
}

shared_ptr<Package> send_alarm_msg(uint16_t device_id, char *tmp)
{
//   shared_ptr<Package> package(new Package(225, SYNC_DATA));
  shared_ptr<Package> package(new Package(233, SYNC_DATA));
  package->set_dest_id(3);
  package->set_sequence_number(get_sequence_number());
  package->set_db_type(4); // StatusAlarm表
  uint8_t *ptr = package->get_data();
  AV_WL64(ptr , get_alarm_number());
  string current_time = GetCurrentTimeStamp();
  memcpy(ptr + 8, current_time.c_str(), current_time.size());
  AV_WL16(ptr + 31, device_id);
  string str(tmp);
  memcpy(ptr + 33, str.c_str(), str.size());
  // string msg(package->get_message(),package->get_message()+package->get_length());
  return package;
}

shared_ptr<Package> send_actionrecord_msg(shared_ptr<Package> pkg , bool is_sync)
{
  shared_ptr<Package> package(new Package(226, SYNC_DATA));
  package->set_dest_id(3);
  package->set_sequence_number(get_sequence_number());
  package->set_db_type(5); // ActionRecord表
  uint8_t *ptr = package->get_data();
  string current_time = GetCurrentTimeStamp();
  memcpy(ptr, current_time.c_str(), current_time.size());
  AV_WL16(ptr + 23, pkg->get_dest_id());
  ptr[25] = is_sync;
  memcpy(ptr + 26, pkg->get_message(), pkg->get_length());
  return package;
}
shared_ptr<Package> convert_package(shared_ptr<Package> pkg)
{
   if(pkg->get_length()==53||pkg->get_length()==59||pkg->get_length()==25)
   {
    int length = pkg->get_length()-9;
   uint8_t *ptr = pkg -> get_data();
   shared_ptr<Package> package(new Package(length, UP_DEVICE_INFO));
   uint8_t *ptr_n = package -> get_data();
    package->set_dest_id(3);
   if (int(pkg->get_src_id()) == 39)
  {
    package->set_src_id(39);
    AV_WL16(ptr_n,AV_RB16(ptr));
    AV_WL16(ptr_n+ 2,AV_RB16(ptr + 2));
    AV_WL16(ptr_n + 4,AV_RB16(ptr + 4));
    AV_WL16(ptr_n + 6,AV_RB16(ptr + 6));
    AV_WL16(ptr_n + 8,AV_RB16(ptr + 8));
    AV_WL16(ptr_n + 10,AV_RB16(ptr + 10));
    AV_WL16(ptr_n + 12,AV_RB16(ptr + 12));
    AV_WL16(ptr_n + 14,AV_RB16(ptr + 14));
    return package;
  }
  if (int(pkg->get_src_id()) == 999) // 解析井数据
  {
    package->set_src_id(999);
    AV_WL16(ptr_n, AV_RB16(ptr));
    string str(ptr + 2, ptr + 2 + 20);
    memcpy(ptr_n + 2,str.c_str(),str.size());
    AV_WL16(ptr_n + 22, AV_RB16(ptr + 22));
    AV_WL16(ptr_n + 24, AV_RB16(ptr + 24));
    AV_WL16(ptr_n + 26, AV_RB16(ptr + 26));
    AV_WL16(ptr_n + 28, AV_RB16(ptr + 28));
    ptr_n[30] = ptr[30];
    ptr_n[31] = ptr[31];
    ptr_n[32] = ptr[32];
    ptr_n[33] = ptr[33];
    AV_WL16(ptr_n + 34, AV_RB16(ptr + 34));
    AV_WL16(ptr_n + 36, AV_RB16(ptr + 36));
    AV_WL16(ptr_n + 38, AV_RB16(ptr + 38));
    ptr_n[40] = ptr[40];
    ptr_n[41] = ptr[41];
    ptr_n[42] = ptr[42];
    ptr_n[43] = ptr[43];
    return package;
  }
  if (int(pkg->get_src_id()) == 1107 || int(pkg->get_src_id()) == 1207 || int(pkg->get_src_id()) == 1307 || int(pkg->get_src_id()) == 1407 || int(pkg->get_src_id()) == 1507 || int(pkg->get_src_id()) == 1607)
  {
    package->set_src_id(int(pkg->get_src_id()));
    AV_WL16(ptr_n, AV_RB16(ptr));
    ptr_n[2] = ptr[2];
    ptr_n[3] = ptr[3];
    ptr_n[4] = ptr[4];
    ptr_n[5] = ptr[5];
    AV_WL16(ptr_n + 6, AV_RB16(ptr + 6));
    AV_WL16(ptr_n + 8, AV_RB16(ptr + 8));
    AV_WL16(ptr_n + 10, AV_RB16(ptr + 10));
    AV_WL16(ptr_n + 12, AV_RB16(ptr + 12));
    AV_WL16(ptr_n + 14, AV_RB16(ptr + 14));
    AV_WL16(ptr_n + 16, AV_RB16(ptr + 16));
    AV_WL16(ptr_n + 18, AV_RB16(ptr + 18));
    AV_WL16(ptr_n + 20, AV_RB16(ptr + 20));
    AV_WL16(ptr_n + 22, AV_RB16(ptr + 22));
    AV_WL16(ptr_n + 24, AV_RB16(ptr + 24));
    AV_WL16(ptr_n + 26, AV_RB16(ptr + 26));
    AV_WL16(ptr_n + 28, AV_RB16(ptr + 28));
    AV_WL16(ptr_n + 30, AV_RB16(ptr + 30));
    ptr_n[32] = ptr[32];
    ptr_n[33] = ptr[33];
    ptr_n[34] = ptr[34];
    ptr_n[35] = ptr[35];
    AV_WL16(ptr_n + 36, AV_RB16(ptr + 36));
    AV_WL16(ptr_n + 38, AV_RB16(ptr + 38));
    AV_WL16(ptr_n + 40, AV_RB16(ptr + 40));
    AV_WL16(ptr_n + 42, AV_RB16(ptr + 42));
    AV_WL16(ptr_n + 44, AV_RB16(ptr + 44));
    AV_WL16(ptr_n + 46, AV_RB16(ptr + 46));
    AV_WL16(ptr_n + 48, AV_RB16(ptr + 48));
    return package;
  }
   }
   else
   {
        return nullptr;
   }
   
}
