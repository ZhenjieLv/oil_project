#ifndef _SYNC_CLOUD_MSG_H
#define _SYNC_CLOUD_MSG_H
#include "controller.h"
#include "controllerrecord.h"
#include "mandrel.h"
#include "mandrelrecord.h"
#include "user.h"
#include "useraction.h"
#include "well.h"
#include "wellrecord.h"
#include "status_alarm.h"
#include "package.h"
#include "variables.h"
#include <cstdint>
#include <string>
#include "dealpk.h"
#include "intreadwrite.h"
#include "KafkaProducer.h"
#include "KafkaConsumer.h"
#include "AuthenProducer.h"
#include "AuthenConsumer.h"
#include "cryptmsg.h"
#include "actionrecord.h"
using namespace std;

void *listen_cloud_control_thread(void *arg);
void *cloud_control_ACK_thread(void *arg);
#endif