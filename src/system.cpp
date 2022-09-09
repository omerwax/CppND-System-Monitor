#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <string>
#include <vector>
#include <sstream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() 
{
    // Read the list of process ids 
    std::vector<int> pids = LinuxParser::Pids();

    // sort the vector
    // std::sort(pids.begin(), pids.end(), [](int a, int b){ return (a > b); });

    // Clear the current processes_ vector 
    processes_.clear();
    
    // Popluate the processes vector
    for (const auto &pid : pids)
    {
        Process proc(pid);
        processes_.emplace_back(proc);
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() 
{ 
    // Check if Kernel_ is already initialized of not, read it one time
    if (this->kernel_ == "")
        this->kernel_ = LinuxParser::Kernel();
    return this->kernel_; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

// Done: Return the operating system name
std::string System::OperatingSystem() 
{ 
    // Check if OS is already initialized of not, read it one time
    if (this->os_ == "")
    {
        os_ = LinuxParser::OperatingSystem();
    }
    return os_;
}

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Done: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Done: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); } 
    
