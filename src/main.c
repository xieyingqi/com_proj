#include "includes.h"

UNIX_SOCKET_T gServer, gClient;
int gAcceptFd;

int main(int argc, char const *argv[])
{
	char buf[50];
	// int epollFd;
	// struct epoll_event ep_event;

	gAcceptFd = createUnixSocketServer(&gServer, &gClient);
	if (gAcceptFd < 0)
	{
		perror("create unix socket failed!");
		close(gAcceptFd);
		close(gServer.fd);
		close(gClient.fd);
		return 0;
	}
	
	// epollFd = epoll_create1(0);
	// ep_event.data.fd = gAcceptFd;
	// ep_event.events = EPOLLIN;
	// epoll_ctl(epollFd, EPOLL_CTL_ADD, gAcceptFd, &ep_event);
	// epoll_wait(epollFd, &ep_event, SOCKET_MAX_CON, 500);

	read(gAcceptFd, buf, sizeof(buf));
	printf("receive: %s \n", buf);

	close(gAcceptFd);
	return 0;
}
