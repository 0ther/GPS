#pragma once


#include "pch.h"
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include "Interface.h"

#pragma comment(lib, "ws2_32.lib")



int main()
{
	ClearLog();
  WriteIntoLog("Начало работы программы");
	setlocale(LC_ALL, "Russian");
  
	WriteIntoLog("Запуск эмулятора GPS-датчика...");
	GPS_EMULATOR EMU1 = GPS_EMULATOR();
  EMU1.StartGenerating();
	
	WriteIntoLog("Запуск клиентского интерфейса...");
	CLIENT_INTERFACE INT1 = CLIENT_INTERFACE();

	std::ifstream logfile("client.log");

  SOCKET Connection;
	if (!logfile.is_open()) {
		WriteIntoLog("Ошибка, не удалось создать файл логирования событий"); //Вот тут крайне странная логика работы
	}
	else {
		WriteIntoLog("Запуск сокета...");
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			WriteIntoLog("Ошибка запуска сокета");
			exit(1);
		}

		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;

		WriteIntoLog("Соединение с сервером...");
		Connection = socket(AF_INET, SOCK_STREAM, NULL);
		if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
			std::cout << "Ошибка: не удалось соединиться с сервером." << std::endl;
			WriteIntoLog("Ошибка: не удалось соединиться с сервером");
			return 1;
		}
		std::cout << "Клиент успешно соединён с сервером" << std::endl;

    INT1.SetSock(Connection);
	
		WriteIntoLog("Клиент начинает работу...");
	
		while (!INT1.Get_ExitFlag()) {
			INT1.Login_Screen();
		}
	}

	WriteIntoLog("Выход из программы...");

	WSACleanup();
	closesocket(Connection);
	system("pause");
	return 0;
}

