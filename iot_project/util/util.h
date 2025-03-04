/**
 * @file util.h
 * @author YCChiang
 * @version 0.1
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include <spdlog/spdlog.h>
#include "buffer_queue.h"
#include <jsoncpp/json/json.h>

#define MAX_SEQUENCE_NUMBER 0x9999
#define MAX_TEXT 512
#define BACKEND_SEND_TO_FRONTEND_MSG_KEY 2000
#define FRONTEND_SEND_TO_BACKEND_MSG_KEY 2002

//消息结构体
struct my_msg_st{
    long int my_msg_type;                       //消息类型
    uint8_t anytext[MAX_TEXT];                     //消息数据
 };
Json::Value string_to_json(std::string json_str);
void init_logger();
void close_logger();
bool read_file_to_buffer(const std::string path, char* buffer, int max_length);
Json::Value read_json_file(std::string filename);
#endif
