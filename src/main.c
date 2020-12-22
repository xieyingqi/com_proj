#include "includes.h"

SOCKET_IPC_T ipc;

int main(int argc, char const *argv[])
{
	IPC_MSG_T msg;

	strcpy(msg.buf, "hello");

	creatIpc(&ipc, "send");

	while (1)
	{
		sendIpc(&ipc, "recv", msg);
		sleep(2);
	}
	

	return 0;
}