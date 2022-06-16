#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, value;
  string line;
  float total, free;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(getline(stream,line)) {
      std::istringstream linestream(line);
      linestream>>key>>value;
      if(key=="MemTotal:") 
        total=stof(value);
      if(key=="MemFree:") {
        free=stof(value);
        return (total-free)/total;
      }
    }
    
  } 
  return 0.0; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string value;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  } 
  return std::stol(value); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> jiffies=LinuxParser::CpuUtilization();
  long total=0;
  for(auto i : jiffies)
    total+=stol(i);
  return total; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  long total=0;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if(stream.is_open()) {
    getline(stream, line);
    std::istringstream linestream(line);
    for(int i=1;i<=17;i++) {
      linestream>>value;
      if(i>=14 && i<=17) {
        total=total+(stol(value)/sysconf(_SC_CLK_TCK));
      }
    }
    return total;
  } 
  return 0; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> jiffies=LinuxParser::CpuUtilization();
  
  return stol(jiffies[kUser_])+stol(jiffies[kNice_])+stol(jiffies[kSystem_])+stol(jiffies[kIRQ_])+stol(jiffies[kSoftIRQ_])+stol(jiffies[kSteal_]); 
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies=LinuxParser::CpuUtilization();

  return stol(jiffies[kIdle_])+stol(jiffies[kIOwait_]); 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuUtilInfo;
  string word, time;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> word;
    while(linestream>>time) {
      cpuUtilInfo.emplace_back(time);

    }
  }
  return cpuUtilInfo; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if(linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
} 


// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if(linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
      
    }
  } 
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmd;
  std::ifstream stream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if(stream.is_open()) {
    getline(stream, cmd);
    return cmd;
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatusFilename);
  while(getline(stream, line)) {
    std::istringstream linestream(line);
    linestream>>key>>value;
    if(key=="VmSize:") {
      return value;
    }
  }
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatusFilename);
  if(stream.is_open()) {
    while(getline(stream, line)) {
      std::istringstream linestream(line);
      linestream>>key>>value;
      if(key=="Uid:") {
        return value;
      }
    }
  } 
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string Uid=LinuxParser::Uid(pid);
  string line, key, x, value;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream>>key>>x>>value;
      if(value==Uid) {
        return key;
      }
    }
  } 
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, uptime;
  long uptime_seconds;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if(stream.is_open()) {
    getline(stream, line);
    std::istringstream linestream(line);
    for(int i=1;i<=22;i++) {
      linestream>>uptime;
    }
    uptime_seconds=stol(uptime)/sysconf(_SC_CLK_TCK);
    return uptime_seconds;
  } 
  return 0; 
}
