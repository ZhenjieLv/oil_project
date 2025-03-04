#include <iostream>
#include <pthread.h>
#include "KafkaProducer.h"
#include "KafkaConsumer.h"
#include "error_no.h"
using namespace std;
// pthread_t producer_tid = 0;
// pthread_t consumer_tid = 0;
// //bool need_producer_info = false;
// //bool need_consumer_info = false;
// int start_backend_thread()
// {
//     int ret;
//     //need_producer_info = true;
//     //need_consumer_info = true;
//     ret = pthread_create(&producer_tid, NULL, producer_thread, NULL);
//     if (ret != 0)
//     {
//         producer_tid = 0;
//         ret = PTHREAD_ERR;
//         goto error;
//     }
//     ret = pthread_create(&consumer_tid, NULL, consumer_thread, NULL);
//     if (ret != 0)
//     {
//         consumer_tid = 0;
//         ret = PTHREAD_ERR;
//         goto error;
//     }
//     // ret = pthread_create(&monitor_info_tid, NULL, sync_cloud_thread, NULL);
//     // if (ret != 0)
//     // {
//     //     monitor_info_tid = 0;
//     //     ret = PTHREAD_ERR;
//     //     goto error;
//     // }

//     return EXC_SUCCESS;
// error:
//     if (producer_tid > 0)
//     {
//        // g_edge_recv_queue->exit_cond_wait();
//         //need_producer_info = false;
//         pthread_join(producer_tid, NULL);
//     }
//     if (consumer_tid > 0)
//     {
//         //need_consumer_info = false;
//         pthread_join(consumer_tid, NULL);
//     }
//     // if (monitor_cloud_tid > 0)
//     // {
//     //     need_monitor_cloud_info = false;
//     //     pthread_join(monitor_cloud_tid, NULL);
//     // }
//    return ret;
// }
int main()
{
    pthread_t my_thread;
    pthread_t my_thread2;
    int result = pthread_create(&my_thread, NULL, producer_thread,NULL);
    int result2 = pthread_create(&my_thread2, NULL, consumer_thread,NULL); 
    
   if (result != 0) {
        perror("Thread creation failed");
        return 1;
    }
    result = pthread_join(my_thread, NULL);
    if (result != 0) {
        perror("Thread join failed");
        return 1;
    }
    if (result2 != 0) {
        perror("Thread creation failed");
        return 1;
    }
    result = pthread_join(my_thread2, NULL);
    if (result != 0) {
        perror("Thread join failed");
        return 1;
    }

}