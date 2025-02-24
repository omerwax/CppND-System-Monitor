#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <experimental/filesystem>

#include "linux_parser.h"
#include <unistd.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using namespace LinuxParser;

namespace fs = std::experimental::filesystem;


// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
    filestream.close();
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
  }
  return kernel;
}

// DONE : BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() 
{
  vector<int> pids;
  for (auto & dir_item : fs::directory_iterator(fs::path(kProcDirectory)))
    if (fs::is_directory(dir_item)){
      // Is every character of the name a digit?
      string filename(dir_item.path().filename().c_str());
      if (std::all_of(filename.begin(), filename.end(), isdigit)) 
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  return pids;
}


// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  string line;
  string key;
  float mem_total;
  float mem_free;
   
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    
    // Read the first line (with MemTotal)
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> mem_total;
    
    // Read the next line (MemFree)
    std::getline(filestream, line);
    linestream.str(line);
    
    linestream >> key >> mem_free;
       
    return (mem_total - mem_free) / mem_total;

    filestream.close();
    
  }

  return 0;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  long uptime = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    stream.close();
  }
  return uptime; 
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{
   // create a jiffies vector
  std::vector<long> jiffies;
  
  // Read jiffies status and store the vector
  LinuxParser::UpdateJiffies(jiffies);

  long total = 0;
  for (auto const &jiffie : jiffies)
    total += jiffie;
  

  return total;

}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid){ 
  string line;
  string temp_s;
  long temp;
  long jiffies = 0;

  // open the /proc/pid/stat file for reading
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    // Stat is a single line. read it
    std::getline(filestream, line);
    std::istringstream linestream(line);
    
    // go through the process /stat and count relevant jiffies
    for (auto i = 1; i <= kProcessUpTime; i++) 
    {
      switch (i)
      {
      case kProcessCpuUtime:
        linestream >> temp;
        jiffies += temp;
        break;
      case kProcessCpuStime:
        linestream >> temp;
        jiffies += temp;
        break;
      case kProcessCpuCUtime:
        linestream >> temp;
        jiffies += temp;
        break;
      case kProcessCpuCStime:
        linestream >> temp;
        jiffies += temp;
        break;
      default:
        linestream >> temp_s;
        break;
      }
    } // for

    filestream.close();
  }// if 
  
  return jiffies; 
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  // create a jiffies vector
  std::vector<long> jiffies;
  
  // Read jiffies status and store the vector
  LinuxParser::UpdateJiffies(jiffies);
  
  // Return the active jiffies
  return jiffies[kUser_] + jiffies[kNice_] + jiffies[kSystem_] + 
          jiffies[kIRQ_] + jiffies[kSoftIRQ_] + jiffies[kSteal_];
  
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{ 
  // create a jiffies vector
  std::vector<long> jiffies;
  
  // Read jiffies status and store the vector
  LinuxParser::UpdateJiffies(jiffies);
  
  // Return the idle jiffies
  return jiffies[kIdle_] + jiffies[kIOwait_]; 
  
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  
  string line;
  string key;
  string val;
  int processes;
   
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    // Iterate through the file, look for the line with the VmSize
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "processes") {
          linestream >> processes;
          return processes;
        }
      }
    }
    filestream.close();
  }
  return int();
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  string line;
  string key;
  string val;
  int processes;
   
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    // Iterate through the file, look for the line with the VmSize
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "procs_running") {
          linestream >> processes;
          return processes;
        }
      }
    }
    filestream.close();
  }
  return int();
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{ 
  string command;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
    stream.close();
  }
  return command; 
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid)
{
  string line;
  string key;
  string val;
  long ram;
   
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    // Iterate through the file, look for the line with the VmSize
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        // Original instructions on the project page were to us VmSize which includes all mapped virtual memeory of a process
        // including disk space and etc. VmRSS displays only RAM used by a process which is more friendly and meaningful to users
        if (key == "VmRSS:") {
          linestream >> ram;
          ram /= 1024; // convert KBytes to MBytes  
          return to_string(ram);
        }
      }
    }
    filestream.close();
  }
  return string();
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid)
{
  
  string line;
  string key;
  string val;
   
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    // Iterate through the file, look for the line with the uid
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> val) {
        if (key == "Uid:") {
          return val;
        }
      }
    }
    filestream.close();
  }
  return val;
}  


// DONE: Read and return the user associated with a process
string LinuxParser::User(std::string uid)
{
  if (uid.empty())
    return string();
  
  string line;
  string user;
  string x;
  string uid_;
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    // Iterate through the file, look for the line with the uid
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid_) {
        if (uid_ == uid) {
          return user;
        }
      }
    }
    filestream.close();
  }
  return user;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid)
{
  string line;
  string temp;
  long uptime = 0;
   
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    // Stat is a single line. read it
    std::getline(filestream, line);
    std::istringstream linestream(line);
    
    // read the first (KprocessUpTime - 1) items in the line to temp (they are not needed)
    for (auto i = 1; i < kProcessUpTime; i++) 
      linestream >> temp;
    
    // Read the 22 item to uptime
    linestream >> uptime;

    // Convert from clock tick to seconds
    uptime /= sysconf(_SC_CLK_TCK);

    // Process up time is relative to system start-time, substract it from system up to time to get absolute value
    uptime = LinuxParser::UpTime() - uptime; 

    filestream.close();
     
  }

  return uptime;

}

// Read all the jiffies and store them in  a vector of jiffies (long)
void LinuxParser::UpdateJiffies(std::vector<long> &jiffies)
{
  string line;
  string temp_s;
  
  // open the /proc/stat file for reading
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if (filestream.is_open()) {
    // Read the first line (holding the cpu inf0)
    std::getline(filestream, line);
    std::istringstream linestream(line);
    
    // Read the first line item (should be cpu) 
    linestream >> temp_s;
    
    jiffies.resize(kGuest_);
    // go through the process /stat and store all jiffies
    for (auto i = 0; i < kGuest_; i++)
      linestream >> jiffies[i];

    filestream.close();  
       
  } // if
}