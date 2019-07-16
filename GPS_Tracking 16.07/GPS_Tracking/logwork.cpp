#include "pch.h"
#include "logwork.h"


std::ofstream logfile;


void WriteIntoLog(std::string Message) {
  logfile.open("client.log", std::ios::app);
  logfile << Message << std::endl;
  logfile.close();
}


void ClearLog() {
  logfile.open("client.log");
  logfile.clear();
}