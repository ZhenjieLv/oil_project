#include "util.h"
#include "variables.h"
#include <sys/msg.h>

// 本机报文序列号
uint32_t sequence_number = 0;
uint32_t alarmid = 0;
// sequence_number的互斥锁
pthread_mutex_t seq_num_mutex;
pthread_mutex_t alarm_num_mutex;
string brokers = "117.119.65.12:8735";
string local_brokers = "localhost:9092";
// string brokers = "10.16.14.20:9092";
BufferQueue* g_edge_recv_queue = NULL;
BufferQueue* g_com_send_queue = NULL;
BufferQueue* g_cloud_send_queue = NULL;
atomic<bool> is_wireless(false);//false:有线、网口；true:无线、串口
#define LOG_FILE_MSG "[util variables.cpp]: "
/**
 * @brief  初始化全局变量
 */
void initialize_global_variable()
{
    g_edge_recv_queue = new BufferQueue();
    g_com_send_queue = new BufferQueue();
    g_cloud_send_queue = new BufferQueue();
    is_wireless = false;
    pthread_mutex_init(&seq_num_mutex, NULL);
    //创建消息队列
    int msgid=msgget((key_t)FRONTEND_SEND_TO_BACKEND_MSG_KEY,0664|IPC_CREAT);
    msgid=msgget((key_t)BACKEND_SEND_TO_FRONTEND_MSG_KEY,0664|IPC_CREAT);
}


/**
 * @brief 销毁全局变量
 */
void destroy_global_variable()
{
    delete g_edge_recv_queue;
    delete g_com_send_queue;
    delete g_cloud_send_queue;
    auto logger = spdlog::get("project_log");
    pthread_mutex_destroy(&seq_num_mutex);
    int msgid=msgget((key_t)FRONTEND_SEND_TO_BACKEND_MSG_KEY,0664|IPC_CREAT);//获取消息队列
    //删除消息队列
    if(msgctl(msgid,IPC_RMID,0)==-1){
      logger->error("{}前端消息队列删除失败！",LOG_FILE_MSG);
       // perror("msgctl err");
    }
    msgid=msgget((key_t)BACKEND_SEND_TO_FRONTEND_MSG_KEY,0664|IPC_CREAT);
    if(msgctl(msgid,IPC_RMID,0)==-1){
       logger->error("{}后端消息队列删除失败！",LOG_FILE_MSG);
       // perror("msgctl err");
    }
    close_logger();
}

/**
 * @brief   获得报文序号（0000-MAX_SEQUENCE_NUMBER）
 * @return  uint32_t 报文序号
 */
uint32_t get_sequence_number()
{
    uint32_t seq_num = 0;
    pthread_mutex_lock(&seq_num_mutex);
    seq_num = sequence_number;
    if(++sequence_number > MAX_SEQUENCE_NUMBER)
        sequence_number = 0;
    pthread_mutex_unlock(&seq_num_mutex);
    return seq_num;
}
uint32_t get_alarm_number()
{
    uint32_t alarm_num = 0;
    pthread_mutex_lock(&alarm_num_mutex);
    ++alarmid;
    alarm_num = alarmid;
    pthread_mutex_unlock(&alarm_num_mutex);
    return alarm_num;
}
/**
 * @return 获取系统当前时间
*/
std::string GetCurrentTimeStamp()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	std::tm* now_tm = std::localtime(&now_time_t);
	char buffer[128];
	strftime(buffer, sizeof(buffer), "%F %T", now_tm);
	std::ostringstream ss;
	ss.fill('0');
	std::chrono::milliseconds ms;
	ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	ss << buffer << "." << ms.count();	
	return ss.str();
}
/**
 * @return uint16_t 循环校验码
*/
uint16_t mb_crc16(uint8_t *pFrame, uint16_t len)
{
    uint8_t ucCRCHi = 0xFF;
    uint8_t ucCRCLo = 0xFF;
    int iIndex;

    while (len--)
    {
        iIndex = ucCRCLo ^ *(pFrame++);
        ucCRCLo = (uint8_t)(ucCRCHi ^ aucCRCHi[iIndex]);
        ucCRCHi = aucCRCLo[iIndex];
    }
    return (uint16_t)(ucCRCHi << 8 | ucCRCLo);
}
uint32_t get_alarm_id()
{
  std::vector<status_alarm_t> ret;
  ret = search_alarm_id();
  if(ret.empty())
    alarmid = 0;
  else
    alarmid = ret[0].alarm_id;
    return alarmid;
}