#include <jsoncpp/json/json.h>
#include <iostream>
#include "error_no.h"
#include "intreadwrite.h"
#include "variables.h"
#include "util.h"
#include "KafkaProducer.h"
using namespace std;
std::string JsonAsString(const Json::Value &json)
{
  std::string result;
  Json::StreamWriterBuilder wbuilder;
  wbuilder["indentation"] = ""; // Optional
  result = Json::writeString(wbuilder, json);
  return result;
}

int main()
{
  init_logger();
  KafkaProducer producer(local_brokers, "well");
  Json::Value root;
  Json::Value schema;
  Json::Value payload;
  Json::Value data1;                                                                                             
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
  uint8_t buffer[53] = {0x03, 0x35, 0x00, 0x00, 0x00, 0x03, 0x00, 0xE7, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x09, 0x00, 0x09, 0x00, 0x01, 0x00, 0x08, 0x00, 0x07, 0x00, 0x09, 0x00, 0x01, 0x00, 0x02};
  payload["DeviceID"] = AV_RL16(buffer + 7);
   payload["WellID"] = AV_RB16(buffer + 9);
  // string s(buffer + 11,buffer + 11 + 20);
   payload["WellName"] = "123";
   payload["LayerNumber"] = AV_RB16(buffer + 31);
   payload["PowerStatus"] = AV_RB16(buffer + 33);
   payload["StatusFlag"] = AV_RB16(buffer + 35);
   payload["OperatingMode"] = AV_RB16(buffer + 37);
   payload["ControlTraffic"] = *(float*)(buffer + 39);
   payload["ControlPressure"] = AV_RB16(buffer + 43);
   payload["ControlOpening"] = AV_RB16(buffer + 45);
   payload["ControlStatus"] = AV_RB16(buffer + 47);
   payload["TrafficSetting"] = *(float*)(buffer + 49);
   root["payload"] = payload;
 // cout << "id:" << root["id"].asInt() << endl;
  string str = JsonAsString(root);
  producer.pushMessage(str);
  cout<<str<<endl;
  return 0;
}