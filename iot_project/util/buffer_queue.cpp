#include "buffer_queue.h"

using namespace std;

BufferQueue::BufferQueue(/* args */)
{
    size = 0;
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
}

BufferQueue::~BufferQueue()
{
    clean_datas();
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

shared_ptr<Package> BufferQueue::get_package()
{
    shared_ptr<Package> package = nullptr;
    pthread_mutex_lock(&mutex);
    while (size == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    // 退出阻塞机制
    if(size == INT_MIN) {
        clean_datas();
        pthread_mutex_unlock(&mutex);
        return nullptr;
    }
    size--;
    package =  buffer_queue.front();
    buffer_queue.pop();
    pthread_mutex_unlock(&mutex);    
    return package;
}

uint32_t BufferQueue::add_package(shared_ptr<Package> package)
{  
    uint32_t pkg_size = 0;
    pthread_mutex_lock(&mutex);
    buffer_queue.push(package);
    size++;
    pthread_cond_signal(&cond);
    pkg_size = package->get_length();
    pthread_mutex_unlock(&mutex);
    return pkg_size;
}

void BufferQueue::clean_datas()
{
    while (!buffer_queue.empty())
    {
        buffer_queue.pop();
    }
    size = 0;
}

bool BufferQueue::is_empty()
{
    return buffer_queue.empty();
}

/**
 * @brief 结束get_package函数的阻塞
 * @details 在调用get_package时，队列里没有任何数据，就会一直在get_package函数中阻塞，等待数据到来。如果想要退出等待，就调用此函数。
 */
void BufferQueue::exit_cond_wait()
{
    pthread_mutex_lock(&mutex);
    size = INT_MIN;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}