#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "Servsocket.h"
/*
����� �����:

-�������� ������ �������������
-�������� �� ������� �������
-������� �� ������������
-�������� �� ������������
-��������� ������������
-������� ������������

*/

class ADMIN_INTERFACE {
private:
  static bool ExitFlag;
  static bool Logged;
public:
  ADMIN_INTERFACE() {};

  static void Autorise();

  static void UsersList();
  static void SeeFileContent(std::string);
  static void WatchDB();
  static void CleanDB();
  static void AddUser();
  //void AddUser(std::string, std::string);
  static void DeleteUser();
  void Refresh();

  static void Interface(bool&);
  static void LoginScreen();
};

