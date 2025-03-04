/**
 * @file util.cpp
 * @author YCChiang
 * @version 0.1
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "util.h"
#include <fstream>

#define LOG_FILE_MSG "[util util.cpp]: "

using namespace std;

/**
 * @brief 解析字符串成Json
 * @param  json_str         内容为json的字符串
 * @retval Json::Value      返回解析后的json
 * @retval Json::nullValue  无法解析此字符串
 */
Json::Value string_to_json(string json_str)
{
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(json_str, root);
    auto logger = spdlog::get("project_log");
    if ( !parsingSuccessful ) {
        logger->error("{}无法将字符串解析为JSON", LOG_FILE_MSG);
        return Json::nullValue;
    }
    return root;
}

void init_logger()
{
    auto max_size = 1024*1024*5;
    auto max_files = 10;
    auto logger = spdlog::rotating_logger_mt("project_log", "./logs/device.log", max_size, max_files);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [thread %t] %v");
    
#ifdef DEBUG
    logger->set_level(spdlog::level::debug);
#else
    logger->set_level(spdlog::level::info);
#endif
}

/**
 * @brief 关闭log库
 */
void close_logger()
{
    // Apply a function on all registered loggers
    spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info(": 结束运行"); });

    // Release and close all loggers
    spdlog::drop_all();
}
#define MAX_FILE_BUF_LEN 4096
/**
 * @brief   读取文件内容到buffer中
 * @param   path             文件路径
 * @param   buffer           缓冲地址
 * @param   max_length       缓冲最大的字节数
 * @retval  true             成功
 * @retval  false            失败
 */
bool read_file_to_buffer(const string path, char* buffer, int max_length)
{
    ifstream input_file(path);
    auto logger = spdlog::get("project_log");
    if (!input_file.is_open()) {
        logger->error("无法打开{}文件", path);
        input_file.close();
        return false;
    }
    input_file.read(buffer, max_length);
    input_file.close();
    return true;
}

/**
 * @brief 读取Json文件到Json数组中
 * 
 * @param filename 文件路径
 * @return Json::Value 读取到的数据
 */
Json::Value read_json_file(string filename)
{
    char file_buffer[MAX_FILE_BUF_LEN];
    auto logger = spdlog::get("project_log");
    if(!read_file_to_buffer(filename, file_buffer, MAX_FILE_BUF_LEN)) {
        logger->error("无法读取{}文件，请检查是否存在该文件", filename);
        return Json::nullValue;
    }
    Json::Value root = string_to_json(file_buffer);
    if(root.isNull()) {
        logger->error("无法解析{}文件，请检查该JSON文件格式", filename);
        return Json::nullValue;
    }
    return root;
}
