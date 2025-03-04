#ifndef AUTHENPRODUCER_H
#define AUTHENPRODUCER_H


#pragma once
#include <string>
#include <iostream>
#include "librdkafka/rdkafkacpp.h"
//#include "rdkafkacpp.h"
#include "variables.h"
#include <spdlog/spdlog.h>
#include "status_alarm.h"
#include "cryptmsg.h"
#include "dealpk.h"
class AuthenProducer
{
public:
	/**
	* @brief AuthenProducer
	* @param brokers
	* @param topic
	*/
	//explicit KafkaProducer(const std::string& brokers, const std::string& topic, int partition);
  explicit AuthenProducer(const std::string& brokers, const std::string& topic);
	/**
	* @brief push Message to Kafka
	* @param str, message data
	*/
//bool pushMessage(const std::string& str, const std::string& key);
bool pushMessage(const std::string& str);
//void pushMessage(const std::string& str, const std::string& key);
//bool pushMessage(const uint8_t* str, const std::string& key, uint32_t& len);
	~AuthenProducer();


private:
	std::string m_brokers;			// Broker列表，多个使用逗号分隔
	std::string m_topicStr;			// Topic名称
	//int m_partition;				// 分区
	RdKafka::Conf* m_config;        // Kafka Conf对象 配置接口类，用来设置对生产者、消费者、broker的各项配置值
	RdKafka::Conf* m_topicConfig;   // Topic Conf对象
	RdKafka::Topic* m_topic;		// Topic对象
	RdKafka::Producer* m_producer;	// Producer对象


	/*只要看到Cb 结尾的类，要继承它然后实现对应的回调函数*/
	RdKafka::DeliveryReportCb* m_dr_cb;//RdKafka::DeliveryReportCb 用于在调用 RdKafka::Producer::produce() 后返回发送结果，RdKafka::DeliveryReportCb是一个类，需要自行填充其中的回调函数及处理返回结果的方式，为了使用队列化的投递报告回调函数，必须调用RdKafka::poll()函数。
	RdKafka::EventCb* m_event_cb;//RdKafka::EventCb 用于从librdkafka向应用程序传递errors,statistics,logs 等信息的通用接口
//	RdKafka::PartitionerCb* m_partitioner_cb;//Rdkafka::PartitionCb 用于设定自定义分区器
};

//void *producer_thread(void *arg);
// bool kafka_push(const std::string& str);
#endif