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
  uint8_t buffer[59] = {0x03, 0x3B, 0x00, 0x00, 0x00, 0x03, 0x00, 0x53, 0x04, 0x00, 0x01, 0x00, 0x02, 0x00 ,0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02};
  payload["DeviceID"] = AV_RL16(buffer + 7);
   payload["WellID"] = 999;
  // string s(buffer + 11,buffer + 11 + 20);
   payload["Layer"] =  AV_RB16(buffer + 9);
   payload["Traffic"] = *(float*)(buffer + 11);
   payload["Temperature"] = AV_RB16(buffer + 15);
   payload["InternalPressure"] = AV_RB16(buffer + 17);
   payload["ExternalPressure"] = AV_RB16(buffer + 19);
   payload["OpeningAccuracy"] = *(float*)(buffer + 21);
   payload["MoisturePercentage"] = AV_RB16(buffer + 23);
   payload["Status"] = AV_RB16(buffer + 25);
   payload["MotorStatus"] = AV_RB16(buffer + 27);
   payload["MotorCurrent"] = *(float*)(buffer + 29);
   payload["Voltage"] = AV_RB16(buffer + 31);
   payload["Current"] = AV_RB16(buffer + 33);
   payload["OperatingMode"] = *(float*)(buffer + 35);
   payload["ControlStatus"] = AV_RB16(buffer + 37);
   payload["YieldControl"] =AV_RB16(buffer + 39);
   payload["YieldValue"] = *(float*)(buffer + 41);
   payload["UpFlowDeviation"] = AV_RB16(buffer + 45);
   payload["DownFlowDeviation"] = AV_RB16(buffer + 47);
   payload["AdjustmentMinutes"] = AV_RB16(buffer + 49);
   payload["PressureTestControl"] = AV_RB16(buffer + 51);
   payload["PressureTestHour"] = AV_RB16(buffer + 53);
   payload["WaterAbsorptionTest"] = AV_RB16(buffer + 55);
   payload["FaucetFlag"] = AV_RB16(buffer + 57);
   root["payload"] = payload;
 // cout << "id:" << root["id"].asInt() << endl;
  string str = JsonAsString(root);
  producer.pushMessage(str);
  cout<<str<<endl;
  return 0;
}