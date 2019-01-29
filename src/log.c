//
// Created by Gijs Sijpesteijn on 20/12/2017.
// Until I figure out how to use syslog on apple, I use this.
//

#include <stdlib.h>
#include "log.h"
#include <syslog.h>
#include <stdarg.h>.


void info(const char *message, ...) {
#ifdef __APPLE__
    printf(*message, ...);
#else
    syslog (LOG_INFO, message, __va_list__);
#endif
}

void debug(const char *message, ...) {
#ifdef __APPLE__
    printf(*message, ...);
#else
    syslog (LOG_DEBUG, message);
#endif
}

void error(const char *message, ...) {
#ifdef __APPLE__
    printf(*message, ...);
#else
    syslog (LOG_ERR, message);
#endif
}