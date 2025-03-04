#ifndef _BACKEND_H
#define _BACKEND_H
#include <iostream>
#include "sync_cloud_msg.h"
#include "cryptmsg.h"
#include "edge_communication.h"
#include "status_alarm.h"
#include "variables.h"

using namespace std;
void *backend_recv_thread(void* arg);
void *recv_msg_thread(void *arg);
void send_msg_to_frontend(shared_ptr<Package> package);
#endif
