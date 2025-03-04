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
  KafkaProducer producer(local_brokers, "controller");
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
  uint8_t buffer[25] = { 0x03, 0x19, 0x00, 0x00, 0x00, 0x03, 0x00, 0x27, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x14, 0x00, 0x32, 0x00, 0x02, 0x18, 0x02, 0x09, 0x00, 0x20, 0x02};
  payload["DeviceID"] = 39;
   payload["ControllerID"] = AV_RB16(buffer + 9);
   payload["Controllertype"] = AV_RB16(buffer + 11);
   payload["OutputVoltage"] = AV_RB16(buffer + 13);
   payload["OutputCurrent"] = AV_RB16(buffer + 15);
   payload["WellNum"] = AV_RB16(buffer + 17);
   payload["datetime1"] = buffer[19]*100+buffer[20];
   payload["datetime2"] = buffer[21]*100+buffer[22];
   payload["datetime3"] = buffer[23]*100+buffer[24];
   root["payload"] = payload;
 // cout << "id:" << root["id"].asInt() << endl;
  string str = JsonAsString(root);
  producer.pushMessage(str);
  cout<<str<<endl;
  return 0;

}