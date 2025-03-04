#include "KafkaConsumer.h"
// #include "AuthenConsumer.h"
#define LOG_FILE_MSG "[kafka KafkaConsumer.cpp]: "
class ConsumerEventCb : public RdKafka::EventCb // RdKafka::EventCb  事件是从RdKafka传递错误、统计信息、日志等消息到应用程序的通用接口
{
public:
  void event_cb(RdKafka::Event &event) // 事件回调函数 virtual void event_cb(Event &event)=0;
  {
    auto loggers = spdlog::get("project_log");
    switch (event.type())
    {
    case RdKafka::Event::EVENT_ERROR: // 错误条件时间
      loggers->error("{}ERROR({}):{}", LOG_FILE_MSG, RdKafka::err2str(event.err()), event.str());
      break;

    case RdKafka::Event::EVENT_STATS: // json文档统计事件
      loggers->info("{}\"STATS\":{} ", LOG_FILE_MSG, event.str());
      break;

    case RdKafka::Event::EVENT_LOG: // log消息事件
      loggers->info("{}LOG-{}-{}: {}", LOG_FILE_MSG, event.severity(), event.fac().c_str(), event.str().c_str());
      break;
    case RdKafka::Event::EVENT_THROTTLE: // 来自brokerde throttle级信号事件
      loggers->error("{}THROTTLED: {} ms by {} id {}", LOG_FILE_MSG, event.throttle_time(), event.broker_name(), event.broker_id());
      break;
    default:
      loggers->error("{}EVENT {} ({}):{}", LOG_FILE_MSG, event.type(), RdKafka::err2str(event.err()), event.str());
      break;
    }
  }
};
/*
当kafka遇到如下四种情况，kafka会触发Rebalance机制：
1、消费组成员发生了变更，比如有新的消费者加入了消费者组或者消费者宕机
2、消费者无法在指定的时间内完成消息的消费
3、消费组订阅的topic发生了变化
4、订阅的topic的partition发生了变化
*/
class ConsumerRebalanceCb : public RdKafka::RebalanceCb
{
public:
  void rebalance_cb(RdKafka::KafkaConsumer *consumer,
                    RdKafka::ErrorCode err,
                    std::vector<RdKafka::TopicPartition *> &partitions) // kafka服务端通过err参数传入再均衡的具体事件（发生前，发生后），通过partitions参数传入再均衡 前/后，旧的/新的 分区信息
  {
    auto loggers = spdlog::get("project_log");
    for (unsigned int i = 0; i < partitions.size(); i++)
    {
      loggers->error("{}RebalanceCb:{}：{}[{}], ", LOG_FILE_MSG, RdKafka::err2str(err), partitions[i]->topic(), partitions[i]->partition());
    }
    if (err == RdKafka::ERR__ASSIGN_PARTITIONS) // ERR__ASSIGN_PARTITIONS: 表示“再均衡发生之后，消费者开始消费之前”，此时消费者客户端可以从broker上重新加载offset
    {
      consumer->assign(partitions); // 再均衡后，重新assign()订阅这些分区
      partition_count = (int)partitions.size();
    }
    else if (err == RdKafka::ERR__REVOKE_PARTITIONS) // ERR__REVOKE_PARTITIONS: 表示“消费者停止消费之后，再均衡发生之前”，此时应用程序可以在这里提交 offset
    {
      consumer->unassign(); // 再均衡前，unassign() 退订这些分区
      partition_count = 0;  // 退订所有分区后，清0
    }
    else
    {
      loggers->error("{}Rebalancing error:{}", LOG_FILE_MSG, RdKafka::err2str(err));
    }
  }

private:
  int partition_count; // 保存consumer消费者客户端 当前订阅的分区数
};

KafkaConsumer::KafkaConsumer(const std::string &brokers, const std::string &groupID,
                             const std::vector<std::string> &topics, int partition)
{
  m_brokers = brokers;
  m_groupID = groupID;
  m_topicVector = topics;
  m_partition = partition;
  auto loggers = spdlog::get("project_log");
  std::string errorStr;
  RdKafka::Conf::ConfResult errorCode;                          // 设置配置对象的属性值，成功返回CONF_OK,错误时错误信息输出到errstr
  m_config = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL); // 创建配置对象

  m_event_cb = new ConsumerEventCb;
  errorCode = m_config->set("event_cb", m_event_cb, errorStr); // 设置event_cb属性
  if (errorCode != RdKafka::Conf::CONF_OK)
  {
    loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
  }

  m_rebalance_cb = new ConsumerRebalanceCb;
  errorCode = m_config->set("rebalance_cb", m_rebalance_cb, errorStr);
  if (errorCode != RdKafka::Conf::CONF_OK)
  {
    loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
  }
  // errorCode = m_config->set("security.protocol", "SASL_PLAINTEXT",errorStr);
	// if (errorCode != RdKafka::Conf::CONF_OK)
	// {
	// 	loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
	// }
  // errorCode = m_config->set("sasl.mechanisms", "PLAIN", errorStr);
	// if (errorCode != RdKafka::Conf::CONF_OK)
	// {
	// 	loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
	// }
  // errorCode = m_config->set("sasl.username", "tly", errorStr);
	// if (errorCode != RdKafka::Conf::CONF_OK)
	// {
	// 	loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
	// }
  // errorCode = m_config->set("sasl.password", "123456", errorStr);
  // if (errorCode != RdKafka::Conf::CONF_OK)
	// {
	// 	loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
	// }
  errorCode = m_config->set("enable.partition.eof", "false", errorStr); // enable.partition.eof: 当消费者到达分区结尾，发送 RD_KAFKA_RESP_ERR__PARTITION_EOF 事件，默认值 true
  if (errorCode != RdKafka::Conf::CONF_OK)
  {
    loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
  }
  errorCode = m_config->set("group.id", m_groupID, errorStr); // 设置消费组名：group.id（string类型）
  if (errorCode != RdKafka::Conf::CONF_OK)
  {
    loggers->error("{}Conf set 'group.id' failed: {}", LOG_FILE_MSG, errorStr);
  }
  errorCode = m_config->set("bootstrap.servers", m_brokers, errorStr);
  if (errorCode != RdKafka::Conf::CONF_OK)
  {
    loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
  }
  errorCode = m_config->set("max.partition.fetch.bytes", "1024000", errorStr); // 消费消息的最大大小
  if (errorCode != RdKafka::Conf::CONF_OK)
  {
    loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
  }

  // partition.assignment.strategy  range,roundrobin

  m_topicConfig = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
  // 获取最新的消息数据
  errorCode = m_topicConfig->set("auto.offset.reset", "latest", errorStr); // 这个参数是针对新的groupid中的消费者而言，当有新groupid的消费者来消费指定的topic时，对于该参数的配置，会有不同的语义。earliest：在各分区下有提交的offset时，从offset处开始消费，在各分区下无提交的offset时，从头开始消费。latest:在各分区下有新提交的offset时，从offset处开始消费，在各分区下无提交的offset时，从最新的数据开始消费
  // errorCode = m_topicConfig->set("auto.offset.reset", "earliest", errorStr);
  if (errorCode != RdKafka::Conf::CONF_OK)
  {
    loggers->error("{}Topic Conf set failed: {}", LOG_FILE_MSG, errorStr);
  }
  errorCode = m_config->set("default_topic_conf", m_topicConfig, errorStr); // 对于自动订阅的主题设置默认的topic属性
  if (errorCode != RdKafka::Conf::CONF_OK)
  {
    loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
  }
  m_consumer = RdKafka::KafkaConsumer::create(m_config, errorStr); // 创建Consumer消费者客户端
  if (m_consumer == nullptr)
  {
    loggers->error("{}Create KafkaConsumer failed: {}", LOG_FILE_MSG, errorStr);
  }
}
void deal_consume_info(RdKafka::Message *msg)
{
  string str = msg_decrypt(string((uint8_t *)msg->payload(), (uint8_t *)msg->payload() + msg->len()));
  uint8_t buffer[MAX_BUF_LEN];
  memset(buffer, 0, MAX_BUF_LEN);
  memcpy(buffer, str.c_str(), str.size());
  shared_ptr<Package> package(new Package(str.size()));
  package->parse_message(buffer, str.size());
  cloud_action_record.emplace(package->get_sequence_number(), package);
  g_com_send_queue->add_package(package);
}

void msg_consume(RdKafka::Message *msg, void *opaque)
{
  auto loggers = spdlog::get("project_log");
  //bool is_sync;
  string enmsg;
  switch (msg->err())
  {
  case RdKafka::ERR__TIMED_OUT:
    loggers->warn("{}Consumer error: {}", LOG_FILE_MSG, msg->errstr()); // 超时
    break;
  case RdKafka::ERR_NO_ERROR: // 有消息进来
    deal_consume_info(msg);
    break;
  default:
    loggers->error("{}Consumer error: {}", LOG_FILE_MSG, msg->errstr().c_str());
    // KafkaProducer producer(local_brokers, "test_topic_name");
    KafkaProducer producer(local_brokers, "data_realtime_toflink");
    KafkaProducer alarm_producer(local_brokers, "StatusAlarm");
    char tmp[500] = {0};
    memset(tmp, '\0', sizeof(tmp));
    sprintf(tmp, "网络错误,无法从kafka获得消息:%s", msg->errstr().c_str());
    shared_ptr<Package> alarm_package = send_alarm_msg(2, tmp);
    enmsg = string(alarm_package->get_message(), alarm_package->get_message() + alarm_package->get_length());
    producer.pushMessage(msg_encrypt(enmsg));
    alarm_producer.pushMessage(parse_alarm_to_kafka(alarm_package));    
    // if (!insert_status_alarm_info(2, tmp))
    //   loggers->error("{}插入报警表失败！", LOG_FILE_MSG); // 插入报警表 报警原因是“网络错误，无法发送到kafka，message.errstr()”
    break;
  }
}

void KafkaConsumer::pullMessage()
{
  auto loggers = spdlog::get("project_log");
  // 订阅Topic
  RdKafka::ErrorCode errorCode = m_consumer->subscribe(m_topicVector); // Consumer消费者订阅topic主题
  if (errorCode != RdKafka::ERR_NO_ERROR)
  {
    loggers->error("{}subscribe failed: {}", LOG_FILE_MSG, RdKafka::err2str(errorCode));
    // std::cout << "subscribe failed: " << RdKafka::err2str(errorCode) << std::endl;
  }
  // 消费消息
  while (true)
  {
    // cout<<"consume!"<<endl;
    RdKafka::Message *msg = m_consumer->consume(1000); // 若超过1000ms未订阅到消息，就会触发RdKafka::ERR_TIMED_OUT
    msg_consume(msg, NULL);
    delete msg;
  }
}

KafkaConsumer::~KafkaConsumer()
{
  m_consumer->close();
  delete m_config;
  delete m_topicConfig;
  delete m_consumer;
  delete m_event_cb;
  delete m_rebalance_cb;
}
