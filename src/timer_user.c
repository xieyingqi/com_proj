#include "timer_user.h"

/**
 * @brief 添加定时器
 * @param timer  定时器结构体
 * @param gap    周期
 * @param cbFunc 回调函数
 * @return 0:成功 -1:失败
 */
int addTimer(TIMER_STRU_T *timer, int gap, cb_timerFunc cbFunc)
{
	struct sigevent ev;

	timer->gap = gap;
	timer->cbFunc = cbFunc;

	ev.sigev_value.sival_ptr = &timer->id;
	ev.sigev_notify = SIGEV_THREAD;
	ev.sigev_notify_function = timer->cbFunc;
	if(timer_create(CLOCK_REALTIME, &ev, &timer->id) < 0)
	{
		perror("timer create error!\n");
		return -1;
	}

	return 0;
}

/**
 * @brief 启动定时器
 * @param timer 定时器结构体
 * @return 0:成功 -1:失败
 */
int startTimer(TIMER_STRU_T *timer)
{
	struct itimerspec ts;

	ts.it_interval.tv_sec = timer->gap;
	ts.it_interval.tv_nsec = 0;
	ts.it_value.tv_sec = 0;
	ts.it_value.tv_nsec = 0;

	if(timer_settime(timer->id, 0, &ts, NULL) < 0)
	{
		perror("timer start error!\n");
		return -1;
	}
	
	return 0;
}