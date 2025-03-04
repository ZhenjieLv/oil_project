/**
 * @file package.cpp
 * @author YCChiang
 * @brief 按照网络架构中报文格式进行组包和解包
 * @version 0.1
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "package.h"
#include "intreadwrite.h"
#include "error_no.h"

#define LOG_FILE_MSG "[util package.cpp]: "

/// 报文类型在报文头的位置
constexpr int MSG_TYPE_IDX = 0;
/// 报文长度在报文头的位置
constexpr int MSG_LENGTH_IDX = MSG_TYPE_IDX+1;
/// 目的编号在报文头的位置
constexpr int MSG_DEST_IDX = MSG_LENGTH_IDX+4;
/// 源编号在报文头的位置
constexpr int MSG_SRC_IDX =  MSG_DEST_IDX+2;
/// 报文序号在报文头的位置
constexpr int MSG_SQC_NUM_IDX = MSG_SRC_IDX+2;
/// 命令在报文头的位置
constexpr int MSG_CMD_IDX = MSG_SQC_NUM_IDX+4;
/// 数据在报文头的位置
constexpr int MSG_DATA_IDX = MSG_SQC_NUM_IDX+4;
/// 上报数据在报文头的位置
constexpr int MSG_UP_DATA_IDX = MSG_SRC_IDX+2;
///数据库名称在报文头位置
constexpr int MSG_DB_IDX = MSG_SQC_NUM_IDX+4;
///数据重传报文数据在报文头位置
constexpr int MSG_DB_DATA_IDX = MSG_DB_IDX+1;
/// 通信模式数据在报文头位置
constexpr int MSG_COMMU_IDX = MSG_SQC_NUM_IDX+4;


using namespace std;

/// 用于日志记录报文类型
const char *type_name[8] = {
    "心跳报文（LTE）",
    "下行查询报文",
    "查询反馈报文",
    "定时上报报文",
    "下行控制报文",
    "ACK报文",
    "数据重传报文",
    "改变有线无线传输报文"
};

/**
 * @brief 通过报文类型获取报文头的长度
 * 
 * @param t 报文类型
 * @return uint32_t 报文头长度
 */
uint32_t get_header_length(type_t t)
{
    uint32_t header_len = 0;
    switch (t)
    {
    case DOWN_QUERY:
    case UP_QUERY_FEEDBACK:
    case DOWN_CONTROL:
    case ACK_MSG:
        header_len = MSG_SQC_NUM_IDX+4;
        break;
    case CHANGE_MODE:
        header_len = MSG_COMMU_IDX+1;
        break;
    case UP_DEVICE_INFO:
        header_len = MSG_SRC_IDX+2;
        break;
    case SYNC_DATA:
        header_len = MSG_DB_IDX+1;
        break;
    default:
        break;
    }
    return header_len;
}

/**
 * @brief   判断是否是需要ACK
 * @param   t       报文类型
 */
bool need_ack(type_t t)
{
    bool ret = false;
    //if(t==UP_DEVICE_INFO || t == CHANGE_MODE) 
    if(t==UP_DEVICE_INFO)
        return false;
    else 
        return true;
}

/**
 * @brief   Package构造函数，用于接收报文之后解析
 * @param   length  报文总长度（包括报文头）
 * @warning length必须大于0，构造函数中未设置判断
 */
Package::Package(uint32_t length)
{
    buffer = new uint8_t[length]();
    logger = spdlog::get("project_log");
}

/**
 * @brief   Package构造函数，用于组成报文后发送
 * @param   length           报文总长度（不包括报文头）
 * @param   t                报文的类型
 * @warning length必须大于等于0，构造函数中未设置判断
 */
Package::Package(uint32_t length, type_t t)
{
    uint32_t header_len = get_header_length(t);
    length += header_len;
    buffer = new uint8_t[length]();
    set_type(t);
    AV_WL32(buffer+MSG_LENGTH_IDX, length);
    //data_length = length;
    // TODO change src idx
    set_src_id(2);
    logger = spdlog::get("project_log");
}

Package::~Package()
{
    if(buffer)
        delete[] buffer;
}

/**
 * @brief   接收端解析报文
 * @param   message          需要解析的报文指针
 * @param   msg_len          报文长度
 * @retval  EXC_SUCCESS 成功
 */
int Package::parse_message(uint8_t *message, uint32_t msg_len, uint32_t serial_number)
{
    memcpy(buffer, message, msg_len);
    type_t t = get_type();
    data_length = msg_len-get_header_length(t);
    return EXC_SUCCESS;
}

/**
 * @brief   用于发送端组包，将data设置为报文体
 * @param   command             报文数据
 */
int Package::set_command(string& command)
{
    memcpy(buffer+MSG_CMD_IDX, command.c_str(), command.size());
    data_length = command.size();
    return EXC_SUCCESS;
}

/**
 * @brief   设置报文类型
 * @param   t          报文类型
 */
void Package::set_type(type_t t)
{
    buffer[MSG_TYPE_IDX] = t;
}

void Package::set_dest_id(uint16_t id)
{
    AV_WL16(buffer+MSG_DEST_IDX, id);
}

void Package::set_src_id(uint16_t id)
{
    AV_WL16(buffer+MSG_SRC_IDX, id);
}

/**
 * @brief   设置报文序号
 * @param   number      报文序号
 * @retval  EXC_SUCCESS 成功
 * @retval  NO_SUCH_ITEM 此类型报文的报文头无报文序号
 */
int Package::set_sequence_number(uint32_t number)
{
    type_t t = get_type();
    if(need_ack(t)) {
        AV_WL32(buffer+MSG_SQC_NUM_IDX, number);
        return EXC_SUCCESS;
    }
    logger->warn("{}报文类型为{}，报文头中没有报文序号，非法调用set_sequence_number",LOG_FILE_MSG, type_name[t]);
    return NO_SUCH_ITEM;
}
/**
 * @brief   设置数据库名称
 */
 void Package::set_db_type(uint8_t id)
 {
    buffer[MSG_DB_IDX] = id;
 }

void Package::set_commu_type(bool is_wireless)
{
    buffer[MSG_COMMU_IDX] = is_wireless? 0: 1;
}

 /**
 * @brief   获取数据库名称
 * @return  uint8_t
 */
uint8_t Package::get_db_type()
{
    return buffer[MSG_DB_IDX];
}
/**
 * @brief   获取报文类型
 * @return  type_t 报文类型
 */
type_t Package::get_type()
{
    return type_t(buffer[MSG_TYPE_IDX]);
}
                                             
/**
 * @brief   获取目的编号
 * @return  uint16_t 目的编号
 */
uint16_t Package::get_dest_id()
{
    return AV_RL16(buffer+MSG_DEST_IDX);
}

/**
 * @brief   获取源编号
 * @return  uint16_t 源编号
 */
uint16_t Package::get_src_id()
{
    return AV_RL16(buffer+MSG_SRC_IDX);
}

/**
 * @brief   获取报文序号
 * @return  uint32_t 报文序号
 * @retval  0000-MAX_SEQUENCE_NUMBER 报文序号
 * @retval  UINT32_MAX 此类型报文的报文头无报文序号
 */
uint32_t Package::get_sequence_number()
{
    type_t t = get_type();
    if(need_ack(t)) {
        return AV_RL32(buffer+MSG_SQC_NUM_IDX);
    }
    logger->warn("{}报文类型为{}，报文头中没有报文序号，非法调用get_sequence_number",LOG_FILE_MSG, type_name[t]);
    return UINT32_MAX;
}

/**
 * @brief   获取打包好的报文
 * @return  报文的头指针
 */
uint8_t* Package::get_message()
{
    return buffer;
}

/**
 * @brief   获取报文的总长度
 * @return  长度
 */
uint32_t Package::get_length()
{
    return AV_RL32(buffer+MSG_LENGTH_IDX);
}

/**
 * @brief 获取去除报文头后的数据头指针
 * @return uint8_t* 数据的头部指针
 */
uint8_t* Package::get_data()
{
   /* if(get_type() == UP_DEVICE_INFO)
        return buffer+MSG_UP_DATA_IDX;
    else
        return buffer+MSG_DATA_IDX;*/
  if(get_type() == UP_DEVICE_INFO)
        return buffer+MSG_UP_DATA_IDX;
  else
  if(get_type() == SYNC_DATA)
        return buffer+MSG_DB_DATA_IDX;
  else
        return buffer+MSG_DATA_IDX;
}

/**
 * @brief 获取数据长度
 * @return uint32_t 数据长度
 * @retval 0 此报文不是定时上报报文
 */
uint32_t Package::get_data_length()
{
    return data_length;
}

/**
 * @brief 获取下行查询报文或下行控制报文的控制命令
 * @return string 控制命令
 * @retval "" 此报文不是下行查询报文或下行控制报文
 */
string Package::get_command()
{
    if(get_type() == DOWN_QUERY || get_type() == DOWN_CONTROL)
        return string(buffer+MSG_CMD_IDX, buffer+MSG_CMD_IDX+data_length);
    else
        return string("");
}

/**
 * @brief  获得报文长度
 * @param  message    报文指针
 * @return uint32_t 报文长度
 */
uint32_t msg_get_length(uint8_t *message)
{
    return AV_RL32(message + MSG_LENGTH_IDX);
}

/**
 * @brief log报文
 * @param message 报文指针
 * @param info log时需要附加的信息
 * @param is_send 是发送的报文还是接收的报文，用于确定是记录目的地址还是源地址
 */
void log_message(uint8_t *message, const char *info, bool is_send)
{
    auto logger = spdlog::get("project_log");
    // type_t t = msg_get_type(message);
    char buffer[1000] = {0}, *ptr;
    // sprintf(buffer, ": %s报文，类型：%s，%s：%s，长度：%d", info, type_name[t], 
    //     is_send? "目的编号" : "源编号", 
    //     is_send?  : msg_get_src_ip(message).c_str(), 
    //     msg_get_length(message));
    // if(is_upload_message(t)) {
    //     uint32_t total_num = msg_get_total_number(message);
    //     ptr = buffer + strlen(buffer);
    //     if(total_num == UINT32_MAX)
    //         sprintf(ptr, "，序号：%d", msg_get_sequence_number(message));
    //     else
    //         sprintf(ptr, "，序号：%d，分包信息：%d/%d", msg_get_sequence_number(message), msg_get_serial_number(message), total_num);
    // }
    logger->info(buffer);   
}
