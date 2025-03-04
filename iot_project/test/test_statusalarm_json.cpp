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
  KafkaProducer producer(local_brokers, "StatusAlarm");
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
  payload["DeviceID"] = 39;
  payload["Reason"] = "设备井(999)连接状态不好，无法收到回应！";
   root["payload"] = payload;
 // cout << "id:" << root["id"].asInt() << endl;
  string str = JsonAsString(root);
  producer.pushMessage(str);
  cout<<str<<endl;
  return 0;

}