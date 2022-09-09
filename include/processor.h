#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();  // Done: See src/processor.cpp

  // Done: Declare any necessary private members
 private:
    std::vector<long> jiffies_;
    long idle_ = 0;
    long active_ = 0;
};

#endif