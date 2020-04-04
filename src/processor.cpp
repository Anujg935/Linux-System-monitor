#include "processor.h"
#include<iostream>
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    return std::stof(LinuxParser::CpuUtilization()[0]);
}