#include <signal.h>
#include "ping.h"
#include <iostream>
using namespace std;
Ping * p;

void SingnalHandler(int signo) {

    p->statistic();

    exit(0);
}

int main(int argc, char * argv[]) {
    struct sigaction action;

    action.sa_handler = SingnalHandler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGINT,&action,NULL);

    Ping ping(argv[1], 1);
    p = &ping;
    ping.CreateSocket();
    // while(1)
    // {
    //     ping.SendPacket();
    //     ping.RecvPacket();
    //     sleep(1);
    // }
    bool check_mode = ping.SendPacket() && ping.RecvPacket();
  if (check_mode)
    cout<<"success"<<endl;
  else
    cout<<"fail"<<endl;
}