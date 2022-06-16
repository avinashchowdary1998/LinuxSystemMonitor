#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
   long PrevTotal_=0, PrevIdle_=0, Total_, Active_, Idle_;
   
   float Cpu_Percentage_;
};

#endif