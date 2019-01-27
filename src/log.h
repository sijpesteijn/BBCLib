//
// Created by Gijs Sijpesteijn on 20/12/2017.
//

#ifndef BBLASER_LOG_H
#define BBLASER_LOG_H

#include <string>

class log {
public:
    static void info(std::string message);
    static void debug(std::string message);
    static void error(std::string message);
};


#endif //BBLASER_LOG_H
