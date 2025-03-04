#include "package_process.h"
using namespace std;
#define LOG_FILE_MSG "[frontend package_process.cpp]: "
pthread_t frontend_tid = 0;
bool need_frontend_info = false;
void *frontend_msg_thread(void *arg)
{
#ifdef DEBUG
  pid_t pid = syscall(SYS_gettid);
  pthread_t tid = pthread_self();
  printf("frontend_msg_thread: pid=%d, tid=%lu\n", pid, tid);
#endif
  int msgid;
  struct my_msg_st data;
  long int msg_to_rcv = 0;
  auto logger = spdlog::get("project_log");
  // rcv msg
  msgid = msgget((key_t)BACKEND_SEND_TO_FRONTEND_MSG_KEY, 0664 | IPC_CREAT); // 获取消息队列
  if (msgid == -1)
  {
    logger->error("{}获取后端发送前端消息队列失败！", LOG_FILE_MSG);
  }
  while (need_frontend_info)
  {
    // 接收消息
    if (msgrcv(msgid, (void *)&data, BUFSIZ, msg_to_rcv, 0) == -1)
    {
      logger->error("{}从前端接收后端消息队列失败！", LOG_FILE_MSG);
      usleep(100);
      continue;
    }
    else
    {
    uint32_t pkg_size = AV_RL32(data.anytext + 1);
    shared_ptr<Package> package(new Package(pkg_size));
    package->parse_message(data.anytext, pkg_size);
    map<uint32_t, shared_ptr<Package>>::iterator t;
    t = recv_package.find(package->get_sequence_number());
    if (t != recv_package.end()) // 如果没有找到就存入map,否则就进行更新
      recv_package[package->get_sequence_number()] = package;
    else
    {
      recv_package.emplace(package->get_sequence_number(), package);
      map<uint32_t, shared_ptr<Package>>::iterator m;
      m = sent_package.find(package->get_sequence_number());
      if (m != sent_package.end())
        sent_package.erase(package->get_sequence_number());
    }
    package = nullptr;
  }
  }
}

int start_thread()
{
  int ret;
  need_frontend_info = true;
  ret = pthread_create(&frontend_tid, NULL, frontend_msg_thread, NULL);
  if (ret != 0)
  {
    frontend_tid = 0;
    ret = PTHREAD_ERR;
    goto error;
  }
  return EXC_SUCCESS;
error:
  if (frontend_tid > 0)
  {
    need_frontend_info = false;
    pthread_join(frontend_tid, NULL);
  }
  return ret;
}

void stop_thread()
{
  if (frontend_tid > 0)
  {
    need_frontend_info = false;
    pthread_join(frontend_tid, NULL);
  }
}