#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <linux_parser.h>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
    public:
        // process is constructed with a PID and last_time variable is initiated
        Process(int pid) : pid_(pid), uid_(std::string()), 
                    user_(std::string()), command_(std::string()), utilization_(CpuUtilization()) {}

        int Pid();                               // TODO: See src/process.cpp
        std::string User();                      // TODO: See src/process.cpp
        std::string Command();                   // TODO: See src/process.cpp
        float CpuUtilization();                  // TODO: See src/process.cpp
        std::string Ram();                       // TODO: See src/process.cpp
        long int UpTime();                       // TODO: See src/process.cpp
        bool operator<(Process const& a) const;  // TODO: See src/process.cpp
        float Utilization() const { return this->utilization_; } // Return utilization

  // DONE: Declare any necessary private members
    private:
        int pid_;
        std::string uid_ ;
        std::string user_ ;
        std::string command_ ;
        float utilization_;
};


#endif