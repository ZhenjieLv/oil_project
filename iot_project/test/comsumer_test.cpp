#include "AuthenConsumer.h"
#include <iostream>
using namespace std;
int main()
{
    initialize_global_variable();
    init_logger();
    std::vector<std::string> topics;
    topics.push_back("data_control_fromflink");
    // topics.push_back("test2");
    std::string group = "test-consumer-group";
    AuthenConsumer consumer(brokers, group, topics, RdKafka::Topic::OFFSET_BEGINNING);
    cout<<"12345"<<endl;
    consumer.pullMessage();
    RdKafka::wait_destroyed(5000);
    close_logger();
    destroy_global_variable();
    return 0;
}

// void *consumer_thread(void *arg)
// {
//   std::string brokers = "10.16.14.20:9092";
//     std::vector<std::string> topics;
//     topics.push_back("test");
//     // topics.push_back("test2");
//     std::string group = "testGroup";
//     // if(argcv >= 2) {
//     //     group = argv[1];
//     // }
//     std::cout << "group " << group << std::endl;

//     KafkaConsumer consumer(brokers, group, topics, RdKafka::Topic::OFFSET_BEGINNING);
//     while(true)
//     {
//        consumer.pullMessage();
//     }
//     RdKafka::wait_destroyed(5000);
//     return 0;

// }