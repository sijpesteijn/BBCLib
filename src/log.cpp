//
// Created by Gijs Sijpesteijn on 20/12/2017.
// Until I figure out how to use syslog on apple, I use this.
//

#include "log.h"
#include <iostream>
#include <syslog.h>

using namespace std;

void log::info(string message) {
#ifdef __APPLE__
    cout << message << endl;
#else
    syslog (LOG_INFO, "%s", message.c_str());
#endif
}

void log::debug(string message) {
#ifdef __APPLE__
    cout << message << endl;
#else
    syslog (LOG_DEBUG, "%s", message.c_str());
#endif
}

void log::error(string message) {
#ifdef __APPLE__
    cout << message << endl;
#else
    syslog (LOG_ERR, "%s", message.c_str());
#endif
}