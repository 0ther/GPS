#pragma once

#include "servcontroller.h"

class INTERFACE : public CONTROLLER {
public:
  INTERFACE() {};
  void Login();
  void Console();
  void Quit();
};

bool CheckLogData(std::string, std::string);
