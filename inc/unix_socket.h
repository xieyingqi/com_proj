#ifndef _UNIX_SOCKET_H_
#define _UNIX_SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "epoll_user.h"

#define SOCKET_PATH "/tmp/%s.unix"
#define SOCKET_MAX 10

typedef struct
{
	int fd;
	socklen_t len;
	struct sockaddr_un addr;
} UNIX_SOCKET_T;

typedef struct
{
	UNIX_SOCKET_T sock;
	EP_LISTEN_T ep;
} SOCKET_IPC_T;

typedef struct
{
	char buf[1024];
} IPC_MSG_T;

extern int creatIpc(SOCKET_IPC_T *ipc, const char* id);
extern int sendIpc(SOCKET_IPC_T *ipc, const char* id, IPC_MSG_T data);
extern int recvIpc(SOCKET_IPC_T *ipc, const char* id, IPC_MSG_T *data, int timeOutMs);

#endif