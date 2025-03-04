/**
 * @file buffer_queue.h
 * @brief 缓存队列
 * @author YCChiang
 * @version 1.0
 * @date 2023-10-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _BUFFER_QUEUE_H
#define _BUFFER_QUEUE_H

#include <queue>
#include <pthread.h>
#include "package.h"
#include <memory>
/**
 * @brief 缓存队列，用于缓存收到并且组好的Package报文
 */
class BufferQueue
{
private:
    std::queue<std::shared_ptr<Package>> buffer_queue;
    int size;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
public:
    BufferQueue(/* args */);
    ~BufferQueue();
    std::shared_ptr<Package> get_package();
    uint32_t add_package(std::shared_ptr<Package> package);
    void clean_datas();

    bool is_empty();
    void exit_cond_wait();
};
#endif