#ifndef _TIMER_USER_H_
#define _TIMER_USER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>

typedef void (*cb_timerFunc) (sigval_t);

typedef struct
{
	timer_t id;
	int gap;
	cb_timerFunc cbFunc;
} TIMER_STRU_T;

extern int addTimer(TIMER_STRU_T *timer, int gap, cb_timerFunc cbFunc);
extern int startTimer(TIMER_STRU_T *timer);

#endif