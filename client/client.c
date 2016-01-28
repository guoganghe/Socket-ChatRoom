#include <unistd.h>      //POSIX(fork、pipe、read..)
#include <stdio.h>
#include <stdlib.h>      //exit().....
#include <errno.h>
#include <string.h>
#include <sys/types.h>   //基本系统数据类型
#include <sys/socket.h>
#include <netinet/in.h>  //struct sockaddr_in, 某些结构体声明、宏定义。
#include <sys/epoll.h>

#define PORT 8888
#define LISTENQ 256
#define MAXSIZE 2048

/*普通客户端消息处理函数*/
void str_cli(int sockfd)
{
	/*发送和接收缓冲区*/
	char sendline[MAXSIZE] , recvline[MAXSIZE];
	int n;
	while(1)
	{
		printf("input some message to send:\n");
		scanf("%s", sendline);

		n = write(sockfd , sendline , strlen(sendline));
		printf("send:%s\n", sendline);

		n = read(sockfd, recvline, MAXSIZE);
		if(n == 0){
			printf("server terminated prematurely\n");
			break;
		}
		recvline[n] = '\0';
		printf("recv:%s\n", recvline);

	}//while
}

int main(int argc, char *argv[])
{
	char buff[MAXSIZE];
	int ret;
	int sockfd;
	struct sockaddr_in servaddr;

	printf("Plese input server's IP to connect:\n");
	scanf("%s", buff);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket error");
		exit(1);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	ret = inet_pton(AF_INET, buff, &servaddr.sin_addr);
	if(ret == -1){
		printf("The wrong IP:%s\n", buff);
		exit(1);
	}

	ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(ret == -1)
	{
		perror("connect error");
		exit(1);
	}

	/*调用消息处理函数*/
	str_cli(sockfd);

	exit(0);
}


