#ifndef __SERVER_H__
#define __SERVER_H__

#include <unistd.h>      //POSIX(fork、pipe、read..)
#include <stdio.h>
#include <stdlib.h>      //exit().....
#include <errno.h>
#include <string.h>
#include <sys/types.h>   //基本系统数据类型
#include <sys/socket.h>
#include <netinet/in.h>  //struct sockaddr_in, 某些结构体声明、宏定义。
#include <sys/epoll.h>
#include <fcntl.h>
#include <pthread.h>

static void setNonblocking(int sockfd);
static int epoll_read(int fd, char *buff, int size);
static void epoll_write(int fd, char *buff);
static void do_epoll(int listenfd);
static void* handleThread(int *fd);

#endif
