/**
 * @file device_status.h
 * @author YCChiang
 * @brief 获取边缘一体机的状态
 * @version 0.1
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _DEVICE_STATUS_H
#define _DEVICE_STATUS_H

#include <jsoncpp/json/json.h>

void get_device_status(Json::Value *info);
#endif