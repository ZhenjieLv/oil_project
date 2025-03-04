#include <iostream>
#include "KafkaProducer.h"
using namespace std;


int main()
{
    // 创建Producer
    KafkaProducer producer("10.16.14.20:9092", "test", 0);
   // KafkaProducer producer("10.16.14.20:9092", "test", 0);
    printf("已经完成创建produce\n");
    for(int i = 0; i < 2; i++)
    {
        char msg[64] = {0};
        sprintf(msg, "%s%4d", "Hello RdKafka ", i);
        // 生产消息
        char key[8] = {0};      // 主要用来做负载均衡
        sprintf(key, "%d", i);
        producer.pushMessage(msg, key);  
        //printf("完成消息传输\n");

    }
    RdKafka::wait_destroyed(5000);
}
// void *producer_thread(void *arg)
// {
//    KafkaProducer producer("10.16.14.20:9092", "test", 0);
//    while(true)
//    {
//      char msg[64] = {0};
//      sprintf(msg, "%s%4d", "Hello RdKafka ");
//      // 生产消息
//     //  char key[8] = {0};      // 主要用来做负载均衡
//     //  sprintf(key, "%d", i);
//     //  producer.pushMessage(msg, key); 
//     producer.pushMessage(msg, NULL); 

//    }

// }