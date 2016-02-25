#include <unistd.h>      //POSIX(fork��pipe��read..)
#include <stdio.h>
#include <stdlib.h>      //exit().....
#include <errno.h>
#include <string.h>
#include <sys/types.h>   //����ϵͳ��������
#include <sys/socket.h>
#include <netinet/in.h>  //struct sockaddr_in, ĳЩ�ṹ���������궨�塣
#include <sys/epoll.h>
#include <fcntl.h>
#include <pthread.h>

#define PORT 8888
#define LISTENQ 256
#define MAXSIZE 2048

static void setNonblocking(int sockfd);
static int epoll_read(int fd, char *buff, int size);
static void epoll_write(int fd, char *buff);
static void do_epoll(int listenfd);
static void* handleThread(int *fd);


