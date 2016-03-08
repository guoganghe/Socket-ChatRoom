#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <unistd.h>      //POSIX(fork、pipe、read..)
#include <stdio.h>
#include <stdlib.h>      //exit().....
#include <errno.h>
#include <string.h>
#include <sys/types.h>   //基本系统数据类型
#include <sys/socket.h>
#include <netinet/in.h>  //struct sockaddr_in, 某些结构体声明、宏定义。
#include <sys/epoll.h>

#endif
