/**
 * @file error_no.h
 * @brief 定义错误码
 * @author YCChiang
 * @version 1.0
 * @date 2022-04-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _ERRORNO_H
#define _ERRORNO_H

#define EXC_SUCCESS             1

/// TCP socket 错误
#define TCP_SOCKET_ERR          -6
/// 串口未打开
#define SERIAL_PORT_NOT_OPEN    -7

/// 线程错误
#define PTHREAD_ERR             -8
/// 在报文头中无此数据项
#define NO_SUCH_ITEM            -11
/// 字符串转JSON错误
#define JSON_PARSE_ERR          -12
/// 系统中无此文件
#define NO_SUCH_FILE            -14
/// 执行脚本出错
#define EXECUT_SCRIPT_ERROR        -15

extern char const* error_message;

#endif