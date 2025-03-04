#ifndef KAFKACONSUMER_H
#define KAFKACONSUMER_H

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "variables.h"
#include "librdkafka/rdkafkacpp.h"
#include "cryptmsg.h"
#include "status_alarm.h"
#include "cryptmsg.h"
#include "dealpk.h"
#include "KafkaProducer.h"
class KafkaConsumer
{
public:/**
     * @brief KafkaConsumer
     * @param brokers
     * @param groupID
     * @param topics
     * @param partition
     */
    explicit KafkaConsumer(const std::string& brokers, const std::string& groupID,
                           const std::vector<std::string>& topics, int partition);
    void pullMessage();
    ~KafkaConsumer();
protected:
    std::string m_brokers;
    std::string m_groupID;
    std::vector<std::string> m_topicVector;//一个消费者可以同时订阅到多个主题，所以用vector
    int m_partition;
    RdKafka::Conf* m_config; //global级别的配置（consumer客户端级别）
    RdKafka::Conf* m_topicConfig;//topic级别的配置
    RdKafka::KafkaConsumer* m_consumer;//消费者客户端实例
    RdKafka::EventCb* m_event_cb;//Event事件回调
    RdKafka::RebalanceCb* m_rebalance_cb;//再均衡 回调  当消费者组中有一个消费者不工作，就会触发再平衡或者消费者处理信息的时间过长也会触发再平衡 会分配给其他消费者
};
void deal_consume_info(RdKafka::Message* msg);
void msg_consume(RdKafka::Message* msg, void* opaque);
//void *consumer_thread(void *arg);
#endif // KAFKACONSUMER_H