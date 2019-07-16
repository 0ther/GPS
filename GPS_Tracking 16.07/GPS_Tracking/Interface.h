#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <string>
#include <conio.h>
#include <WinSock2.h>
#include "NMEA_Emulator.h"
#include "logwork.h"


class CLIENT_INTERFACE {
private:
  static SOCKET Sock;
  bool ExitFlag = 0;
	static bool SendFlag;

public:
	CLIENT_INTERFACE() {};
  void SetSock(SOCKET Connection);
  static void SendingHandler(bool& Flag);

	void Register();
	void Authorise();
	void StartSending();
	void StopSending();

	bool Get_ExitFlag() { return ExitFlag; };
	void User_Console();
	void Login_Screen();
};




