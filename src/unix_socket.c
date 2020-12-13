#include "unix_socket.h"

/**
 * @brief 创建本地套接字(服务端)
 * @param server 套接字结构体
 * @return 0:成功 -1:失败
 */
int createUnixSocketServer(UNIX_SOCKET_T *server)
{
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

	return 0;
}

/**
 * @brief 创建本地套接字(客户端)
 * @param client 套接字结构体
 * @return 0:成功 -1:失败
 */
int createUnixSocketClient(UNIX_SOCKET_T *client)
{
	/* 创建本地套接字 */
	client->fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (client->fd < 0)
	{
		perror("socket error!");
		return -1;
	}

	/* 结构体初始化 */
	bzero(&client->addr, sizeof(client->addr));
	client->addr.sun_family = AF_UNIX;
	strcpy(client->addr.sun_path, SOCKET_PATH);

	/* 连接服务端 */
	if (connect(client->fd, (struct sockaddr*)&client->addr, sizeof(client->addr)) < 0)
	{
		perror("connect error!");
		return -1;
	}

	return 0;
}

/**
 * @brief 删除本地套接字
 * @param socket_t 套接字结构体
 */
void deleteUnixSocket(UNIX_SOCKET_T *socket_t)
{
	close(socket_t->fd);
}