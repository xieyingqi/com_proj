#ifndef _SOCKET_H_
#define _SOCKET_H_

#define SOCKET_PATH "/tmp/socket.unix"
#define SOCKET_MAX_CON 10

typedef struct
{
	int fd;
	socklen_t len;
	struct sockaddr_un addr;
} UNIX_SOCKET_T;

extern int createUnixSocketServer(UNIX_SOCKET_T *server, UNIX_SOCKET_T *client);
extern int createUnixSocketClient(UNIX_SOCKET_T *socket_t);

#endif