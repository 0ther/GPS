#include "pch.h"
#include "serverview.h"

void INTERFACE::Login() {
  std::string login, password;
  std::cout << "Введите логин: ";
  //std::cin.ignore();
  getline(std::cin, login);
  std::cout << "Введите пароль: ";
  //std::cin.ignore();
  getline(std::cin, password);
  std::cout << std::endl;
  if (CheckLogData(login, password)) {
    if (Authorisation(login, password)) {
      std::cout << "Авторизация пройдена, запускаю сервер" << std::endl;
      std::thread StartServer(&CONTROLLER::Connect, this);
      StartServer.detach();
      Console();
    }
  }
  else {
    std::cout << "Ошибка авторизации" << std::endl;
  }
};


void INTERFACE::Console() { //реализации функций можно потянуть из старого проекта
  std::cout << "Консоль администратора, версия 1.0" << std::endl;
  std::cout << "Доступные опции:" << std::endl;
  std::cout << "1. Добавить пользователя" << std::endl;
  std::cout << "2. Удалить пользователя" << std::endl;
  std::cout << "3. Разлогиниться" << std::endl;
  int choice;
  std::cin >> choice;
  switch (choice) {
  case 1: 
  {
    //AddUser();
  }
  case 2:
  {
    //DeleteUser();
  }
  case 3:
    Quit();
    break;
  default:
    exit(1);
  }
};


void INTERFACE::Quit() {
  //Останавливаем потоки
  //Разрываем соединение
  WSACleanup();
  CloseConnection();
};


bool CheckLogData(std::string login, std::string pass) {
  if (login.size() != 6 || pass.size() != 6) return false;
  for (int i = 0; i < 6; i++) {
    char temp = login[i];
    if ((int)temp < 97 || (int)temp > 122) return false; //Только 6 латинских символов
  }
  for (int i = 0; i < 6; i++) {
    char temp = pass[i];
    if ((int)temp < 48 || (int)temp > 57) return false; //Только 6 цифр
  }
  return true;
};