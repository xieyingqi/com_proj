#include "includes.h"

/* 创建本地套接字(服务端) */
int createUnixSocketServer(UNIX_SOCKET_T *server, UNIX_SOCKET_T *client)
{
	int sock_fd;

	/* 创建本地套接字 */
	server->fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (server->fd < 0)
	{
		perror("socket error!");
		return -1;
	}

	/* 结构体初始化 */
	bzero(&server->addr, sizeof(server->addr));
	server->addr.sun_family = AF_UNIX;
	strcpy(server->addr.sun_path, SOCKET_PATH);
	unlink(SOCKET_PATH);

	/* 绑定套接字 */
	server->len = sizeof(server->addr);
	if (bind(server->fd, (struct sockaddr*)&(server->addr), server->len) < 0)
	{
		perror("bind error!\n");
		return -1;
	}

	/* 创建监听，第二个参数为允许连接最大客户端数目 */
	if (listen(server->fd, SOCKET_MAX_CON) < 0)
	{
		perror("listen error!");
		return -1;
	}

	printf("wait client to connect...\n");

	/* 等待客户端连接 */
	client->len = sizeof(client->addr);
	sock_fd = accept(server->fd, (struct sockaddr*)&client->addr, &(client->len));

	return sock_fd;
}

/* 创建本地套接字(客户端) */
int createUnixSocketClient(UNIX_SOCKET_T *socket_t)
{
	/* 创建本地套接字 */
	socket_t->fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socket_t->fd < 0)
	{
		perror("socket error!");
		return -1;
	}

	/* 结构体初始化 */
	bzero(&socket_t->addr, sizeof(socket_t->addr));
	socket_t->addr.sun_family = AF_UNIX;
	strcpy(socket_t->addr.sun_path, SOCKET_PATH);

	/* 连接服务端 */
	if (connect(socket_t->fd, (struct sockaddr*)&socket_t->addr, sizeof(socket_t->addr)) < 0)
	{
		perror("connect error!");
		return -1;
	}

	return 0;
}