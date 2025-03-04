#include <jsoncpp/json/json.h>
#include <iostream>
#include "error_no.h"
#include "intreadwrite.h"
#include "variables.h"
#include "util.h"
#include "KafkaProducer.h"
#include <sstream>
#include <iomanip>
using namespace std;
std::string JsonAsString(const Json::Value &json)
{
  std::string result;
  Json::StreamWriterBuilder wbuilder;
  wbuilder["indentation"] = ""; // Optional
  result = Json::writeString(wbuilder, json);
  return result;
}

  int arrayToStr(unsigned char *buf, unsigned int buflen, char *out)
  {
      char strBuf[43] = {0};
      char pbuf[42];
      int i;
      for(i = 0; i < buflen; i++)
      {
          sprintf(pbuf, "%02X", buf[i]);
          strncat(strBuf, pbuf, 2);
      }
      strncpy(out, strBuf, buflen * 2);
      printf("out = %s\n", out);
      return buflen * 2;
 }

int main()
{
  init_logger();
  KafkaProducer producer(local_brokers, "ActionRecord");
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
  payload["DeviceID"] = 39;
  payload["IsSucceed"] = 0;
  uint8_t  tmp[21] = {0x04,0x15,0x00,0x00,0x00,0xe7,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x03,0xf2,0x00,0x01,0xe9,0xbd};
  char str[43] = {0};
  // string msg(tmp,tmp+sizeof(tmp));
  // payload["Package"] = "03350000000300E7030001";
  arrayToStr(tmp,21,str);
  string m(str);
  payload["Package"] = m;
  root["payload"] = payload;
 // cout << "id:" << root["id"].asInt() << endl;
  string s = JsonAsString(root);
  producer.pushMessage(s);
  cout<<s<<endl;
  return 0;

}