#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
 
#define MAXLINE 4096
#define PORT 8088
 
int main(void){
	//定义客户端套接字
	int sockfd = -1;
	//定义想连接的服务器的套接字地址
	struct sockaddr_in servaddr;
 
	//发送和接收数据的缓冲区
	uint8_t sendbuf[MAXLINE], recbuf[MAXLINE];
 
	//初始化服务器套接字地址
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;//IPv4
	servaddr.sin_port = htons(PORT);//想连接的服务器的端口
	servaddr.sin_addr.s_addr = inet_addr("169.254.254.232");//服务器的IP地址
 
	//创建套接字
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		printf("create socket error: %s(error: %d)\n", strerror(errno), errno);
		exit(0);
	}
 
	//向服务器发送连接请求
  while(1)
  {
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		//连接失败
		printf("connect socket error: %s(error: %d)\n", strerror(errno), errno);
		continue;
	}
  printf("123\n");
	while(1){
	//	向服务器发送信息
		printf("向服务器发送信息：");
    uint8_t tmp[23] = {0xff,0x04,0x15,0x00,0x00,0x00,0xe7,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x03,0xf2,0x00,0x01,0xe9,0xbd,0xee};
    memcpy(sendbuf,tmp,23);
    for(int i=0;i<15;i++)
		write(sockfd, sendbuf, 23);
    printf("从服务器接收信息：");
		//从服务器接收信息
		ssize_t len = read(sockfd, recbuf, sizeof(recbuf));
		if(len < 0){
			if(errno == EINTR){
				continue;
			}
			exit(0);
		}
    else{
      for (int i = 0; i < len; i++)
          {
            printf("%x ", recbuf[i]);
          }
          printf("\n");  

    }
 
		//printf("服务器回应：%s\n", recbuf);
	}

  }
	
	//关闭套接字
	close(sockfd);
 
}