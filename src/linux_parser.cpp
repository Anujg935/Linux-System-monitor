#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
  string line;
  string key;
  string value;
  long MemTotal = 0.0;
  long MemAvailable = 0.0;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = stoi(value);
        }
        else if (key == "MemAvailable:") {
          MemAvailable = stoi(value);
          break;
        }
      }
    }
  }
  float MemUseGB = (MemTotal - MemAvailable)/(1024*1024);
  float MemTotalGB = MemTotal/(1024*1024);
  
  
  return MemUseGB/MemTotalGB;
}

// TODO: Read and return the system uptime
long int LinuxParser::UpTime() {
  string line;
  long idle;
  long upTime = 0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idle;
  }
  
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string cpu;
  long user;
  long nice;
  long system;
  long idle;
  long iowait;
  long irq;
  long softirq;
  long steal;
  long guest;
  long guest_nice;

  string line;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> cpu >>user >>nice >>system >>idle >> iowait >> irq >> softirq >>steal >> guest >> guest_nice;
  }

  
  long usertime = user - guest;                     
  long nicetime = nice - guest_nice;                 
  
  long idlealltime = idle + iowait;                 
  long systemalltime = system + irq + softirq;
  long virtalltime = guest + guest_nice;
  long totaltime = usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;
  
  return totaltime;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
float LinuxParser::ActiveJiffies(int pid) {
  string line;
  long utime;
  long stime;
  long cutime;
  long cstime;
  long starttime;
  long uptime = UpTime();
  
  string ignore;
  std::ifstream fileStream(kProcDirectory+"/"+to_string(pid)+kStatFilename);
  if(fileStream.is_open()){
    std::getline(fileStream,line);
    std::istringstream lineStream(line);

    for(int i=0;i<13;i++){lineStream >>ignore;}
    lineStream >> utime >> stime >> cutime >> cstime;
    for(int i=0;i<4;i++){lineStream >> ignore;}
    lineStream >> starttime;
  }
  long total_time = utime + stime + cutime + cstime;

  long seconds = uptime - (starttime*(1.0) / sysconf(_SC_CLK_TCK));

  float cpu_usage = ((total_time / sysconf(_SC_CLK_TCK))*1.0 / seconds);

  return cpu_usage;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string cpu;
  long user;
  long nice;
  long system;
  long idle;
  long iowait;
  long irq;
  long softirq;
  long steal;
  long guest;
  long guest_nice;

  string line;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> cpu >>user >>nice >>system >>idle >> iowait >> irq >> softirq >>steal >> guest >> guest_nice;
  }

                 
  
  long idlealltime = idle + iowait;                 
  
  
  
  return idlealltime;
}

// TODO: Read and return CPU utilization
vector<LinuxParser::CpuTimeUtil> LinuxParser::CpuTime(){
  string cpu;
  long user;
  long nice;
  long system;
  long idle;
  long iowait;
  long irq;
  long softirq;
  long steal;
  long guest;
  long guest_nice;
  vector<LinuxParser::CpuTimeUtil> t;
  string line;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> cpu >>user >>nice >>system >>idle >> iowait >> irq >> softirq >>steal >> guest >> guest_nice;

      if(cpu.substr(0,3) !="cpu"){
        return t;
      }
      LinuxParser::CpuTimeUtil obj;
      obj.idleTime = idle+iowait;
      obj.totalTime = user + nice + system + irq + softirq + idle+iowait; 
      t.push_back(obj);
    }
    return t;
  }
  
}
vector<string> LinuxParser::CpuUtilization() {
  vector<string> res;
  vector<LinuxParser::CpuTimeUtil> prevVec = LinuxParser::CpuTime();
  sleep(2);
  vector<LinuxParser::CpuTimeUtil> currVec = LinuxParser::CpuTime();
  for(int i=0;i<currVec.size();i++){
    long idleT = currVec[i].idleTime - prevVec[i].idleTime;
    long totalT = currVec[i].totalTime - prevVec[i].totalTime;

    float utilization = (totalT - idleT)*1.0/totalT*1.0;
    res.push_back(to_string(utilization));
  }
  return res;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int processes;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "processes"){
        linestream >> processes;
        break;
      }
    }
  }
  return processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int running_processes;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "procs_running"){
        linestream >> running_processes;
        break;
      }
    }
  }
  return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;

  std::ifstream fileStream(kProcDirectory+"/"+to_string(pid)+kCmdlineFilename);
  if(fileStream.is_open()){
    std::getline(fileStream ,line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string text;
  long mem;
  std::ifstream fileSystem(kProcDirectory+"/"+to_string(pid)+kStatusFilename);

  if(fileSystem.is_open()){
    while(getline(fileSystem,line)){
      std::istringstream lineStream(line);
      lineStream >>text;
      if(text =="VmSize:"){
        lineStream >> mem;
        break;
      }
    }
  }
  mem = mem/1024;
  return to_string(mem);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string uid;
  std::ifstream fileStream(kProcDirectory+"/"+to_string(pid)+kStatusFilename);
  if(fileStream.is_open()){
    while(std::getline(fileStream ,line)){
      std::istringstream linestream(line);
      linestream >>key;
      if(key=="Uid:"){
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line;
  string user;
  string paswd;
  string currUid;
  std::ifstream fileStream(kPasswordPath);
  if(fileStream.is_open()){
    while(std::getline(fileStream,line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      lineStream >> user >> paswd >> currUid;
      if(currUid==uid){
        break;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string ignore;
  long utime;
  std::ifstream fileStream(kProcDirectory+"/"+to_string(pid)+kStatFilename);
  if(fileStream.is_open()){
    std::getline(fileStream,line);
    std::istringstream lineStream(line);

    for(int i=0;i<13;i++){lineStream >>ignore;}
    lineStream >> utime ;
    
  }
  return (UpTime() - utime)/sysconf(_SC_CLK_TCK);
}