#ifndef MOCKMARKET_TIME_H
#define MOCKMARKET_TIME_H

#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

class Time {
public:
    static unsigned int convertTimeStringToUInt(const std::string &time_string);
};

#endif //MOCKMARKET_TIME_H
