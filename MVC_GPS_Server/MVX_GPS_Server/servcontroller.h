#pragma once
//Здесь управление потоками, фильтрация сообщений и чай с плюшками
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include "datamodel.h"

class CONTROLLER : public DATAMODEL {
public:

  bool Authorisation(std::string, std::string);
  void Connect();
  void CloseConnection();

  void ReceiveRestOfJSON(int);
  void ReceiveSingle();
  void ReceiveInAThread(bool&);
  void SwitchReceivingSocket(bool&);

  void SendSingle(int);

  void ParseService();
  void ParseLogin();
  void ParseNMEA();

private:
  SOCKET Sock;
  bool RecFlag = 0;
};


void FormJSON(int);
bool FindLogs(std::string, std::string, std::string); //для имени файла и комбинации логина и пароля
void Write(std::string);