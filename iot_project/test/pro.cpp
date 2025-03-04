#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>




int main(int argc, const char *argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "Usage : %s <ip> <prot>.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//1.创建套接字
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1){
		perror("[ERROR] Fail to socket.");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in svr_addr;
	bzero(&svr_addr, sizeof(struct sockaddr_in)); //清零
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(atoi(argv[2]));
	svr_addr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret = connect(sfd, (const struct sockaddr *)&svr_addr, sizeof(struct sockaddr_in));
	if(ret == -1){
		perror("[ERROR] Fail to connect.");
		exit(EXIT_FAILURE);
	}
	
	//2.获取输入并发送
	ssize_t sbytes = 0;
	char buffer[1024] = {0};
	strcpy(buffer, "Hello, server");
	sbytes = send(sfd, buffer, strlen(buffer)+1, 0);
	if(sbytes == -1){
		perror("[ERROR] Fail to send.");
		exit(EXIT_FAILURE);
	}

	bzero(buffer, sizeof(buffer));

	//3.接收数据并显示
	ssize_t rbytes = 0;
	char buffer_recv[1024] = {0};
	rbytes = recv(sfd, buffer_recv, sizeof(buffer_recv), 0);
	if(ret == -1){
		perror("[ERROR] Fail to ercv.");
		exit(EXIT_FAILURE);
	}else if(rbytes > 0){
		printf("buffer : %s\n", buffer_recv);
	}else if(rbytes == 0){
		printf("server has been shut down.\n");
	}

	close(sfd);
    
	return 0;
}
