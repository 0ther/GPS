#include "pch.h"
#include "logwork.h"


std::ofstream logfile;


void WriteIntoLog(std::string Message) {
  logfile.open("server.log", std::ios::app);
  logfile << Message << std::endl;
  logfile.close();
}


void ClearLog() {
  logfile.open("server.log");
  logfile.clear();
}