#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : Pid_(pid) {

}//constructor

// TODO: Return this process's ID
int Process::Pid() { return Pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization()  {
    long uptime=LinuxParser::UpTime();
    long total_time=LinuxParser::ActiveJiffies(Pid_);
    long start_time=LinuxParser::UpTime(Pid_);
    long Total_Elapsed_Time;
    Total_Elapsed_Time=uptime-start_time;
    Process::CpuUtil_= total_time / static_cast<float>(Total_Elapsed_Time); 
    return Process::CpuUtil_; 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid_); }

// TODO: Return this process's memory utilization
string Process::Ram()  { return to_string(stoi(LinuxParser::Ram(Pid_))/1024); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return (LinuxParser::UpTime()-LinuxParser::UpTime(Pid_)); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator>(Process const& a) const {

    return this->CpuUtil_ > a.CpuUtil_; 
}