#include "processor.h"

#include <string>
#include <vector>
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> str = LinuxParser::CpuUtilization();

  long prevtotal = std::stol(str.at(0));
  long prevactive = std::stol(str.at(1));
  long currtotal = std::stol(str.at(2));;
  long curractive = std::stol(str.at(3));

  long totald = currtotal - prevtotal;
  long active = curractive - prevactive;

  float CPU_Percentage = (active * 1.0) / (totald * 1.0);

  return CPU_Percentage;
}
