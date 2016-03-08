#include "config.h"
#include "client.h"
#include "funtion.h"
#include "interface.h"

static void Chat_funtion(int sockfd);

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

	Chat_funtion(sockfd);

	exit(0);
}

static void Chat_funtion(int sockfd)
{
	/*发送和接收缓冲区*/
	char msgbuff[MAXSIZE];
	int Msg_Type;
	char username[21];
	int nsize,ret;
	USER usermsg;
	
	while(1)
	{
		mainInterface();
		
		scanf("%d", &Msg_Type);
		switch(Msg_Type)
		{
			case 1:
				/*登陆*/
				user_login(sockfd, Msg_Type);
				break;
			case 2:
				/*注册*/
				user_register(sockfd, Msg_Type);
				break;
			case 4:
				/*退出*/
				return;
			default:
				printf("无该项操作\n");
				break;
		}
	}
}
