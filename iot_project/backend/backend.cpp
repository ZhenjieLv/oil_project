#include "backend.h"
#include <sys/msg.h>
#define LOG_FILE_MSG "[backend backend.cpp]: "
void send_msg_to_frontend(shared_ptr<Package> package)
{
  int msgid, send_times = 0;
  struct my_msg_st data;
  auto logger = spdlog::get("project_log");
  while (true)
  {
    msgid = msgget((key_t)BACKEND_SEND_TO_FRONTEND_MSG_KEY, 0664 | IPC_CREAT); // 创建消息队列
    if (msgid == -1)
    {
      logger->error("{}从后端发送消息到前端第{}次失败，无法获取消息队列", LOG_FILE_MSG, send_times);
    }
    else
    {
      data.my_msg_type = rand() % 3 + 1;
      memcpy(data.anytext, package->get_message(), package->get_length());
      if (msgsnd(msgid, (void *)&data, sizeof(data), 0) == -1)
      {
        logger->error("{}从后端发送消息到前端第{}次失败，无法获取消息队列\n", LOG_FILE_MSG, send_times);
      }
      else
      {
        return;
      }
    }
    send_times++;
  }
}

/**
 * @brief 处理串口线程
*/
void *backend_recv_thread(void *arg)
{
#ifdef DEBUG
  pid_t pid = syscall(SYS_gettid);
  pthread_t tid = pthread_self();
  printf("backend_recv_thread: pid=%d, tid=%lu\n", pid, tid);
#endif
  uint8_t buffer[MAX_BUF_LEN];
  // KafkaProducer producer(brokers, "Data_Upload");
  // KafkaProducer producer(brokers, "data_realtime_toflink");
  // KafkaProducer producer(local_brokers, "test_topic_name");
  KafkaProducer producer(local_brokers, "data_realtime_toflink");
  KafkaProducer well_producer(local_brokers, "well");
  KafkaProducer mandrel_producer(local_brokers, "mandrel");
  KafkaProducer controller_producer(local_brokers, "controller");
  shared_ptr<Package> converted_pkg;
  //  设置缓存变量
  while (true)
  {
    shared_ptr<Package> package = g_edge_recv_queue->get_package();
    if (package == nullptr)
    {
      break;
    }
    if (package->get_dest_id() == 1)
    {
      g_cloud_send_queue->add_package(package); // 发到云端
      cloud_package.emplace(package->get_sequence_number(), package);
    }
    else
    {
      uint32_t length;
      string current_time;
      switch (package->get_type())
      {
      case UP_QUERY_FEEDBACK:
      case ACK_MSG:
        send_msg_to_frontend(package);
        break;
      case UP_DEVICE_INFO:
        converted_pkg = convert_package(package);
        if(converted_pkg==nullptr)
          continue;
        else
        {
          length = converted_pkg->get_length();
          memcpy(buffer, converted_pkg->get_message(), length);
    #ifdef DEBUG
          cout<<"转换：";
      for(int i = 0;i<length;i++)
        {
          printf("%x ",buffer[i]);
        }
        printf("\n");
    #endif
        current_time = GetCurrentTimeStamp();
        memcpy(buffer + converted_pkg->get_length(), current_time.c_str(), current_time.size());
        length += 23;
        producer.pushMessage(msg_encrypt(convert_string(buffer, length)));
        if (int(package->get_src_id()) == 39)
        {
          controller_producer.pushMessage(parse_controller_to_kafka(package));
        }
        if (int(package->get_src_id()) == 999) // 解析井数据
        {
          well_producer.pushMessage(parse_well_to_kafka(package));
        }
        if (int(package->get_src_id()) == 1107 || int(package->get_src_id()) == 1207 || int(package->get_src_id()) == 1307 || int(package->get_src_id()) == 1407 || int(package->get_src_id()) == 1507 || int(package->get_src_id()) == 1607)
        {
          mandrel_producer.pushMessage(parse_mandrel_to_kafka(package));
        }
        break;
        }
      default:
        break;
      }
    }
    package = nullptr;
  }
  RdKafka::wait_destroyed(5000);
  pthread_exit(NULL);
}

void *recv_msg_thread(void *arg)
{
#ifdef DEBUG
  pid_t pid = syscall(SYS_gettid);
  pthread_t tid = pthread_self();
  printf("recv_msg_thread: pid=%d, tid=%lu\n", pid, tid);
#endif
  int msgid;
  struct my_msg_st data;
  long int msg_to_rcv = 0;
  auto logger = spdlog::get("project_log");
  uint8_t buffer[MAX_BUF_LEN] = {0};
  // char buffer[MAX_BUF_LEN];
  //  rcv msg
  msgid = msgget((key_t)FRONTEND_SEND_TO_BACKEND_MSG_KEY, 0664 | IPC_CREAT); // 获取消息队列
  if (msgid == -1)
  {
    logger->error("{}无法从后端获取前端消息队列！", LOG_FILE_MSG);
  }
  // KafkaProducer producer(brokers, "Data_Upload");
  // KafkaProducer producer(brokers, "data_realtime_toflink");
  // KafkaProducer producer(local_brokers, "test_topic_name");
  KafkaProducer producer(local_brokers, "data_realtime_toflink");
  KafkaProducer alarm_producer(local_brokers, "StatusAlarm");
  while (true)
  {
    // 接收消息
    if (msgrcv(msgid, (void *)&data, BUFSIZ, msg_to_rcv, 0) == -1)
    {
      logger->error("{}无法在后端接收前端消息！", LOG_FILE_MSG);
      usleep(100);
      continue;
    }
    uint32_t pkg_size = AV_RL32(data.anytext + 1);
    shared_ptr<Package> package(new Package(pkg_size));
    package->parse_message(data.anytext, pkg_size);
    string msg;
    string str1;
   // bool is_sync;
    switch (package->get_type())
    {
    case SYNC_DATA:
      AV_WL64(package->get_message()+14,get_alarm_number());
      msg = string(package->get_message(), package->get_message() + package->get_length());
//      for(int i = 0;i < package->get_length()-200; i++)
//          printf("%x ",package->get_message()[i]);
//      str1 = string(package->get_message()+47,package->get_message()+47+200);
//      cout<<str1;
      producer.pushMessage(msg_encrypt(msg));
      alarm_producer.pushMessage(parse_alarm_to_kafka(package));
      // memcpy(buffer, package->get_message() + 39, 200);
      // insert_status_alarm_info(AV_RL16(package->get_data() + 23), buffer, is_sync);
      break;
    case CHANGE_MODE:
      is_wireless = !is_wireless;
      package->set_commu_type(is_wireless);
     // g_com_send_queue->add_package(package);
      send_three_times_msg(package);
      break;
    default:
      g_com_send_queue->add_package(package);
      break;
    }
  }
  RdKafka::wait_destroyed(5000);
}
