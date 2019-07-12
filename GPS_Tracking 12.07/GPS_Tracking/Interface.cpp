#pragma once
#include "pch.h"
#include "Interface.h"
//#include "logwork.h"


SOCKET CLIENT_INTERFACE::Sock = NULL;
bool CLIENT_INTERFACE::SendFlag = 0;


void CLIENT_INTERFACE::SetSock(SOCKET Connection) {
  Sock = Connection;
};


void CLIENT_INTERFACE::SendingHandler(bool& EscapeFlag) {
  while(EscapeFlag) {
    if (SendFlag == 0) break;
    Sleep(790);
    std::string msg1;
    char buff[60];
    std::ifstream fin("temp.json");

    if (!fin.is_open()) { // если файл не открыт
      //std::cout << "Файл не может быть открыт!\n"; // сообщить об этом
      WriteIntoLog("Ошибка открытия файла с координатами");
    }
    else {
      while (true) {

        fin.getline(buff, 60);
        msg1 = buff;
        int msg_size = msg1.size();
        //send(Connection, (char*)&msg_size, sizeof(int), NULL);
        //std::cout << msg1 << std::endl;
        send(Sock, msg1.c_str(), msg_size, NULL);

        Sleep(30);
        if (msg_size == 0) break;
      };
      //std::cout << "Файл отправлен!" << std::endl;
    };
    fin.close();
  };
}


void CLIENT_INTERFACE::Login_Screen() {
  system("cls");
  std::cout << "Привет, пользователь! Клиент работает. Чего хочешь?" << std::endl;
  std::cout << "1. Авторизоваться в системе" << std::endl;
  std::cout << "2. Зарегистрироваться в системе" << std::endl;
  std::cout << "3. Выйти из программы" << std::endl;
  std::cout << "Введи команду" << std::endl;
  int input;
  std::cin >> input;
  switch (input) {
  case 1:
  {
    WriteIntoLog("Авторизация пользователя...");
    char Type[2] = "2";
    send(Sock, Type, sizeof(Type), NULL);
    Authorise();
    break;
  }
  case 2:
  {
    std::cout << "Регистрация нового пользователя" << std::endl;
    WriteIntoLog("Инициализация процедуры регистрации...");
    char Type[2] = "1";
    send(Sock, Type, sizeof(Type), NULL);
    Register();
    break;
  }
  case 3: //Выход из программы, да
  {
    char Type[2] = "6";
    send(Sock, Type, sizeof(Type), NULL);
    ExitFlag = 1;
    closesocket(Sock);
    break;
  }
  default:
    std::cout << "Введи корректно" << std::endl;
    WriteIntoLog("Некорректый ввод на этапе входа в систему");
    Login_Screen();
    break;
  };
};


void CLIENT_INTERFACE::Register() {
  std::cout << "Введи логин, нужно 6 символов латиницей" << std::endl;
  std::cout << "Логин: ";

  char login[6];
  for (int i = 0; i < 6; i++) std::cin >> login[i];

  send(Sock, login, 6, NULL); //Отослали логин

  char* Smsg1 = new char[1];
  *Smsg1 = '0';
  recv(Sock, Smsg1, sizeof(Smsg1), NULL);
  //std::cout << *Smsg1 << std::endl;

  if (*Smsg1 == '1') {
    WriteIntoLog("Сервер получил корректный логин");
    std::cout << "Теперь введи пароль, нужно 6 цифр" << std::endl << "Пароль: ";
    char password[6];
    for (int i = 0; i < 6; i++) std::cin >> password[i];
    std::cout << "Записываем данные на сервер..." << std::endl;
    int msg_size = sizeof(password);
    //send(Connection, (char*)&msg_size, sizeof(int), NULL);
    send(Sock, password, msg_size, NULL); //Отослали пароль

    char* Smsg2 = new char[1];
    *Smsg2 = '0';
    recv(Sock, Smsg2, sizeof(Smsg2), NULL);
    std::cout << Smsg2 << std::endl;

    if (*Smsg2 == '1') {
      WriteIntoLog("Данные успешно записаны на сервер");
      std::cout << "Отлично! Теперь авторизуйся с этими данными" << std::endl;
      ExitFlag = 1;
      Login_Screen();
    }
    else {
      WriteIntoLog("Ошибка: данные не были записаны на сервер");
      std::cout << "Что-то снова не так, попробуйте ещё раз" << std::endl;
      Login_Screen();
    }
  }
  else {
    //std::cout << "Произошла ошибка, давай ещё раз \n";
    WriteIntoLog("Ошибка: сервер не получил корректный логин");
    Login_Screen();
  }
};


void CLIENT_INTERFACE::Authorise() {
  std::cout << "Авторизация в системе:" << std::endl;
  std::cout << "Введите логин: ";
  char* login = new char(6);
  for (int i = 0; i < 6; i++) std::cin >> login[i];

  send(Sock, login, 6, NULL); //Отослали логин

  std::cout << "Теперь введите пароль" << std::endl << "Пароль: ";
  std::string password;
  std::cin.ignore();
  getline(std::cin, password);
  WriteIntoLog("Сервер сверяет введенные данные...");
  //cout << "Получаем данные об авторизации..." << endl;
  int msg_size = password.size();
  send(Sock, (char*)&msg_size, sizeof(int), NULL);
  send(Sock, password.c_str(), msg_size, NULL); //Отослали пароль

  char* Smsg1 = new char[1];
  *Smsg1 = '0';
  recv(Sock, Smsg1, sizeof(Smsg1), NULL);
  std::cout << *Smsg1 << std::endl;

  if (*Smsg1 == '1') {
    WriteIntoLog("Пользователь был успешно авторизован");
    //std::cout << "Пользователь успешно авторизован" << std::endl;
    User_Console();
  }
  else {
    WriteIntoLog("Ошибка: не была распознана комбинация логина и пароля");
    std::cout << "Что-то пошло не так, сорри" << std::endl;
    Sleep(1000);
	Login_Screen();
  }
};


void CLIENT_INTERFACE::StartSending() {
  SendFlag = 1;
  WriteIntoLog("Начало отправки сообщений...");
  //Вот тут нужно направить сигнал серверу, чтобы он переключился в режим получения файлов
  CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendingHandler, &SendFlag, NULL, NULL);
  User_Console();
};


void CLIENT_INTERFACE::StopSending() {
  SendFlag = 0;
  User_Console();
};


void CLIENT_INTERFACE::User_Console() {
  system("cls");
  std::cout << "Консоль пользователя, версия 1.0" << std::endl;
  std::cout << "Доступные операции: " << std::endl;
  std::cout << "1. Включить передачу данных" << std::endl;
  std::cout << "2. Отключить передачу данных" << std::endl;
  std::cout << "3. Разлогиниться" << std::endl;
  std::cout << "Выбирай мудро, юный падаван" << std::endl;
  int input;
  std::cin >> input;
  switch (input) {
  case 1:
  {
    char Type[] = "3";
    send(Sock, Type, sizeof(Type), NULL);
	StartSending();
	break;
  }
  case 2:
  {
    char Type[] = "4";
    send(Sock, Type, sizeof(Type), NULL);
    StopSending();
    break;
  }
  case 3:
  {
    char Type[] = "9";
    send(Sock, Type, sizeof(Type), NULL);
    Login_Screen();
    break;
  }
  default:
  {
    std::cout << std::endl << "Некорректный ввод, давай по новой" << std::endl;
    WriteIntoLog("Некорректный ввод на этапе работы с клиентом");
    User_Console();
    break;
  }
  };
};


