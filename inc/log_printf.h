#ifndef _LOG_PRINTF_H_
#define _LOG_PRINTF_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <syslog.h>

extern void setLogLevel(const char *log_level);
extern void logPrintf(const char *format, ...);

#endif