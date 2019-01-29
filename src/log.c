//
// Created by Gijs Sijpesteijn on 20/12/2017.
// Until I figure out how to use syslog on apple, I use this.
//

#include <stdlib.h>
#include "log.h"
#include <syslog.h>


void info(char *message) {
#ifdef __APPLE__
    cout << message << endl;
#else
    syslog (LOG_INFO, "%s", message.c_str());
#endif
}

void debug(char *message) {
#ifdef __APPLE__
    cout << message << endl;
#else
    syslog (LOG_DEBUG, "%s", message.c_str());
#endif
}

void error(char *message) {
#ifdef __APPLE__
    cout << message << endl;
#else
    syslog (LOG_ERR, "%s", message.c_str());
#endif
}