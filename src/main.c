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
	// IPC_MSG_T send_msg, recv_msg;
	// strcpy(send_msg.buf, "hello");
	// sendIpc(&gIpc, "recv", send_msg);

	// recvIpc(&gIpc, "send", &recv_msg, 0);
	printf("id=%s\n", cmdList.id);
	printf("type=%d\n", cmdList.type);
}

int main(int argc, char *argv[])
{
	analyCmdPara(argc, argv, cmdParaList, sizeof(cmdParaList) / sizeof(CMD_PARA_T));

	epollCreate(&gMainLoop);

	creatIpc(&gIpc, cmdParaList[0].value);
	
	addTimer(&gTimer, 1, cbTimer);
	startTimer(&gTimer);
	
	epollListenLoop(&gMainLoop);

	return 0;
}