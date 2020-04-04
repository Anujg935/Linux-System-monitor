#include <string>
#include<iomanip>
#include<sstream>
#include "format.h"

using std::string;
using std::to_string;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    std::ostringstream formattime;
    int h = seconds/3600;
    seconds = seconds %3600;
    int  m = seconds/60;
    int s = seconds % 60;
    
    formattime << std::setfill('0') << std::setw(2) << h  << ":"  
    << std::setfill('0') << std::setw(2) << m << ":" 
    << std::setfill('0') << std::setw(2) << s;

    return formattime.str();
}