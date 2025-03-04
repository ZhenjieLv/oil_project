#include <iostream>
#include "AuthenProducer.h"
#include <string.h>
#include "variables.h"
#include "util.h"
#include "cryptmsg.h"
#include "KafkaProducer.h"
using namespace std;

void delay(int seconds) //  参数必须为整型，表示延时多少秒
{
    clock_t start = clock();
    clock_t lay = (clock_t)seconds * CLOCKS_PER_SEC;
    while ((clock()-start) < lay);
 }
int main()
{
    init_logger();
    // 创建Producer
   // KafkaProducer producer("127.0.0.1:9092", "test", 0);
 KafkaProducer producer("localhost:9092", "data_realtime_toflink");
  //  AuthenProducer producer(brokers, "data_control_fromflink");
   // KafkaProducer producer("10.16.14.20:9092", "test", 0);
   // printf("已经完成创建produce\n");
   // KafkaProducer producer("127.0.0.1:9092", "Data_Upload");
   uint8_t tmp[25] = {0x03,0x19,0x00,0x00,0x00,0x03,0x00,0x27,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x0b,0x00,0x00,0x00,0x04};
   string msg(tmp,tmp+sizeof(tmp));
   int cnt = 20;
 // while(true)
 bool is_sync;
    while(cnt--)
    {
      delay(2);
     is_sync = producer.pushMessage(msg_encrypt(msg));  

    }
   
    RdKafka::wait_destroyed(5000);
     close_logger();
}