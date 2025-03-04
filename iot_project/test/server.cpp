#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "variables.h"
#include <arpa/inet.h>
#include <time.h>
#define PORT 8288
queue<uint8_t> buffer_tmp;

void delay(int seconds) //  参数必须为整型，表示延时多少秒
{
    clock_t start = clock();
    clock_t lay = (clock_t)seconds * CLOCKS_PER_SEC;
    while ((clock()-start) < lay);
 }
void *sendmsg(void *arg)
{
  while(1)
  {
    delay(10);
    uint8_t tmp[21] = {0x04,0x15,0x00,0x00,0x00,0xe7,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x03,0xf2,0x00,0x01,0xe9,0xbd};
    shared_ptr<Package> package(new Package(21));
    memcpy(package->get_message(),tmp,21);
    g_com_send_queue->add_package(package);

  }

}
void *ethernet_send_recv_thread(void *arg){
	//定义服务器监听套接字和连接套接字
	int listen_fd = -1, connect_fd = -1;//初始化为-1
	struct sockaddr_in servaddr;//定义服务器对应的套接字地址
	//服务器接收和发送缓冲区
	uint8_t sendbuf[MAX_BUF_LEN], recbuf[MAX_BUF_LEN];
 
	//初始化套接字地址结构体
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;//IPv4
	servaddr.sin_port = htons(PORT);//设置监听端口
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//表示接收任意IP的连接请求
  inet_aton("169.254.254.232",&servaddr.sin_addr);
 
	//创建套接字
	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		//如果创建套接字失败，返回错误信息
		//strerror(int errnum)获取错误的描述字符串
		printf("create socket error: %s(error: %d)\n", strerror(errno), errno);
		exit(0);
	}
  int opt = 1;
  setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof( opt ));
	//绑定套接字和本地IP地址和端口
	if(bind(listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		//绑定出现错误
		printf("bind socket error: %s(error: %d)\n", strerror(errno), errno);
		exit(0);
	}
	//使得listen_fd变成监听描述符
	if(listen(listen_fd, 10) == -1){
		printf("listen socket error: %s(error: %d)\n", strerror(errno), errno);
		exit(0);
	}
	//accept阻塞等待客户端请求
	printf("等待客户端发起连接\n");
  int len_recv = 0;
	while(1){
		if((connect_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL)) == -1){ //阻塞等待客户端连接
			printf("accept socket error: %s(error: %d)\n", strerror(errno), errno);
			continue;
		}
 
		//可以一直保持连接
		while(1){
      memset(sendbuf,0,MAX_BUF_LEN);
      memset(recbuf,0,MAX_BUF_LEN);
 
        if(!g_com_send_queue->is_empty()){
            shared_ptr<Package> package=g_com_send_queue->get_package();
            uint32_t length = package->get_length();
            sendbuf[0] = 0xFF;
            memcpy(sendbuf+1, package->get_message(), length);
            sendbuf[length+1] = 0xEE;
            write(connect_fd, sendbuf, length+2);
       // #ifdef DEBUG
            printf("发送数据： ");
            for(int i=0;i<length+2;i++){
                        printf("%x ",sendbuf[i]);
                    }
                    printf("\n");
      //  #endif
        }
        else{
        len_recv =  read(connect_fd, recbuf, sizeof(recbuf));
        if(len_recv < 0){
        if(errno == EINTR){
          continue;
        }
      }
      printf("接收数据：");
      for(int i=0;i<len_recv;i++)
      {
        printf("%x ",recbuf[i]);
      }
      printf("\n");
  
 }
		}
 
		//关闭连接套接字
		close(connect_fd);
	} 
	//关闭监听套接字
	close(listen_fd);
}

int main(){
  initialize_global_variable();
  pthread_t my_thread;
  pthread_t my_thread2;
  int result = pthread_create(&my_thread, NULL, ethernet_send_recv_thread,NULL);
  // int result2 = pthread_create(&my_thread2, NULL, sendmsg,NULL);
  if (result != 0) {
        perror("Thread creation failed");
        return 1;
    }
    //  if (result2 != 0) {
    //     perror("Thread creation failed");
    //     return 1;
    // }
    result = pthread_join(my_thread, NULL);
    if (result != 0) {
        perror("Thread join failed");
        return 1;
    }
    // result = pthread_join(my_thread2, NULL);
    // if (result != 0) {
    //     perror("Thread join failed");
    //     return 1;
    // }

	return 0;
}