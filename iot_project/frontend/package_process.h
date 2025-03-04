#ifndef _PACKAGE_PROCESS_H
#define _PACKAGE_PROCESS_H
#include <map>
#include <string>
#include <sys/msg.h>
#include "package.h"
#include "util.h"
#include "dealpk.h"

using namespace std;
extern bool need_frontend_info;
void *frontend_msg_thread(void* arg);
int start_thread();
void stop_thread();
#endif