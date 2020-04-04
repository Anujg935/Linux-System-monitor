#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() {
    Processor cpu_;
    return cpu_;
}

// TODO: Return a container composed of the system's processes
bool compare(Process a ,Process b){
    float i = a.CpuUtilization();
    float j = b.CpuUtilization();
    return i > j;
}
vector<Process>& System::Processes() {
    vector<int> pid = LinuxParser::Pids();
    
    for(int i=0;i<pid.size();i++){
        Process p(pid[i]);
        processes_.push_back(p);
    }
    std::sort(processes_.begin(),processes_.end(),compare);
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }