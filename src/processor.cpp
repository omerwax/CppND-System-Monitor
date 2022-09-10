#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    // get the current jiffies status
    long current_idle = LinuxParser::IdleJiffies();
    long current_active = LinuxParser::ActiveJiffies(); 
    
    // Calculate the differential values
    long idle = current_idle - idle_;
    long total = idle + current_active - active_;
    // Protect against zero devidance
    if (total == 0)
        return 0;
    
    // calcualet the utilization
    float utilization =  1.0 * (total - idle) / total;

    // update the previous values
    active_ = current_active;
    idle_ = current_idle;

    return(utilization);
}