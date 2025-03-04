#include <string>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <iostream>
using namespace std;
int serial_port_fd = -1; // 串口
struct termios config;
#define MAX_BUF_LEN 4096
#define LOG_FILE_MSG "[test test_serial.cpp]: "
void init()
{
    auto max_size = 1024 * 1024 * 5;
    auto max_files = 10;
    auto logger = spdlog::rotating_logger_mt("test_log", "./logs/testserial.log", max_size, max_files);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [thread %t] %v");

#ifdef DEBUG
    logger->set_level(spdlog::level::debug);
#else
    logger->set_level(spdlog::level::info);
#endif
}

void open()
{
    auto logger = spdlog::get("test_log");
    serial_port_fd = open("/dev/ttyTHS0", O_RDWR | O_NOCTTY); // 打开串口
    if (serial_port_fd == -1)
    {
        logger->error("{}Error opening serial port", LOG_FILE_MSG);
        // pthread_exit(NULL);
    }

    memset(&config, 0, sizeof(struct termios)); // 配置串口
    if (tcgetattr(serial_port_fd, &config) != 0)
    {
        logger->error("{}Error getting serial port attributes", LOG_FILE_MSG);
        close(serial_port_fd);
        // pthread_exit(NULL);
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
        // perror("Error setting serial port attributes");
        close(serial_port_fd);
        // pthread_exit(NULL);
    }
    return;
}
void delay(int seconds) //  参数必须为整型，表示延时多少秒
{
    clock_t start = clock();
    clock_t lay = (clock_t)seconds * CLOCKS_PER_SEC;
    while ((clock()-start) < lay);
 }
void closelogger()
{
    // Apply a function on all registered loggers
    spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info(": 结束运行"); });

    // Release and close all loggers
    spdlog::drop_all();
}
void *send_thread(void *arg)
{
    auto logger = spdlog::get("test_log");
    uint8_t buffer[MAX_BUF_LEN] = {0};
    int countsuccess = 0;
    int countfail = 0;
    int count = 0;
    while (true)
    {
        memset(buffer, 0, MAX_BUF_LEN);
        buffer[0] = 0xFF;
        uint8_t tmp[21] = {0x04, 0x15, 0x00, 0x00, 0x00, 0xe7, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x03, 0xf2, 0x00, 0x01, 0xe9, 0xbd};
        memcpy(buffer + 1, tmp, sizeof(tmp));
        buffer[sizeof(tmp) + 1] = 0xEE;
        delay(1);
        int result = write(serial_port_fd, buffer, sizeof(tmp) + 2);
        if(result > 0)
        {
             logger->info("{},累计{},success{}", LOG_FILE_MSG,count++,countsuccess++);
        }
        else{
            logger->error("{},累计{},fail{}", LOG_FILE_MSG,count++,countfail++);
        }
        cout << "串口write:" << result << endl;
        printf("串口发送数据： ");
        for (int i = 0; i < sizeof(tmp) + 2; i++)
        {
            printf("%x ", buffer[i]);
        }
        printf("\n");
    }
}
int main()
{
    init();
    open();
    pthread_t serial_thread;
    int result = pthread_create(&serial_thread, NULL, send_thread,NULL);
    if (result != 0) {
       cout<<"create fail"<<endl;
    }
    int result_serial = pthread_join(serial_thread, NULL);
    if (result_serial != 0) {
      cout<<"join fail"<<endl;
    }
    closelogger();
    return 0;
}