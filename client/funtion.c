#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "config.h"

void user_login(int sockfd, int Msg_Type)
{
	char username[256];
	char password[256];
	USER usermsg;
	char msgbuff[MAXSIZE];
	int ret;
	unsigned long timeout;
	
	while(1)
	{
		printf("用户名:");
		setbuf(stdin,NULL);/*没有换行符,刷新缓冲机制才能打印出来*/
		scanf("%s", username);
		if(strlen(username) > 20)
		{
			printf("用户名错误:长度超过20字符\n");
			goto NEXTACT;
		}
		else
		{
			printf("密码:");
			setbuf(stdin,NULL);/*没有换行符,刷新缓冲机制才能打印出来*/
			scanf("%s", password);
			if(strlen(username) > 20){
				printf("密码错误:长度超过20字符\n");
				goto NEXTACT;
			}
			else
			{
				usermsg.MsgType = Msg_Type;
				strcpy(usermsg.From_UseName, username);
				strcpy(usermsg.password, password);
				memcpy(msgbuff, &usermsg, sizeof(usermsg));
				
				timeout = 0;
				while( timeout < 100000 && send(sockfd, msgbuff, sizeof(msgbuff), 0) == -1 )
				{
					timeout++;
				}
				if( timeout >= 100000){
					printf("登陆服务器超时\n");
					goto NEXTACT;
				}
				
				timeout = 0;
				while( timeout < 100000 && recv(sockfd, msgbuff, sizeof(msgbuff), 0) == -1 )
				{
					timeout++;
				}
				if( timeout >= 100000){
					printf("服务器响应超时\n");
					goto NEXTACT;
				}
				
				memcpy(&usermsg, msgbuff, sizeof(msgbuff));
				if(usermsg.result == 0){
					printf("登陆失败:%s\n", usermsg.MsgBuff);
					goto NEXTACT;
				}
				break ;
			}
		}
		
NEXTACT:
		printf("选择操作:\n");
		printf("1:继续登陆\n");
		printf("2:退出\n");
		scanf("%d", &Msg_Type);
		if(Msg_Type == 1){
			continue;
		}
		else if(Msg_Type == 2){
			return ;
		}else{
			printf("无效操作,请重新选择操作\n");
			goto NEXTACT;
		}
	}
	
	/*登陆后......*/
	
}

void user_register(int sockfd, int Msg_Type)
{
	char username[256];
	char password[256];
	USER usermsg;
	char msgbuff[MAXSIZE];
	int ret;
	unsigned long timeout;
	
	while(1)
	{
		printf("用户名: ");
		setbuf(stdin,NULL);/*没有换行符,刷新缓冲机制才能打印出来*/
		scanf("%s", username);
		if(strlen(username) > 20)
		{
			printf("用户名错误:长度超过20字符\n");
			goto NEXTACT;
		}
		else
		{
			printf("密码: ");
			setbuf(stdin,NULL);/*没有换行符,刷新缓冲机制才能打印出来*/
			scanf("%s", password);
			if(strlen(username) > 20){
				printf("密码错误:长度超过20字符\n");
				goto NEXTACT;
			}
			else
			{
				usermsg.MsgType = Msg_Type;
				strcpy(usermsg.From_UseName, username);
				strcpy(usermsg.password, password);
				memcpy(msgbuff, &usermsg, sizeof(usermsg));
				
				timeout = 0;
				while( timeout < 10000 && send(sockfd, msgbuff, sizeof(msgbuff), 0) == -1 )
				{
					timeout++;
				}
				if( timeout >= 10000){
					printf("服务器超时\n");
					goto NEXTACT;
				}
				
				timeout = 0;
				while( timeout < 10000 && recv(sockfd, msgbuff, sizeof(msgbuff), 0) == -1 )
				{
					timeout++;
				}
				if( timeout >= 10000){
					printf("服务器响应超时\n");
					goto NEXTACT;
				}
				
				memcpy(&usermsg, msgbuff, sizeof(msgbuff));
				if(usermsg.result == 0){
					printf("注册失败:%s\n", usermsg.MsgBuff);
					goto NEXTACT;
				}
				printf("注册成功,回车键返回\n");
				scanf("%s", password);
				break;
			}
		}
		
NEXTACT:
		printf("选择操作:\n");
		printf("1:继续注册\n");
		printf("2:退出\n");
		scanf("%d", &Msg_Type);
		if(Msg_Type == 1){
			continue;
		}
		else if(Msg_Type == 2){
			return ;
		}else{
			printf("无效操作,请重新选择操作\n");
			goto NEXTACT;
		}
	}
}

