#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << (seconds / 3600) << ":" /* Hours */
        << std::setfill('0') << std::setw(2) << (seconds % 3600) / 60  << ":" /* Minutes */
        << std::setfill('0') << std::setw(2) << (seconds % 3600) % 60;  /* Seconds */
    return ss.str(); 
}