#include "Time.h"

unsigned int Time::convertTimeStringToUInt(const std::string &time_string) {

    std::tm time_struct = {};
    std::istringstream iss(time_string);
    std::size_t dot_pos;
    unsigned int milliseconds = 0;
    time_t timeValue;
    unsigned int result;

    iss >> std::get_time(&time_struct, "%Y-%m-%d %H:%M:%S");
    if (iss.fail()) {
        // Handle parsing error
        throw std::runtime_error("Failed to parse time string");
    }
    // Extract milliseconds from the time string
    dot_pos = time_string.find('.');
    if (dot_pos != std::string::npos) {
        milliseconds = std::stoi(time_string.substr(dot_pos + 1));
    }
    // Convert std::tm to time_t and add milliseconds
    timeValue = std::mktime(&time_struct);
    result = static_cast<unsigned int>(timeValue) * 1000 + milliseconds;

    return result;
}