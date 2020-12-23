#include "includes.h"

SOCKET_IPC_T ipc;
IPC_MSG_T msg;

void cbTimer(sigval_t i)
{
	//sendIpc(&ipc, "recv", msg);
	printf("run to here1\n");
}

int main(int argc, char const *argv[])
{
	TIMER_STRU_T tim;

	strcpy(msg.buf, "hello");
	creatIpc(&ipc, "send");
	
	// addTimer(&tim, 1, cbTimer);
	// startTimer(&tim);

	struct sigevent ev;
	timer_t id;

	ev.sigev_value.sival_ptr = &id;
	ev.sigev_notify = SIGEV_THREAD;
	ev.sigev_notify_function = cbTimer;
	if(timer_create(CLOCK_REALTIME, &ev, &id) < 0)
	{
		perror("timer create error!\n");
		return 0;
	}

	while (1)
	{
		// sendIpc(&ipc, "recv", msg);
		// sleep(2);
	}
	

	return 0;
}