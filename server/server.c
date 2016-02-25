#include "server.h"

int main(int argc, char *argv[])
{
	int listenfd;
	struct sockaddr_in server_addr;
	int opt = 1;  //套接字选项
	int result;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1){
		perror("socket error");
		exit(1);
	}
	setNonblocking(listenfd);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //接受任意IP
	server_addr.sin_port = htons(PORT);

	//设置socket状态
	//SOL_SOCKET:存取socket层, SO_REUSEADDR:允许在bind()过程中本地址可重复使用
	//setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//绑定套接字
	result = bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(result == -1){
		perror("socket bind error");
		exit(1);
	}

	result = listen(listenfd, LISTENQ);
	if(result == -1){
		perror("socket listen error");
		exit(1);
	}

	do_epoll(listenfd);

	return 0;
}

static void do_epoll(int listenfd)
{
	int epollfd, sockfd;
	//ev用于注册事件,数组用于存储要处理的事件
	struct epoll_event ev, events[LISTENQ];
	struct sockaddr_in clien_addr;
	int fd_numb;
	int i, rw_size;
	char buf[MAXSIZE];
	int clilen;
	pthread_t pid;

	//创建一个描述符
	epollfd = epoll_create(LISTENQ);
	//添加监听描述符事件
	//EPOLLIN:表示对应的文件描述符可读; EPOLLIN:设置ET工作模式
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = listenfd;
	//EPOLL_CTL_ADD:注册listenfd到epollfd中
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

	while(1)
	{
		//获取已经准备好的描述符事件
		//第3个参数不能大于epoll_create的参数, -1:超时时间, -1表示永久阻塞
		fd_numb = epoll_wait(epollfd, events, LISTENQ, -1);
		//printf("收到连接请求数:%d\n", fd_numb);

		//处理事件
		for(i=0; i<fd_numb; i++)
		{
			//检测到用户连接
			if(events[i].data.fd == listenfd){
				clilen = sizeof(clien_addr);
				sockfd = accept(listenfd, (struct sockaddr *)&clien_addr, &clilen );
				if(sockfd == -1){
					perror("accept error");
					exit(1);
				}
				printf("accept a new client: %s:%d\n", inet_ntoa(clien_addr.sin_addr), ntohs(clien_addr.sin_port)/* clien_addr.sin_port*/ );

				setNonblocking(sockfd);
				//继续添加事件
				ev.data.fd = sockfd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
			}
			/*
			else if(events[i].events & EPOLLIN || events[i].events & EPOLLOUT){
				printf("创建了处理线程");
				pthread_create(&pid , NULL, (void *)handleThread, (void *)&sockfd);
			}*/
			
			else if(events[i].events & EPOLLIN){
				pthread_create(&pid , NULL, (void *)handleThread, (void *)&sockfd);
			}
			/*
			else if(events[i].events & EPOLLIN){
				rw_size = epoll_read(sockfd, buf, MAXSIZE);
				//事件读改为写, EPOLL_CTL_MOD:修改已经注册的监听事件
				ev.data.fd = sockfd;
				handleThread,ev.events = EPOLLOUT | EPOLLET;
				epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
			}
			else if(events[i].events & EPOLLOUT){
				//rw_size = write(sockfd, buf, rw_size);
				epoll_write(sockfd, buf);

				//事件写改读
				ev.data.fd = events[i].data.fd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollfd, EPOLL_CTL_MOD, events[i].data.fd, &ev);
			}*/
		}

	}
	close(epollfd);
	free(events);

}

static void* handleThread(int *fd)
{
	int sockfd;
	char buff[MAXSIZE];
	int rwsize;
	
	sockfd = *fd;
	rwsize = epoll_read(sockfd, buff, MAXSIZE);
	printf("receve:%s\n", buff);
	epoll_write(sockfd, buff);
	
	return (void*)0;
}

static void setNonblocking(int sockfd)
{
	int opts;
	opts=fcntl(sockfd,F_GETFL);
	if(opts<0)
	{
		perror("fcntl(sock,GETFL)");
		return;
	}//if

	opts = opts|O_NONBLOCK;
	if(fcntl(sockfd,F_SETFL,opts)<0)
	{
		perror("fcntl(sock,SETFL,opts)");
		return;
	}//if
}

//epoll的ET(边沿出发)模式下，正确的读写方式为：
//读：只要可读，就一直读，直到返回0,或者 errno = EAGAIN  (如果不把剩下的数据读完,socket将永远不可读)
//写：只要可写，就一直写，直到数据发送完，或者 errno = EAGAIN  (如果不把剩下的数据写完,socket将永远不可写)
static int epoll_read(int fd, char *buff, int size)
{
	int nread, readsize;

	readsize = 0;
	while( (nread = read(fd, buff + readsize, size-1)) > 0)
	{
		readsize += nread;
	}
	if(nread == -1 && errno != EAGAIN){
		perror("read error");
	}
	buff[readsize] = '\0';
	return readsize;
}

//分包写还未理解完善
static void epoll_write(int fd, char *buff)
{
	int datasize, nwrite;

	datasize = strlen(buff);
	nwrite = 0;
	while(datasize > 0)
	{
		nwrite = write(fd, buff+nwrite, datasize);
		if(nwrite == -1 && errno != EAGAIN){
			perror("write error");
			break;
		}
		else if(errno == EAGAIN){
			break;
		}
		datasize -= nwrite;
	}
}




