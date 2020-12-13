#ifndef _UNIX_SOCKET_H_
#define _UNIX_SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/socket.unix"
#define SOCKET_MAX_CON 10

typedef struct
{
	int fd;
	socklen_t len;
	struct sockaddr_un addr;
} UNIX_SOCKET_T;

extern int createUnixSocketServer(UNIX_SOCKET_T *server);
extern int createUnixSocketClient(UNIX_SOCKET_T *client);
extern void deleteUnixSocket(UNIX_SOCKET_T *socket_t);

#endif