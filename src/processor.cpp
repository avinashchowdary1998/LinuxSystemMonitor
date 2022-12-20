#include<string>
#include<vector>

#include "processor.h"
#include "linux_parser.h"

using namespace std;


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  
  long Active=LinuxParser::ActiveJiffies();
  long Idle=LinuxParser::IdleJiffies();

  long Total= Active + Idle;

  long TotalDiff=Total-PrevTotal_;
  long IdleDiff=Idle-PrevIdle_;

  Cpu_Percentage_=(float)(TotalDiff-IdleDiff)/TotalDiff;

  PrevTotal_=Total;
  PrevIdle_=Idle;
    

  return Cpu_Percentage_; 
}