#ifndef _EDGE_COMMUNICATION_H
#define _EDGE_COMMUNICATION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <iostream>
# include <queue>
#include "intreadwrite.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ping.h"
#include "variables.h"
#include "package.h"
#include "dealpk.h"
#include <spdlog/spdlog.h>
using namespace std;
extern bool commu_mode;
extern int m;
void SingnalHandler(int signo);
void *send_package_thread(void *arg);
void *ethernet_recv_thread(void *arg);
void *serial_port_receive_thread(void *arg);
bool ping_ethernet();
void open_serial();
bool on_ping();
void send_three_times_msg(shared_ptr<Package> package);
void delay(int seconds);//  参数必须为整型，表示延时多少秒
#endif