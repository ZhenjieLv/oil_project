#include "mytimer.h"
void printNum(shared_ptr<Package> package)
 {
    map<uint32_t, shared_ptr<Package>>::iterator t;
    t = cloud_package.find(package->get_sequence_number());
    if (t != cloud_package.end()) // 如果没有找到就存入map,否则就进行更新
      cloud_package.erase(package->get_sequence_number());
    else
    {
    KafkaProducer producer(local_brokers, "data_realtime_toflink");
	KafkaProducer alarm_producer(local_brokers, "StatusAlarm");
	char tmp[500] = {0};
    string msg;
	memset(tmp, '\0', sizeof(tmp));
	sprintf(tmp, "设备%d连接状态不好，无法收到回应！", package->get_dest_id());
    shared_ptr<Package> alarm_package = send_alarm_msg(package, tmp);
	msg = string(alarm_package->get_message(), alarm_package->get_message() + alarm_package->get_length());
	producer.pushMessage(msg_encrypt(msg));
	alarm_producer.pushMessage(parse_alarm_to_kafka(alarm_package));
    }
}
