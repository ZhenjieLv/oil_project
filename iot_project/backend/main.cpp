#include "backend.h"
#include "util.h"
#include <cstdio>
#include <stdlib.h>
#include <sys/msg.h>
#include "edge_communication.h"
#include "monitortimer.h"
using namespace std;
#define LOG_FILE_MSG "[backend main.cpp]: "
int main()
{
  pthread_t serial_recv_thread;
  pthread_t backend_thread;
  pthread_t monitor_thread;
  pthread_t listen_thread;
  pthread_t cloud_thread;
  pthread_t recv_thread;
  pthread_t ethernet_thread;
  pthread_t send_thread;
  initialize_global_variable();
  init_logger();
  auto logger = spdlog::get("project_log");
  get_alarm_id();
  open_serial();
  commu_mode = on_ping();
  if (commu_mode == false)
  {
    string msg;
    KafkaProducer producer(local_brokers, "data_realtime_toflink");
    KafkaProducer alarm_producer(local_brokers, "StatusAlarm");
    char tmp[200] = {0};
    memset(tmp, '\0', sizeof(tmp));
    sprintf(tmp, "网络错误,无法通过有线方式发送！");
    shared_ptr<Package> alarm_package = send_alarm_msg(2, tmp);
    msg = string(alarm_package->get_message(), alarm_package->get_message() + alarm_package->get_length());
    producer.pushMessage(msg_encrypt(msg));
    alarm_producer.pushMessage(parse_alarm_to_kafka(alarm_package));
  }
  monitorTimer obj;
  obj.start(300000, monitor_network);
  int result7 = pthread_create(&ethernet_thread, NULL, ethernet_recv_thread, NULL);
  int result = pthread_create(&serial_recv_thread, NULL, serial_port_receive_thread, NULL);
  int result2 = pthread_create(&backend_thread, NULL, backend_recv_thread, NULL);
  int result6 = pthread_create(&recv_thread, NULL, recv_msg_thread, NULL);
  int result4 = pthread_create(&listen_thread, NULL, listen_cloud_control_thread, NULL);
  int result5 = pthread_create(&cloud_thread, NULL, cloud_control_ACK_thread, NULL);
  int result8 = pthread_create(&send_thread, NULL, send_package_thread, NULL);
  if (result7 != 0)
  {
    logger->error("{}{}创建失败！", LOG_FILE_MSG, "网口接收线程");
  }
  if (result != 0)
  {
    logger->error("{}{}创建失败！", LOG_FILE_MSG, "串口接收线程");
  }
  if (result2 != 0)
  {
    logger->error("{}{}创建失败！", LOG_FILE_MSG, "后端接收线程");
  }
  if (result4 != 0)
  {
    logger->error("{}{}创建失败！", LOG_FILE_MSG, "云端控制监听线程");
  }
  if (result5 != 0)
  {
    logger->error("{}{}创建失败！", LOG_FILE_MSG, "云端控制ACK发送线程");
  }
  if (result6 != 0)
  {
    logger->error("{}{}创建失败！", LOG_FILE_MSG, "监听前端消息队列线程");
  }
  if (result8 != 0)
  {
    logger->error("{}{}创建失败！", LOG_FILE_MSG, "发送线程");
  }

  int result_serial = pthread_join(serial_recv_thread, NULL);
  if (result_serial != 0)
  {
    logger->error("{}{}加入失败！", LOG_FILE_MSG, "串口接收线程");
  }
  int result_backend = pthread_join(backend_thread, NULL);
  if (result_backend != 0)
  {
    logger->error("{}{}加入失败！", LOG_FILE_MSG, "后端接收线程");
  }
  int result_listen = pthread_join(listen_thread, NULL);
  if (result_listen != 0)
  {
    logger->error("{}{}加入失败！", LOG_FILE_MSG, "云端控制监听线程");
  }
  int result_cloud = pthread_join(cloud_thread, NULL);
  if (result_cloud != 0)
  {
    logger->error("{}{}加入失败！", LOG_FILE_MSG, "云端控制ACK发送线程");
  }
  int result_recv = pthread_join(recv_thread, NULL);
  if (result_recv != 0)
  {
    logger->error("{}{}加入失败！", LOG_FILE_MSG, "监听前端消息队列线程");
  }
  int result_send = pthread_join(send_thread, NULL);
  if (result_send != 0)
  {
    logger->error("{}{}加入失败！", LOG_FILE_MSG, "发送线程");
  }
  int result_ethernet = pthread_join(ethernet_thread, NULL);
  if (result_ethernet != 0)
  {
    logger->error("{}{}加入失败！", LOG_FILE_MSG, "网口接收线程");
  }
  destroy_global_variable();
  return 0;
}
