#include "AuthenProducer.h"
#include "KafkaProducer.h"
#define LOG_FILE_MSG "[kafka AuthenProducer.cpp]: "
// call back
class AuthenProducerDeliveryReportCb : public RdKafka::DeliveryReportCb
{
public:
	void dr_cb(RdKafka::Message &message) // 重载基类RdKafka::DeliveryReportCb中的虚函数dr_cb()
	{
		auto loggers = spdlog::get("project_log");
		if (message.err() != 0) // 发送出错
		{
			//bool is_sync;
			string msg;
			// KafkaProducer producer(local_brokers, "test_topic_name");
            KafkaProducer producer(local_brokers, "data_realtime_toflink");
			KafkaProducer alarm_producer(local_brokers, "StatusAlarm");
			char tmp[500] = {0};
			memset(tmp, '\0', sizeof(tmp));
			loggers->error("{}AuthenProducer Message delivery failed: {}", LOG_FILE_MSG, message.errstr());
			sprintf(tmp, "网络错误,边缘一体机无法发送到云端kafka:%s", message.errstr().c_str());
            shared_ptr<Package> alarm_package = send_alarm_msg(2, tmp);
			msg = string(alarm_package->get_message(), alarm_package->get_message() + alarm_package->get_length());
			producer.pushMessage(msg_encrypt(msg));
			alarm_producer.pushMessage(parse_alarm_to_kafka(alarm_package));
// if(!insert_status_alarm_info(2,tmp))
  //loggers->error("{}插入报警表失败！", LOG_FILE_MSG);// 插入报警表 报警原因是“网络错误，无法发送到kafka，message.errstr()”
#ifdef DEBUG
			cout << "AuthenProducer Message delivery failed: " << message.errstr() << endl;
#endif
		}
#ifdef DEBUG
		else
		{
			cout << "AuthenProducer Message delivered success!" << endl;
		}
#endif
	}
};

/*
class DeliveryReportCb {                        //Kafka将会在produce()之后返回发送结果时调用 DeliveryReportCb::dr_cb(), 并将结果填充到message中
public:
	virtual void dr_cb(Message &message) = 0;   //Message::err() 中保存返回结果
	virtual ~DeliveryReportCb() {}
};

返回的发送结果 Message 中包含的信息：
class Message {
public:
	virtual std::string     errstr() const = 0;     //返回错误原因
	virtual ErrorCode       err() const = 0;        //返回错误码，ErrorCode是enum枚举类型
	virtual Topic           *topic() const = 0;     //返回生产的消息所发送到的主题，返回值类型为 RdKafka::Topic 类对象
	virtual std::string     topic_name() const = 0; //返回生产的消息所发送到的主题名
	virtual int32_t         partition() const = 0;  //返回生产的消息所发送到的分区号

	virtual void            *payload() const = 0;   //消息内容
	virtual size_t          len() const = 0;        //消息长度
	virtual const std::string *key() const = 0;     //消息的key
	virtual int64_t         offset() const = 0;     //消息偏移量
	//...
};

enum ErrorCode {
	ERR_BEGIN = -200,
	//...
	ERR_UNKNOWN = -1,
	ERR_NO_ERROR = 0,   //SUCCESS
	ERR_OFFSET_OUT_OF_RANGE = 1,
	//...
};
*/
class AuthenProducerEventCb : public RdKafka::EventCb
{
public:
	void event_cb(RdKafka::Event &event)
	{
		auto loggers = spdlog::get("project_log");
		switch (event.type())
		{
		case RdKafka::Event::EVENT_ERROR: // 错误条件事件
			loggers->error("{}RdKafka::Event::EVENT_ERROR: {}", LOG_FILE_MSG, RdKafka::err2str(event.err()));
			//	std::cout << "RdKafka::Event::EVENT_ERROR: " << RdKafka::err2str(event.err()) << std::endl;
			break;
		case RdKafka::Event::EVENT_STATS: // Json文档统计事件
			// loggers->info("{}RdKafka::Event::EVENT_STATS: {}", LOG_FILE_MSG, event.str());
			//	std::cout << "RdKafka::Event::EVENT_STATS: " << event.str() << std::endl;
			break;
		case RdKafka::Event::EVENT_LOG: // Log消息事件
			loggers->info("{}RdKafka::Event::EVENT_LOG: {}", LOG_FILE_MSG, event.fac());
			//	std::cout << "RdKafka::Event::EVENT_LOG " << event.fac() << std::endl;
			break;
		case RdKafka::Event::EVENT_THROTTLE: // Log消息事件
			loggers->info("{}RdKafka::Event::EVENT_THROTTLE: {}", LOG_FILE_MSG, event.broker_name());
			//	std::cout << "RdKafka::Event::EVENT_THROTTLE " << event.broker_name() << std::endl;
			break;
		}
	}
};

// class HashPartitionerCb : public RdKafka::PartitionerCb//实现自定义分区
// {
// public:
// 	int32_t partitioner_cb(const RdKafka::Topic *topic, const std::string *key,
// 		int32_t partition_cnt, void *msg_opaque)
// 	{
// 		char msg[128] = { 0 };
// 		int32_t partition_id = generate_hash(key->c_str(), key->size()) % partition_cnt;
// 		//                          [topic][key][partition_cnt][partition_id]
// 		//                          :[test][6419][2][1]
// 		sprintf(msg, "HashPartitionerCb:topic:[%s], key:[%s]partition_cnt:[%d], partition_id:[%d]", topic->name().c_str(),
// 			key->c_str(), partition_cnt, partition_id);
// 	//	std::cout << msg << std::endl;
// 		return partition_id;
// 	}
// private:

// 	static inline unsigned int generate_hash(const char *str, size_t len)
// 	{
// 		unsigned int hash = 5381;
// 		for (size_t i = 0; i < len; i++)
// 			hash = ((hash << 5) + hash) + str[i];
// 		return hash;
// 	}
// };

// KafkaProducer::KafkaProducer(const std::string& brokers, const std::string& topic, int partition)
AuthenProducer::AuthenProducer(const std::string &brokers, const std::string &topic)
{
	m_brokers = brokers;
	m_topicStr = topic;
	//	m_partition = partition;
	/* 创建Kafka Conf对象 */
	auto loggers = spdlog::get("project_log");
	m_config = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL); // 全局配置
	if (m_config == NULL)
		std::cout << "Create RdKafka Conf failed." << std::endl;
		// loggers->error("{}Create RdKafka Conf failed.", LOG_FILE_MSG);

	/* 创建Topic Conf对象 */
	m_topicConfig = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC); // topic配置
	if (m_topicConfig == NULL)
		loggers->error("{}Create RdKafka Topic Conf failed.", LOG_FILE_MSG);
	// std::cout << "Create RdKafka Topic Conf failed." << std::endl;

	/* 设置Broker属性 */
	/*
	enum ConfResult{
	  CONF_UNKNOWN = -2,
	  CONF_INVALID = -1,
	  CONF_OK = 0
  };
	*/
	RdKafka::Conf::ConfResult errCode;
	std::string errorStr;
	m_dr_cb = new AuthenProducerDeliveryReportCb;
	// 设置dr_cb属性值
	errCode = m_config->set("dr_cb", m_dr_cb, errorStr); // 设置每个消息发送后的发送结果要回调
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
	}
	// 设置event_cb属性值
	m_event_cb = new AuthenProducerEventCb;
	errCode = m_config->set("event_cb", m_event_cb, errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
	}
	// 自定义分区策略
	// m_partitioner_cb = new HashPartitionerCb;
	// errCode = m_topicConfig->set("partitioner_cb", m_partitioner_cb, errorStr);
	// if (errCode != RdKafka::Conf::CONF_OK)
	// {
	// 	std::cout << "Conf set failed:" << errorStr << std::endl;
	// }
	// 设置配置对象的属性值
	errCode = m_config->set("statistics.interval.ms", "10000", errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
		// std::cout << "Conf set failed:" << errorStr << std::endl;
	}
    errCode = m_config->set("security.protocol", "SASL_PLAINTEXT",errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
		cout << "Conf set failed:" << errorStr << std::endl;
	}
    errCode = m_config->set("sasl.mechanisms", "PLAIN", errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
		cout << "Conf set failed:" << errorStr << std::endl;
	}
    errCode = m_config->set("sasl.username", "tly", errorStr);
    // errCode = m_config->set("sasl.username", "admin", errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
		cout << "Conf set failed:" << errorStr << std::endl;
	}
    errCode = m_config->set("sasl.password", "123456", errorStr);
    // errCode = m_config->set("sasl.password", "admin-secret", errorStr);
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
		cout << "Conf set failed:" << errorStr << std::endl;
	}
	errCode = m_config->set("message.max.bytes", "10240000", errorStr); // 设置发送端的最大字节数，如果发送的消息过大则返回失败
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
	}
	errCode = m_config->set("bootstrap.servers", m_brokers, errorStr); // 设置生产者待发送服务器的地址：ip:port格式
	if (errCode != RdKafka::Conf::CONF_OK)
	{
		loggers->error("{}Conf set failed:{}", LOG_FILE_MSG, errorStr);
		// std::cout << "Conf set failed:" << errorStr << std::endl;
	}

	/* 创建Producer */
	m_producer = RdKafka::Producer::create(m_config, errorStr);
	if (m_producer == NULL)
	{
		loggers->error("{}Create Producer failed:{}", LOG_FILE_MSG, errorStr);
		// std::cout << "Create Producer failed:" << errorStr << std::endl;
	}

	/* 创建Topic对象 */
	m_topic = RdKafka::Topic::create(m_producer, m_topicStr, m_topicConfig, errorStr);
	if (m_topic == NULL)
	{
		loggers->error("{}Create Topic failed: {}", LOG_FILE_MSG, errorStr);
		// std::cout << "Create Topic failed:" << errorStr << std::endl;
	}
}

AuthenProducer::~AuthenProducer()
{
	auto loggers = spdlog::get("project_log");
	while (m_producer->outq_len() > 0)
	{
		loggers->info("{}AuthenProducer等待所有未完成的produce请求数量：{}", LOG_FILE_MSG, m_producer->outq_len());
		// std::cerr << "Waiting for " << m_producer->outq_len() << std::endl;
		m_producer->flush(5000);
		/*
		ErrorCode flush (int timeout_ms):等待所有未完成的所有Produce请求完成。为了确保所有队列和已经执行的Produce请求在中止前完成，flush操作优先于销毁生产者实例完成。本函数会调用Producer::poll()函数，因此会触发回调函数。
		*/
	}
	delete m_config;
	delete m_topicConfig;
	delete m_topic;
	delete m_producer;
	delete m_dr_cb;
	delete m_event_cb;
	// delete m_partitioner_cb;
}

/* librdkafka提供的异步的生产接口，异步的消费接口和同步的消息接口，没有同步的生产接口。同一个生产者可以发送多个主题的，在内部处理时根据传入的topic对象发送给对应的主题分区。
 */
// void KafkaProducer::pushMessage(const std::string& str, const std::string& key)
// bool KafkaProducer::pushMessage(const uint8_t* str, const std::string& key, uint32_t& len)
// bool KafkaProducer::pushMessage(const std::string& str, const std::string& key)
bool AuthenProducer::pushMessage(const std::string &str)
{
	auto loggers = spdlog::get("project_log");
	int32_t len = str.length();
	void *payload = const_cast<void *>(static_cast<const void *>(str.data()));
	// void* payload = const_cast<void*>(static_cast<const void*>(str));
	RdKafka::ErrorCode errorCode = m_producer->produce(
		m_topic,						// 主题
		RdKafka::Topic::PARTITION_UA,	// 默认的随机方法   //生产者将消息发往的主题分区
		RdKafka::Producer::RK_MSG_COPY, // RK_MSG_COPY：拷贝playload;RK_MSG_FREE：发送后释放playload内容;RK_MSG_BLOCK:在消息队列满时阻塞produce函数
		payload,						// 消息
		len,							// 消息长度
		//&key,//key是可选的消息key，如果非NULL，会被传递给主题partitioner，并被随消息发送到Broker和传递给Consumer
		NULL,
		NULL);			 // msg_opaque是可选的应用程序提供给每条消息的opaque指针，opaque指针会在dr_cb回调函数内提供
	m_producer->poll(0); // poll()参数为0意味着不阻塞；poll(0)主要是为了触发应用程序提供的回调函数
	// m_producer->flush(5000);
	// std::cerr << "Waiting for " << m_producer->outq_len() << std::endl;
	if (errorCode != RdKafka::ERR_NO_ERROR)
	{
		loggers->error("{}生产消息失败：{}", LOG_FILE_MSG, RdKafka::err2str(errorCode));
		// std::cerr << "Produce failed: " << RdKafka::err2str(errorCode) << std::endl;
		if (errorCode == RdKafka::ERR__QUEUE_FULL)
		{
			m_producer->poll(100); // 如果发送失败的原因是队列正满，则阻塞等待一段时间
		}
		return false;
	}
	else
		return true;
}
