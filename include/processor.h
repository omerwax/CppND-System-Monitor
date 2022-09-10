#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  Processor() : idle_(0), active_(0) {}
  float Utilization();  // DONE: See src/processor.cpp

  // DONE: Declare any necessary private members
 private:
    long idle_;
    long active_;
};

#endif