#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
using std::string;
using std::to_string;
using std::vector;
#include "linux_parser.h"
Process::Process(int pid)
{
    this->pid = pid;
}
// TODO: Return this process's ID
int Process::Pid() 
{  
   return this->pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const{ return  static_cast<float>(((LinuxParser::ActiveJiffies(this->pid) * 1.0) /(this->UpTime() *1.0)/(sysconf(_SC_CLK_TCK)*1.0))); }
 
// TODO: Return the command that generated this process
string Process::Command() { return  LinuxParser::Command(this->pid); }

// TODO: Return this process's memory utilization
string Process::Ram(){ return LinuxParser::Ram(this->pid);}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(this->pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const{ return (LinuxParser::UpTime() - LinuxParser::UpTime(this->pid) / sysconf(_SC_CLK_TCK)); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const
 {
    return (a.CpuUtilization() < this->CpuUtilization());
 }
