#include "monitortimer.h"
void SingnalHandler(int signo)
{
    exit(0);

}
 void monitor_network()
 {
  struct sigaction action;
  action.sa_handler = SingnalHandler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGINT, &action, NULL);
  Ping *p;
  Ping ping("117.119.65.12", 1);
  p = &ping;
  ping.CreateSocket();
  bool check_mode = ping.SendPacket() && ping.RecvPacket();
  if (!check_mode)
  {
    KafkaProducer producer(local_brokers, "data_realtime_toflink");
	KafkaProducer alarm_producer(local_brokers, "StatusAlarm");
	char tmp[500] = {0};
    string msg;
	memset(tmp, '\0', sizeof(tmp));
	sprintf(tmp, "当前网络状况不佳, 暂时无法同步！");
    shared_ptr<Package> alarm_package = send_alarm_msg(2, tmp);
	msg = string(alarm_package->get_message(), alarm_package->get_message() + alarm_package->get_length());
	producer.pushMessage(msg_encrypt(msg));
	alarm_producer.pushMessage(parse_alarm_to_kafka(alarm_package));
  }
   
}

