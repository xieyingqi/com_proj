#include "unix_socket.h"

/**
 * @brief 创建本地套接字
 * @param sock 套接字结构体
 * @return 0:成功 -1:失败
 */
static int createUnixSocket(UNIX_SOCKET_T *sock, const char* path)
{
	/* 创建本地套接字 */
	sock->fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock->fd < 0)
	{
		perror("socket error!");
		return -1;
	}

	/* 结构体初始化 */
	bzero(&sock->addr, sizeof(sock->addr));
	sock->addr.sun_family = AF_UNIX;
	strcpy(sock->addr.sun_path, path);
	unlink(path);

	/* 绑定套接字 */
	sock->len = sizeof(sock->addr);
	if (bind(sock->fd, (struct sockaddr*)&(sock->addr), sock->len) < 0)
	{
		perror("bind error!\n");
		close(sock->fd);
		return -1;
	}

	return 0;
}

/**
 * @brief 删除本地套接字
 * @param sock 套接字结构体
 */
static void deleteUnixSocket(UNIX_SOCKET_T *sock)
{
	close(sock->fd);
}

/**
 * @brief 创建本地套接字监听(IPC通讯)
 * @param sock 套接字结构体
 * @param id 程序通讯id
 */
int creatIpc(SOCKET_IPC_T *ipc, const char* id)
{
	char path[50];

	sprintf(path, SOCKET_PATH, id);

	if (createUnixSocket(&ipc->sock, path) < 0)
	{
		deleteUnixSocket(&ipc->sock);
		return -1;
	}

	/* 创建epoll监听事件 */
	if (epollCreate(&ipc->ep) < 0)
	{
		epollDesrory(&ipc->ep);
		return -1;
	}

	epollAddEvent(&ipc->ep, ipc->sock.fd, NULL);

	return 0;
}

/**
 * @brief 本地套接字发送
 * @param ipc 结构体
 * @param id 目标id
 * @param data 要发送的数据
 */
int sendIpc(SOCKET_IPC_T *ipc, const char* id, IPC_MSG_T data)
{
	UNIX_SOCKET_T sock;
	char path[50];

	sprintf(path, SOCKET_PATH, id);
	bzero(&sock.addr, sizeof(sock.addr));
	sock.addr.sun_family = AF_UNIX;
	strcpy(sock.addr.sun_path, path);
	sock.len = sizeof(sock.addr);

	sendto(ipc->sock.fd, data.buf, sizeof(data.buf), 0, (struct sockaddr *)&sock.addr, sock.len);
	printf("ipc send to %s: %s\n", id, data.buf);

	return 0;
}

/**
 * @brief 本地套接字接收
 * @param ipc 结构体
 * @param id 目标id
 * @param data 接收到的数据指针
 * @param timeOutMs 超时时间
 */
int recvIpc(SOCKET_IPC_T *ipc, const char* id, IPC_MSG_T *data, int timeOutMs)
{
	int fds, i, j;
	size_t rd = 0;
	UNIX_SOCKET_T sock;
	struct epoll_event listenEvents[SOCKET_MAX] = {0};

	fds = epoll_wait(ipc->ep.epfd, listenEvents, SOCKET_MAX, timeOutMs);
	for (i = 0; i < fds; i++)
	{
		for (j = 0; j < SOCKET_MAX; j++)
		{
			if (listenEvents[i].data.fd == ipc->ep.event[j].fd)
			{
				sock.len = sizeof(sock.addr);
				rd = recvfrom(ipc->ep.event[j].fd, data->buf, sizeof(data->buf), 0, (struct sockaddr *)&sock.addr, &sock.len);
				printf("ipc recv from %s: %s\n", id, data->buf);
				break;
			}
		}
	}

	return rd;
}