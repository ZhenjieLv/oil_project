/**
 * @file package.h
 * @brief 用于报文组包和解析报文
 * @details 按照网络架构中报文格式进行组包和解包
 * @author YCChiang
 * @version 1.0
 * @date 2023-07-05
 * @copyright Copyright (c) 2023 
 */

#ifndef _PACKAGE_H
#define _PACKAGE_H

#include <unistd.h>
#include <cstdint>
#include <string>
#include <spdlog/spdlog.h>

/// 用于日志记录报文类型
extern const char *type_name[8];
/**
 * @brief 报文类型，参数类型为uint8_t
 */
typedef enum type: uint8_t {
    /// 下行查询报文
    DOWN_QUERY = 1,
    /// 查询反馈报文
    UP_QUERY_FEEDBACK,
    /// 定时上报报文
    UP_DEVICE_INFO,
    /// 下行控制报文
    DOWN_CONTROL,
    /// ACK报文
    ACK_MSG,
    // 数据重传报文
    SYNC_DATA,
    // 改变有线无线传输报文
    CHANGE_MODE
} type_t;

/**
 * @brief 报文类，用于组成报文和解析报文
 * @see examples/image_test.cpp
 */
class Package {
private:
    /* 
    *                |                 length                |
    *                |---------------------------------------|
    *                |                                       |
    *
    *              buffer       data        data_end
    *                +-----------+-------------+-------------+
    *                |/ / / / / /|/ / / / / / /|/ / / / / / /|
    *  buffer:       |/ header / |/ consumed / |/to be write/|
    *                |/ / / / / /|/ / / / / / /|/ / / / / / /|
    *                +-----------+-------------+-------------+
    * 
    */
    uint8_t *buffer;
    uint32_t data_length;

    std::shared_ptr<spdlog::logger> logger;

public:
    Package(uint32_t length);
    Package(uint32_t length, type_t t);
    ~Package();
    int parse_message(uint8_t *message, uint32_t msg_len, uint32_t serial_number = UINT32_MAX);
    int set_command(std::string& command);
    void set_type(type_t t);
    void set_dest_id(uint16_t id);
    void set_src_id(uint16_t id);
    int set_sequence_number(uint32_t number);
    void set_db_type(uint8_t id);
    void set_commu_type(bool is_wireless);
    type_t get_type();
    uint16_t get_dest_id();
    uint16_t get_src_id();
    uint32_t get_sequence_number();
    uint32_t get_length();
    uint32_t get_data_length();
    uint8_t get_db_type();
    std::string get_command();
    uint8_t* get_message();
    uint8_t* get_data();
};

/* 以下为处理报文的函数 */
uint32_t msg_get_length(uint8_t *message);
void log_message(uint8_t *message, const char *info, bool is_send);
#endif
