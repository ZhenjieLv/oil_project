#include "edge_communication.h"
int ethernet_fd = -1;
pthread_mutex_t ethernet_mutex = PTHREAD_MUTEX_INITIALIZER;
int serial_port_fd = -1; // 串口
pthread_mutex_t serial_port_mutex = PTHREAD_MUTEX_INITIALIZER;
struct termios config;
#define PORT 8288
// #define PORT 8088
queue<uint8_t> buffer_t;
queue<uint8_t> buffer_tmp;
Ping *p;
bool commu_mode = false;
shared_ptr<Package> pkg;
int m = 1;
#define LOG_FILE_MSG "[util edge_communication.cpp]: "
void SingnalHandler(int signo)
{
  exit(0);
}
void send_three_times_msg(shared_ptr<Package> package)
{
  int cnt = 3;
  while (cnt--)
  {
    g_com_send_queue->add_package(package);
    delay(1);
  }
}
void delay(int seconds) //  参数必须为整型，表示延时多少秒
{
  clock_t start = clock();
  clock_t lay = (clock_t)seconds * CLOCKS_PER_SEC;
  while ((clock() - start) < lay);
}
bool on_ping()
{
  auto logger = spdlog::get("project_log");
  if (ping_ethernet())
  {
    pkg = send_change_mode_msg(is_wireless);
#ifdef DEBUG
    printf("网口：");
    for (int i = 0; i < pkg->get_length(); i++)
    { 
      printf("%x ", pkg->get_message()[i]);
    }
#endif
    return true;
  }
  else
  {
    is_wireless = !is_wireless;
    pkg = send_change_mode_msg(is_wireless);
#ifdef DEBUG
    printf("串口：");
    for (int i = 0; i < pkg->get_length(); i++)
    {
      printf("%x ", pkg->get_message()[i]);
    }
#endif 
    logger->error("{} ping 169.254.254.232 fails", LOG_FILE_MSG);
    return false;
  }
}
void open_serial()
{
  auto logger = spdlog::get("project_log");
  serial_port_fd = open("/dev/ttyTHS0", O_RDWR | O_NOCTTY); // 打开串口
  if (serial_port_fd == -1)
  {
    logger->error("{}Error opening serial port", LOG_FILE_MSG);
  }

  memset(&config, 0, sizeof(struct termios)); // 配置串口
  if (tcgetattr(serial_port_fd, &config) != 0)
  {
    logger->error("{}Error getting serial port attributes", LOG_FILE_MSG);
    close(serial_port_fd);
  }

  cfsetispeed(&config, B115200);                     // 设置输入波特率为115200
  cfsetospeed(&config, B115200);                     // 设置输出波特率为115200
  config.c_cflag |= (CLOCAL | CREAD);                // 忽略调制解调器状态线，启用接收器
  config.c_cflag &= ~PARENB;                         // 禁用奇偶校验
  config.c_cflag &= ~CSTOPB;                         // 使用一个停止位
  config.c_cflag &= ~CSIZE;                          // 屏蔽数据位设置
  config.c_cflag |= CS8;                             // 设置数据位为8位
  config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 设置为非规范模式
  config.c_iflag &= ~(IXON | IXOFF | IXANY);         // 忽略软件流控制
  config.c_cflag &= ~CRTSCTS;                        // 禁用硬件流控制
  config.c_cc[VMIN] = 1;                             // 至少读取1个字符
  config.c_cc[VTIME] = 0;                            // 不使用超时
  config.c_iflag &= ~(INLCR | ICRNL);                // 0x0a 0x0d
  config.c_iflag &= ~(IXON);                         // 0x11 0x13
  config.c_iflag &= ~(INLCR | ICRNL | IGNCR);
  config.c_oflag &= ~(ONLCR | OCRNL);
  if (tcsetattr(serial_port_fd, TCSANOW, &config) != 0)
  {
    logger->error("{}Error setting serial port attributes", LOG_FILE_MSG);
    close(serial_port_fd);
  }
  return;
}

void *send_package_thread(void *arg)
{
  uint8_t buffer[MAX_BUF_LEN] = {0};
  if (commu_mode == false)
    send_three_times_msg(pkg);
  while (true)
  {
    shared_ptr<Package> package = g_com_send_queue->get_package();
    memset(buffer, 0, MAX_BUF_LEN);
    uint32_t length = package->get_length();
    buffer[0] = 0xFF;
    memcpy(buffer + 1, package->get_message(), length);
    buffer[length + 1] = 0xEE;
    if (!is_wireless && ethernet_fd != -1)
    {
      pthread_mutex_lock(&ethernet_mutex);
      int flag_mode = fcntl(ethernet_fd, F_GETFL, 0);
      flag_mode &= ~O_NONBLOCK;  //非阻塞设置为阻塞
      fcntl(ethernet_fd, F_SETFL, flag_mode);
      int result = write(ethernet_fd, buffer, length + 2);
#ifdef DEBUG
      cout << "网口write:" << result << endl;
#endif
      pthread_mutex_unlock(&ethernet_mutex);
#ifdef DEBUG
      cout << "write:ethernet_fd:" << ethernet_fd << endl;
      printf("网口发送数据： ");
      for (int i = 0; i < length + 2; i++)
      {
        printf("%x ", buffer[i]);
      }
      printf("\n");
#endif
    }
    else
    {
      // 串口发送
      pthread_mutex_lock(&serial_port_mutex);
      int result = write(serial_port_fd, buffer, length + 2);
      if(result < 0)
      {
        close(serial_port_fd);
        serial_port_fd = -1;
        open_serial();
        usleep(500);
      }
      pthread_mutex_unlock(&serial_port_mutex);
#ifdef DEBUG
      cout << "串口write:" << result << endl;
      if (result < 0)
      {
        cout << "write:" << strerror(errno) << endl;
      }
#endif
#ifdef DEBUG
      printf("串口发送数据： ");
      for (int i = 0; i < length + 2; i++)
      {
        printf("%x ", buffer[i]);
      }
      printf("\n");
#endif
    }
  }
}

void *ethernet_recv_thread(void *arg)
{
  auto logger = spdlog::get("project_log");
#ifdef DEBUG
  pid_t pid = syscall(SYS_gettid);
  pthread_t tid = pthread_self();
  printf("ethernet_recv_thread: pid=%d, tid=%lu\n", pid, tid);
#endif
  struct sockaddr_in servaddr; // 定义服务器对应的套接字地址
                               // 定义服务器监听套接字和连接套接字
  int listen_fd = -1;
  int len_recv = 0;
  // 服务器接收和发送缓冲区
  uint8_t recbuf[MAX_BUF_LEN];
  // 初始化套接字地址结构体
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;   // IPv4
  servaddr.sin_port = htons(PORT); // 设置监听端口
  // servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//表示接收任意IP的连接请求
  inet_aton("169.254.254.232", &servaddr.sin_addr);

  // 创建套接字
  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    // 如果创建套接字失败，返回错误信息
    // strerror(int errnum)获取错误的描述字符串
    logger->error("{}create socket error: {}(error: {})", LOG_FILE_MSG, strerror(errno), errno);
    // printf("create socket error: %s(error: %d)\n", strerror(errno), errno);
    exit(0);
  }
  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  // 绑定套接字和本地IP地址和端口
  if (bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
  {
    // 绑定出现错误
    // logger->error("{}bind socket error: {}(error: {})", LOG_FILE_MSG, strerror(errno), errno);
    printf("bind socket error: %s(error: %d)\n", strerror(errno), errno);
    // exit(0);
  }
  // 使得listen_fd变成监听描述符
  if (listen(listen_fd, 100) == -1)
  {
    logger->error("{}listen socket error: {}(error: {})", LOG_FILE_MSG, strerror(errno), errno);
    printf("listen socket error: %s(error: %d)\n", strerror(errno), errno);
    // exit(0);
  }
  while (true)
  {
    // accept阻塞等待客户端请求
    if ((ethernet_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL)) == -1)
    { // 阻塞等待客户端连接
      logger->error("{}accept socket error: {}(error: {})", LOG_FILE_MSG, strerror(errno), errno);
      continue;
    }
    #ifdef DEBUG
    cout << "保持连接" << endl;
    cout << "recv:ethernet_fd:" << ethernet_fd << endl;
    #endif
    if (commu_mode == true && m == 1)
      send_three_times_msg(pkg);
    m = 2;
    // 可以一直保持连接
    while (true)
    {
      pthread_mutex_lock(&ethernet_mutex);
      int flags = fcntl(ethernet_fd, F_GETFL, 0);
      flags |= O_NONBLOCK;
      fcntl(ethernet_fd, F_SETFL, flags);//非阻塞读取数据
      len_recv = read(ethernet_fd, recbuf, sizeof(recbuf));
      pthread_mutex_unlock(&ethernet_mutex);
      if (len_recv < 0)
      {
        if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
        {
          continue;
        }
        else
        {
          #ifdef DEBUG
          cout << "重新建立连接！" << endl;
          cout << "read:" << strerror(errno) << endl;
          #endif
          // if (is_wireless == false) // 有线则切换格式
          // {
          //   shared_ptr<Package> commu_pkg(new Package(0, CHANGE_MODE));
          //   commu_pkg->set_dest_id(0);
          //   commu_pkg->set_sequence_number(0);
          //   is_wireless = !is_wireless;
          //   commu_pkg->set_commu_type(is_wireless);
          //   send_three_times_msg(commu_pkg);
          // }
          break;
        }
      }
      int count = 0, i = 0;
      uint8_t tmp[1024];
      memset(tmp, 0, 1024);
      if (int(recbuf[0]) != 255)
      { // 上一次发送的包里含有这次报文的前半部分
        while (!buffer_t.empty())
        {
          tmp[count++] = buffer_t.front();
          buffer_t.pop();
        }
        while (i < len_recv && recbuf[i] != 238)
        {
          tmp[count++] = recbuf[i++];
        }
        shared_ptr<Package> package(new Package(count));
        package->parse_message(tmp, count);
        g_edge_recv_queue->add_package(package);
        ++i;
        count = 0;
      }
      while (i < len_recv && int(recbuf[i]) == 255)
      { // 将‘FF’与‘EE’之间的数据取到package里
        ++i;
        while (int(recbuf[i]) != 238)
        {
          if (i == len_recv)
          { // 最后剩余的只有‘FF’开头的数据放入队列里，以便与下次发来的数据结合
            for (int j = 0; j < count; ++j)
              buffer_t.push(tmp[j]);
            break;
          }
          tmp[count++] = recbuf[i++];
        }
        if (i == len_recv)
          break;
        shared_ptr<Package> package(new Package(count));
        package->parse_message(tmp, count);
#ifdef DEBUG
        printf("网口接收数据： ");
#endif
//         if (package->get_length() > 60)
//         {
// #ifdef DEBUG
//           cout << "error！" << endl;
//           printf("出错报文类型:%d，长度:%d\n", package->get_type(), package->get_length());
// #endif
//           logger->error("{}出错报文类型:{}，长度:{}", LOG_FILE_MSG, package->get_type(), package->get_length());
//           package = nullptr;
//         }
//         else
//         {
// #ifdef DEBUG
//           for (int i = 0; i < package->get_length(); i++)
//           {
//             printf("%x ", package->get_message()[i]);
//           }
//           printf(" read:%d", len_recv-2);
//           printf("\n");
// #endif
//           g_edge_recv_queue->add_package(package);
//         }
    if (package->get_type()==UP_DEVICE_INFO)
    {
      if(package->get_length()==25||package->get_length()==53||package->get_length()==59)
      {
        #ifdef DEBUG
          for (int i = 0; i < package->get_length(); i++)
          {
            printf("%x ", package->get_message()[i]);
          }
          printf(" read:%d", package->get_length());
          printf("\n");
        #endif
        g_edge_recv_queue->add_package(package);
      }
      else{
        #ifdef DEBUG
          cout << "error！" << endl;
          printf("出错报文类型:%d，长度:%d\n", package->get_type(), package->get_length());
        #endif
          logger->error("{}出错报文类型:{}，长度:{}", LOG_FILE_MSG, package->get_type(), package->get_length());
          package = nullptr;
      }
    }
    else
        {
        #ifdef DEBUG
          for (int i = 0; i < package->get_length(); i++)
          {
            printf("%x ", package->get_message()[i]);
          }
          printf(" read:%d", package->get_length());
          printf("\n");
        #endif
          g_edge_recv_queue->add_package(package);
        }
        ++i;
        count = 0;
      }
    }
    // 关闭连接套接字
    pthread_mutex_lock(&ethernet_mutex);
    close(ethernet_fd);
    ethernet_fd = -1;
    pthread_mutex_unlock(&ethernet_mutex);
  }
  pthread_mutex_destroy(&ethernet_mutex);
  // 关闭监听套接字
  close(listen_fd);
}

bool ping_ethernet()
{
  struct sigaction action;
  action.sa_handler = SingnalHandler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGINT, &action, NULL);
  Ping ping("169.254.254.231", 1);
  p = &ping;
  ping.CreateSocket();
  bool check_mode = ping.SendPacket() && ping.RecvPacket();
  if (check_mode)
    return true;
  else
    return false;
}

void *serial_port_receive_thread(void *arg)
{
#ifdef DEBUG
  pid_t pid = syscall(SYS_gettid);
  pthread_t tid = pthread_self();
  printf("serial_port_receive_thread: pid=%d, tid=%lu\n", pid, tid);
#endif
  auto logger = spdlog::get("project_log");
  uint8_t buffer[MAX_BUF_LEN] = {0};
  int len_package = 0, len_recv = 0;
  int flag = 0;
  while (true)
  {
    if (serial_port_fd == -1)
      continue;
    memset(buffer, 0, MAX_BUF_LEN);
    fd_set read_fds; // 创建文件描述符集合
    FD_ZERO(&read_fds);
    FD_SET(serial_port_fd, &read_fds);
    struct timeval timeout; // 设置超时时间
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int select_ret = select(serial_port_fd + 1, &read_fds, NULL, NULL, &timeout); // 使用select()轮询串口是否有数据可读
    if (select_ret == -1)
    {
      logger->error("{}select！", LOG_FILE_MSG);
      break;
    }
    else if (select_ret == 0)
    {
      // cout<<"select"<<endl;
    }
    else
    {
      if (FD_ISSET(serial_port_fd, &read_fds))
      {
        pthread_mutex_lock(&serial_port_mutex);
        len_recv = read(serial_port_fd, buffer, sizeof(buffer)); // 从串口读取数据
        pthread_mutex_unlock(&serial_port_mutex);
        if (len_recv < 0)
        {
          logger->error("{}Error reading datas", LOG_FILE_MSG);
        }
        else
        {
          int count = 0, i = 0;
          uint8_t tmp[1024];
          if (int(buffer[0]) != 255)
          { // 上一次发送的包里含有这次报文的前半部分
            while (!buffer_tmp.empty())
            {
              tmp[count++] = buffer_tmp.front();
              buffer_tmp.pop();
            }
            while (i < len_recv && buffer[i] != 238)
            {
              tmp[count++] = buffer[i++];
            }

            shared_ptr<Package> package(new Package(count));
            package->parse_message(tmp, count);
            g_edge_recv_queue->add_package(package);
            ++i;
            count = 0;
          }
          while (i < len_recv && int(buffer[i]) == 255)
          { // 将‘FF’与‘EE’之间的数据取到package里
            ++i;
            while (int(buffer[i]) != 238)
            {
              if (i == len_recv)
              { // 最后剩余的只有‘FF’开头的数据放入队列里，以便与下次发来的数据结合
                for (int j = 0; j < count; ++j)
                  buffer_tmp.push(tmp[j]);
                break;
              }
              tmp[count++] = buffer[i++];
            }
            if (i == len_recv)
              break;
            shared_ptr<Package> package(new Package(count));
            package->parse_message(tmp, count);
#ifdef DEBUG
            printf("串口接收数据： ");
#endif
//             if (package->get_length() > 60)
//             {
// #ifdef DEBUG
//               cout << "error！" << endl;
//               printf("出错报文类型:%d，长度:%d\n", package->get_type(), package->get_length());
// #endif
//               logger->error("{}出错报文类型:{}，长度:{}", LOG_FILE_MSG, package->get_type(), package->get_length());
//               package = nullptr;
//             }
//             else
//             {
// #ifdef DEBUG
//               for (int i = 0; i < package->get_length(); i++)
//               {
//                 printf("%x ", package->get_message()[i]);
//               }
//               printf(" read:%d", len_recv-2);
//               printf("\n");
// #endif
//               g_edge_recv_queue->add_package(package);
//             }
          if (package->get_type()==UP_DEVICE_INFO)
        {
            if(package->get_length()==25||package->get_length()==53||package->get_length()==59)
          {
            #ifdef DEBUG
              for (int i = 0; i < package->get_length(); i++)
            {
              printf("%x ", package->get_message()[i]);
            }
            printf(" read:%d", package->get_length());
            printf("\n");
            #endif
        g_edge_recv_queue->add_package(package);
      }
      else{
        #ifdef DEBUG
          cout << "error！" << endl;
          printf("出错报文类型:%d，长度:%d\n", package->get_type(), package->get_length());
        #endif
          logger->error("{}出错报文类型:{}，长度:{}", LOG_FILE_MSG, package->get_type(), package->get_length());
          package = nullptr;
      }
    }
    else
        {
          #ifdef DEBUG
              for (int i = 0; i < package->get_length(); i++)
            {
              printf("%x ", package->get_message()[i]);
            }
            printf(" read:%d", package->get_length());
            printf("\n");
          #endif
          g_edge_recv_queue->add_package(package);
        }
            ++i;
            count = 0;
          }
        }
      }
    }
  }
  pthread_mutex_lock(&serial_port_mutex);
  close(serial_port_fd);
  serial_port_fd = -1;
  pthread_mutex_unlock(&serial_port_mutex);
//  pthread_mutex_destroy(&serial_port_mutex);
  pthread_exit(NULL);
}
