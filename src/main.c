#include "includes.h"

EP_LISTEN_T gMainLoop;
SOCKET_IPC_T gIpc;
TIMER_STRU_T gTimer;
CMD_LIST_T cmdList;

CMD_PARA_T cmdParaList[] = 
{
	{"id",   TYPE_TEXT, cmdList.id},
	{"type", TYPE_INT,  &cmdList.type}
};

void cbTimer(sigval_t i)
{
	IPC_MSG_T send_msg;
	
	strcpy(send_msg.buf, "hello");

	if(strcmp(cmdList.id, "recv") != 0)
	{
		sendIpc(&gIpc, "recv", send_msg);
	}

	//IPC_MSG_T recv_msg;
	//recvIpcSync(&gIpc, cmdList.id, &recv_msg, 0);
}

void cbSock(IPC_MSG_T *msg)
{
	printf("rcv %d data from:%s msg:%s\n", msg->cnt, msg->id, msg->buf);
}

int main(int argc, char *argv[])
{
	parseCmdPara(argc, argv, cmdParaList, sizeof(cmdParaList) / sizeof(CMD_PARA_T));

	epollCreate(&gMainLoop);

	creatIpc(&gMainLoop, &gIpc, cmdList.id);
	
	setIpcCallBack(&gIpc, cbSock);
	
	addTimer(&gTimer, 1, cbTimer);
	
	startTimer(&gTimer);
	
	epollListenLoop(&gMainLoop);

	return 0;
}