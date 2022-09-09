#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <ctime>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

const int kProcessUpTime = 22;    // Process startup time
const int kProcessCpuUtime = 14;  // time spent is user mode
const int kProcessCpuStime = 15;  // time spent is kernel mode
const int kProcessCpuCUtime = 16; // time of children processes spent is user mode
const int kProcessCpuCStime = 17; // time of children processes spent is kernel mode


// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();
void UpdateJiffies(std::vector<long> &jiffies);
// Processes
struct ProcessCpuTime
{
  long u_time = 0;  // User mode cpu time   
  long s_time = 0;
  long cu_time = 0;
  long cs_time = 0;
  long st_time = 0;
};

std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(std::string uid);
long int UpTime(int pid);
void CpuUtilization(int pid, ProcessCpuTime &time);

};  // namespace LinuxParser

#endif