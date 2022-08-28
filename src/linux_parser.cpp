#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
  std::ifstream total_stream(kProcDirectory + "/meminfo");
  long total_mem;
  long available_mem;
  long free_mem;
  long buffers_mem;
  long chached_mem;

  if (total_stream.is_open()) {
    string str_dummy;
    total_stream >> str_dummy >> total_mem >> str_dummy;
     total_stream >> str_dummy >> free_mem >> str_dummy;
    total_stream >> str_dummy >> available_mem >> str_dummy;
    total_stream >> str_dummy >> buffers_mem >> str_dummy;
    total_stream >> str_dummy >> chached_mem >> str_dummy;

    total_stream.close();
  }
  float utlilized_mem =  (total_mem * 1.0 - buffers_mem * 1.0 - chached_mem * 1.0 )/ total_mem * 1.0;
  return (utlilized_mem);
}


// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string str;
  if (stream.is_open()) {
    stream >> str;
  }
  double uptime = stod(str);
  return ((long)uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string str;
   long jiffies_temp = 0 ;
  long total_jiffies = 0;
  if (stream.is_open()) {
    stream >> str;
    for (int i = 0; i < 9; i++) {
      stream >> jiffies_temp;
      total_jiffies += jiffies_temp;
    }
  }
  return ((long)total_jiffies);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid ) 
{  

  std::ifstream stream(kProcDirectory + "/" +std::to_string(pid) +  kStatFilename);
  long jiffies_temp[4] = {0} ;
  long active_jiffies = 0;
  if (stream.is_open()) {
    string ignore_jiffies;
    for (int i = 0; i < 13; i++) {
      stream >> ignore_jiffies;
    }
    stream >> jiffies_temp[0] >> jiffies_temp[1] >> jiffies_temp[2] >> jiffies_temp[3];
  }
  active_jiffies = jiffies_temp[0] + jiffies_temp[1] + jiffies_temp[2] + jiffies_temp[3];
  return ((long)active_jiffies);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return (Jiffies() - IdleJiffies()); }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string str;
  long jiffies_temp[2] = {0} ;
  string ignore_jiffies;
  long idle_jiffies = 0;
  if (stream.is_open()) {
    stream >> str;
    for (int i = 0; i < 3; i++) {
      stream >> ignore_jiffies;
    }
    stream >> jiffies_temp[0] >> jiffies_temp[1];
  }
  idle_jiffies = jiffies_temp[0] + jiffies_temp[1];
  return ((long)idle_jiffies);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  static long prevtotal = 0;
  static long prevactive= 0;
  long currtotal = Jiffies();
  long curractive = ActiveJiffies();

  vector<string> cpu_precantage;

  cpu_precantage.push_back(to_string(prevtotal));
  cpu_precantage.push_back(to_string(prevactive));
  cpu_precantage.push_back(to_string(currtotal));
  cpu_precantage.push_back(to_string(curractive));

  prevtotal = currtotal;
  prevactive = curractive;

  return cpu_precantage;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{  
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string total_process_str = "processes";
  std::string key ;
  std::string value;
  int total_processes;
  if (stream.is_open()) 
  {
    std::string str;
    while(std::getline( stream, str ))
    {
      std::istringstream this_line(str);
      this_line >> key >> value;
      if(key == total_process_str)
      {
        total_processes = stoi(value);

      }
    }
  }
   return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string running_process_str = "procs_running";
  std::string key ;
  std::string value;
  int running_processes;
  if (stream.is_open()) 
  {
    std::string str;
    while(std::getline( stream, str ))
    {
      std::istringstream this_line(str);
      this_line >> key >> value;
      if(key == running_process_str)
      {
        running_processes = stoi(value);

      }
    }
  }
   return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
  std::ifstream stream(kProcDirectory + "/" + to_string(pid)  + kCmdlineFilename);
  if (stream.is_open()) 
  {
     string str;
     if(getline(stream , str)){
       return str;
     }
  }

} 

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
  std::ifstream stream(kProcDirectory + "/" + to_string(pid)  + kStatusFilename);
  std::string rss_memory_str = "VmRSS:";
  std::string key ; 
  std::string value;
  std::string rss_memory;
  if (stream.is_open()) 
  {
    std::string str;
    while(std::getline( stream, str ))
    {
      std::istringstream this_line(str);
      this_line >> key >> value;
      if(key == rss_memory_str)
      {
        rss_memory = value;
      }
    }
  }
  long value_kb = stol(rss_memory);
  long value_mb = value_kb / 1000;
   return to_string(value_mb);
} 
// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{ 
   std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  std::string uid_str = "Uid:";
  std::string key ;
  std::string value;
  std::string uid;
  if (stream.is_open()) 
  {
    std::string str;
    while(std::getline( stream, str ))
    {
      std::istringstream this_line(str);
      this_line >> key >> value;
      if(key == uid_str)
      {
        uid = value;
      }
    }
  }
   return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid ) 
{ 
    string uid = LinuxParser::Uid(pid);
    std::ifstream stream(kPasswordPath);
    std::string user;
    if (stream.is_open()) 
    {
      std::string str;
      while(std::getline( stream, str ))
      {
        std::vector <string> out;
        char *token = strtok(const_cast<char *>(str.c_str()), ":");
        while (token != nullptr)
        {
        out.push_back(std::string(token));
        token = strtok(nullptr, ":");
        }
      if(out.at(2) == uid){
       user = out.at(0);
       break;
      }
    }
  }
return user;    
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{
   std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
   long uptime_specific;  
   if (stream.is_open())
   {
     std::string dummy;
      for(int i = 1 ; i <= 21 ; i++)
      {
        stream >> dummy;
      }
      stream >> uptime_specific; 
   }
  return uptime_specific;
}
