#pragma once
#include "pch.h"

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#pragma warning(disable: 4996)


class SOCKET_CONNECTION {
private:
	static SOCKET Sock;
	static std::string login;
	static std::string password;
	static bool Sending_Flag;
  static bool Escape_Flag;

public:
	SOCKET_CONNECTION() {};
	SOCKET_CONNECTION(SOCKET S) { Sock = S; }
  static bool Get_Escape_Flag() { return Escape_Flag; };
	void Connect();
	static void Sorting();
	static void catch_reg();
	static void catch_aut();
	static void catch_files();
	static void ClientHandler(bool&);
	void WaitingForABreak();

  friend class ADMIN_INTERFACE;
  friend std::string JSONtoSTRING(std::ifstream&);
};

bool IsRegistered(std::string, std::string, std::string);
bool find_word(const std::string, const std::string);
std::string FormId();
void AddNewUser(std::string, std::string, std::string);