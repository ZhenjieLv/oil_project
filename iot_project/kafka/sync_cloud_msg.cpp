#include "sync_cloud_msg.h"
pid_t gettid()
{
  return syscall(SYS_gettid);
}

// 云端控制监听线程 控制报文转到g_comm_send_queue kafka pull指令监听
void *listen_cloud_control_thread(void *arg)
{
  #ifdef DEBUG
  pid_t pid = gettid();
  pthread_t tid = pthread_self();
  printf("listen_cloud_control_thread: pid=%d, tid=%lu\n", pid, tid);
  #endif
  std::vector<std::string> topics;
  topics.push_back("data_control_fromflink");
  std::string group = "test_consumer_group";
  AuthenConsumer consumer(brokers, group, topics, RdKafka::Topic::OFFSET_END);
  // AuthenConsumer consumer("10.16.14.20:9092", group, topics, RdKafka::Topic::OFFSET_END);
  while (true)
  {
    consumer.pullMessage();
  }
  RdKafka::wait_destroyed(5000);
}
// 云端控制ACK发送线程 g_cmd_cloud_queue kafka push指令发送
void *cloud_control_ACK_thread(void *arg)
{
  #ifdef DEBUG
  pid_t pid = gettid();
  pthread_t tid = pthread_self();
  printf("cloud_control_ACK_thread: pid=%d, tid=%lu\n", pid, tid);
  #endif
  AuthenProducer producer(brokers, "data_control_toflink");
  KafkaProducer action_producer(local_brokers, "data_realtime_toflink");
  KafkaProducer actionrecord_producer(local_brokers, "ActionRecord");
  bool is_sync;
  while (true)
  {
    // if(g_cloud_send_queue->is_empty())
    //     continue;
    if(!(g_cloud_send_queue->is_empty()))
    {
      shared_ptr<Package> package = g_cloud_send_queue->get_package();
    string msg(package->get_message(), package->get_message() + package->get_length());
    is_sync = producer.pushMessage(msg_encrypt(msg));
    map<uint32_t, shared_ptr<Package>>::iterator t;
    t = cloud_action_record.find(package->get_sequence_number());
    if (t != cloud_action_record.end())
    {
    shared_ptr<Package> action_package = send_actionrecord_msg(cloud_action_record.at(package->get_sequence_number()), is_sync);
    string enmsg(action_package->get_message(), action_package->get_message() + action_package->get_length());
    action_producer.pushMessage(msg_encrypt(enmsg));
    string s =  parse_actionrecord_to_kafka(cloud_action_record.at(package->get_sequence_number()), is_sync);
    actionrecord_producer.pushMessage(s);   
    cloud_action_record.erase(package->get_sequence_number()); 
    } 
    }
    }
  RdKafka::wait_destroyed(5000);
}

