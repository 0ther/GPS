//Сервер умеет
//Управлять списком пользователей
//Работать с базой данных перемещений
//Визуализировать данные в Google Maps
//Логировать события в системе
//И всё это максимально независимо по ресурсам
#include "pch.h"


#include <iostream>

#include "Interface.h"


int main() {
	setlocale(LC_ALL, "Russian"); 
	
  ADMIN_INTERFACE AINT = ADMIN_INTERFACE();
  AINT.Refresh();
  AINT.LoginScreen();

	system("pause");
	return 0;
}

