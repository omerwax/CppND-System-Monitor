#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Dobe: Return this process's ID
int Process::Pid() { return this-> pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
    
    // Calc is done according to the deifition here:
    // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    
    // overall time spent (user and kernel mode, incuuding child processes)
    float total  = LinuxParser::ActiveJiffies(this->pid_);
  
    // System up time;
    long system_up_time  = LinuxParser::UpTime();
    float p_start_time = LinuxParser::UpTime(this->pid_);

    long hertz = sysconf(_SC_CLK_TCK);
    
    long seconds = system_up_time - (p_start_time / hertz);

    this->utilization_ =  (((total / hertz) / seconds));

    return this->utilization_;
    
}

// DONE: Return the command that generated this process
string Process::Command() 
{
    // Check if Command_ is already initialized if not, read it once
    if (this->command_.empty())
        this->command_ = LinuxParser::Command(this->pid_);
    
    return this->command_; 
}

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(this->pid_); }

// DONE: Return the user (name) that generated this process
string Process::User()
{
    // get UID first and then then USER
    // check if user_ is already initialized if not, read it once
    if (this->uid_.empty())
        this->uid_ = LinuxParser::Uid(this->pid_);

    // Check if user_ is already initialized if not, read it once
    if (this->user_.empty())
        this->user_ = LinuxParser::User(this->uid_);
    
    return this->user_; 
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return a.Utilization() < this->Utilization(); }