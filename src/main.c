#include "includes.h"

UNIX_SOCKET_T gServer;
EP_LISTEN_T gEvent;

void cbSocketRead(int epfd, struct epoll_event ev)
{
	char buf[50];

	if (ev.events & EPOLLIN)
	{
		read(ev.data.fd, buf, sizeof(buf));
		printf("socket read:%s \n", buf);
	}
}

void cbSocketAdd(int epfd, struct epoll_event ev)
{
	int fd;
	UNIX_SOCKET_T client;

	//if (ev.data.fd == gServer.fd) //如果有客户端连接
	{
		printf("new client connected, fd:%d \n", ev.data.fd);
		client.len = sizeof(client.addr);
		fd = accept(gServer.fd, (struct sockaddr *)&client.addr, &(client.len));
		epollAddEvent(&gEvent, fd, cbSocketRead); //将连接的客户端添加至epoll监听
	}
}

int main(int argc, char const *argv[])
{
	/* 创建本地套接字服务端 */
	if (createUnixSocketServer(&gServer) < 0)
	{
		deleteUnixSocket(&gServer);
		return 0;
	}
	
	/* 创建epoll监听事件 */
	if (epollCreate(&gEvent) < 0)
	{
		epollDesrory(&gEvent);
		return 0;
	}

	epollAddEvent(&gEvent, gServer.fd, cbSocketAdd);
	epollListenLoop(&gEvent);

	deleteUnixSocket(&gServer);
	epollDesrory(&gEvent);

	return 0;
}