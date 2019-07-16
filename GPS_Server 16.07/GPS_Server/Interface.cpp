#include "pch.h"
#include "Interface.h"


bool ADMIN_INTERFACE::Logged = 0;
bool ADMIN_INTERFACE::ExitFlag = 0;
SOCKET_CONNECTION CON1 = SOCKET_CONNECTION();


void ADMIN_INTERFACE::Autorise() {
  //std::cout << "Начата процедура авторизации" << std::endl;
  WriteIntoLog("Начата процедура авторизации");
  std::cout << "Введите логин: " << std::endl;
  std::string login;
  std::cin >> login;
  std::cout << "Введите пароль: " << std::endl;
  std::string pass;
  std::cin >> pass;
  if (IsRegistered(login, pass, "logadmins.txt")) {
    WriteIntoLog("Найдено соответствие в базе учетных записей");
    std::cout << "Администратор успешно авторизован" << std::endl;
    Sleep(1000);
    Logged = 1;
    //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Interface, &Logged, NULL, NULL);
    WriteIntoLog("Администратор авторизован");
    Interface(Logged);
  }
  else {
    std::cout << "Авторизации не произошло, эх" << std::endl;
    Sleep(1000);
    WriteIntoLog("Ошибка: соответствий не найдено");
    LoginScreen();
  }
};


void ADMIN_INTERFACE::UsersList() {
  WriteIntoLog("Построение списка пользователей...");
  std::cout << "Список пользователей системы" << std::endl;
  std::string mystr;
  std::ifstream infile("logusers.txt");
  WriteIntoLog("Открытие файла...");
  while (!infile.eof())
  {
    getline(infile, mystr);
    if (mystr == "") break;
    mystr.erase(mystr.find_first_of("i"), 12);
    mystr.erase(6, mystr.npos);
    std::cout << mystr << std::endl;
  }
  infile.close();
  Sleep(2000);
  WriteIntoLog("Список пользователей построен");
  Interface(Logged);
};


void ADMIN_INTERFACE::SeeFileContent(std::string filename) {
  WriteIntoLog("Построение списка учетных записей...");
  std::string mystr;
  std::ifstream infile(filename);
  WriteIntoLog("Открытие файла...");
  while (!infile.eof()) 
  {
    getline(infile, mystr);
    std::cout << mystr << std::endl;
  }
  infile.close();
  WriteIntoLog("Список построен");
  Sleep(2000);
  Interface(Logged);
};


void ADMIN_INTERFACE::WatchDB() {
  WriteIntoLog("Смотрим базу данных пользователя");
  std::cout << "Введите логин пользователя" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;
  //std::cout << "Ищем вот это:" << templ << std::endl;
  if (find_word("logusers.txt", templ)) {
    WriteIntoLog("Пользователь найден");
    login += ".txt";
    SeeFileContent(login);
  }
  else {
    WriteIntoLog("Ошибка: не найден пользователь");
    std::cout << "Такого пользователя нет" << std::endl;
    Sleep(1000);
    Interface(Logged);
  }
};


void ADMIN_INTERFACE::CleanDB() {
  WriteIntoLog("Очистка базы данных пользователя");
  std::cout << "Введите логин пользователя" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;
  //std::cout << "Ищем вот это:" << templ << std::endl;

  if (find_word("logusers.txt", templ)) {
    login += ".txt";
    std::fstream in(login, std::ios::out);
    in.close();
    WriteIntoLog("База данных ");
    WriteIntoLog(login);
    WriteIntoLog(" очищена");
    Sleep(2000);
    Interface(Logged);
  }
  else {
    WriteIntoLog("Пользователь не найден");
    std::cout << "Такого пользователя нет" << std::endl;
    Sleep(1000);
    Interface(Logged);
  }
};


void ADMIN_INTERFACE::AddUser() {
  //std::cout << "Начата процедура добавления пользователя" << std::endl;
  WriteIntoLog("Начата процедура добавления пользователя... ");
  std::cout << "Введите логин пользователя" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;
  //std::cout << "Ищем вот это:" << templ << std::endl;

  if (find_word("logusers.txt", templ)) {
    WriteIntoLog("Ошибка: такой логин уже есть");
    std::cout << "Ошибка: пользователь с таким логином уже есть" << std::endl;
    AddUser();
  }
  else {
    WriteIntoLog("Логин корректен");
    std::cout << "Введите пароль пользователя" << std::endl;
    std::string pass;
    std::cin >> pass;
    std::cout << "Записываем данные..." << std::endl;
    AddNewUser(FormId(), templ, pass);

    if (find_word("logusers.txt", templ)) {
      WriteIntoLog("Пользователь добавлен");
      std::cout << "Данные успешно записаны" << std::endl;
      Sleep(1000);
      Interface(Logged);
    }
    else {
      WriteIntoLog("Ошибка: не записались данные");
      std::cout << "Ошибка: не удалось записать данные" << std::endl;
      Sleep(1000);
      AddUser();
    }
  }
};


int LineOfLogin(std::string filename, std::string login) {
  WriteIntoLog("Вычисление строки, где находится нужный логин");
  std::ifstream f;
  f.open(filename);
  if (!f.is_open())
  {
    WriteIntoLog("Не удалось открыть файл с логинами");
    //std::cout << "Error open file" << std::endl;
  }
  std::string::size_type n;
  std::string line;
  int lines_count = 0;
  while (getline(f, line))
  {
    if (!std::all_of(line.begin(), line.end(), isspace)) ++lines_count;
    n = line.find(login, 0);
    if (n == 12) {
      std::cout << "Логин найден в " << lines_count << " строке" << std::endl;
      return lines_count;
    }
    else continue;
  }
  WriteIntoLog("Логин найден");
  f.close();
};


void DeleteLineWithIdChanging(std::string filename, int number) {
  WriteIntoLog("Удаляем данные пользователя");
  std::ifstream f;
  f.open(filename);
  std::ofstream out;
  out.open("out.txt");

  if (!f.is_open())
  {
    WriteIntoLog("Не удалось открыть файл с учетками");
    //std::cout << "Error open file" << std::endl;
  }
  bool isdeleted = 0;
  int lines_count = 0;
  std::string line;

  while (getline(f, line))
  {
    if (!std::all_of(line.begin(), line.end(), isspace)) ++lines_count;
    if (lines_count == number) { //Просто пропустим её
      WriteIntoLog("Строка найдена, удаляем");
      isdeleted = 1;
    }
    else { //А все остальные запишем обратно в файл
      if (isdeleted) { //Нужно поправить id, удалим его и вставим корректное значение
        line.erase(3, 2);
        line.insert(3, std::to_string(lines_count - 1));
        if(lines_count < 10) line.insert(3, "0");
        out << line << std::endl;
      }
      else {
        out << line << std::endl;
      }
    }
  }
  f.close();
  out.close();
  WriteIntoLog("Смещения завершены");

  //Часть с удалением старого файла с данными и переименованием файла вывода
  if (remove("logusers.txt")) {
    WriteIntoLog("Ошибка: не удалось удалить файл");
    //std::cout << "Ошибка: не удалось удалить файл" << std::endl;
    Sleep(1000);
  }
  WriteIntoLog("Удаление завершено");
  //std::cout << "Удаление завершено" << std::endl;
  int result;
  char oldname[] = "out.txt";
  char newname[] = "logusers.txt";
  WriteIntoLog("Переименование выводного файла обратно");
  result = rename(oldname, newname);
  if (result == 0) {
    WriteIntoLog("Переименование успешно завершено");
    //std::cout << "Переименование успешно завершено" << std::endl;
    Sleep(1000);
  }
  else {
    WriteIntoLog("Ошибка: не удалось переименовать файл");
    //std::cout << "Ошибка: не удалось переименовать файл" << std::endl;
    Sleep(1000);
  }
};


void ADMIN_INTERFACE::DeleteUser() {
//Удалить пользователя
  /*
  Открыть файл с учетными записями
  Найти строку с нужной учетной записью
  Удалить эту строку
  Сместить айди всех пользователей, которые ниже, на 1
  Закрыть файл с учетными записями
  */
  WriteIntoLog("Начата процедура удаления пользователя...");
  std::cout << "Введите логин пользователя" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;

  if (find_word("logusers.txt", templ)) {
    WriteIntoLog("Логин найден");
    int line = LineOfLogin("logusers.txt", login);
    //std::cout << "Приступаем к удалению" << std::endl;
    DeleteLineWithIdChanging("logusers.txt", line);
    Sleep(1000);
  }
  else {
    WriteIntoLog("Ошибка: Логин не найден");
    std::cout << "Ошибка: пользователя с таким логином нет в системе" << std::endl;
    Sleep(1000);
    DeleteUser();
  }
};


void ADMIN_INTERFACE::Interface(bool& Flag) {
  while (Flag) {
    system("cls");
    std::cout << "Консоль администратора, версия 1.0" << std::endl;
    std::cout << "Доступные опции:" << std::endl;
    std::cout << "1. Запустить сервер" << std::endl;
    std::cout << "2. Вывести список пользователей" << std::endl;
    std::cout << "3. Вывести учетные данные пользователей" << std::endl;
    std::cout << "4. Просмотреть историю перемещений пользователя" << std::endl;
    std::cout << "5. Очистить историю перемещений пользователя" << std::endl;
    std::cout << "6. Добавить пользователя" << std::endl;
    std::cout << "7. Удалить пользователя" << std::endl;
    std::cout << "8. Разлогиниться" << std::endl;
    int choice;
    std::cin >> choice;
    switch (choice) {
    case 1:
      CON1.Connect();
      CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CON1.Sorting, &CON1.Escape_Flag, NULL, NULL);
      break;
    case 2:
      UsersList();
      break;
    case 3:
      SeeFileContent("logusers.txt");
      break;
    case 4:
      WatchDB();
      break;
    case 5:
      CleanDB();
      break;
    case 6:
      AddUser();
      break;
    case 7:
      DeleteUser();
      break;
    case 8:
    {
      WriteIntoLog("Администратор разлогинился...");
      Logged = 0;
      Flag = 0;
      LoginScreen();
    }
    }
  }
};


void ADMIN_INTERFACE::LoginScreen() {
  
  if (!ExitFlag) {

    while (!ExitFlag) {
      system("cls");
      std::cout << "Сервер GPS-трекинга приветствует вас! чего желаете?" << std::endl;
      std::cout << "Доступны следующие опции: " << std::endl;
      std::cout << "1. Авторизоваться в системе" << std::endl;
      std::cout << "2. Пассивно включить сервер" << std::endl;
      std::cout << "3. Выйти из программы" << std::endl;
      int answer;
      std::cin >> answer;
      switch (answer) {
      case 1:
        Autorise();
        break;
      case 2:
        CON1.Connect();
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CON1.Sorting, &CON1.Escape_Flag, NULL, NULL);
        LoginScreen();
        break;
      case 3:
        WriteIntoLog("Выход из программы...");
        ExitFlag = 1;
        break;
      default:
        std::cout << "Ожидался корректный ввод" << std::endl;
        Sleep(1000);
        break;
      }
    }
  }
};


void ADMIN_INTERFACE::Refresh() {//зайти во все .txt файлы пользователей и очистить их, если месяц сменился
  WriteIntoLog("Начата процедура удаления старых записей из баз данных");
  //int Month = St.wMonth;
  SYSTEMTIME st;
  GetLocalTime(&st);
  int Month = st.wMonth;
  std::string example;
  if (Month < 10) example += "0";
  example += std::to_string(Month);
  std::string mystr;
  std::string dat;
  std::ifstream infile("logusers.txt");

  while (!infile.eof())
  {
    getline(infile, mystr); //Читаем строчки с логинами
    if (mystr == "") break;
    mystr.erase(0, 12);
    mystr.erase(6, mystr.npos);
    mystr += ".txt";
    WriteIntoLog("Логин считан, открываем соответствующий файл...");
    std::ifstream infile2(mystr);
    if (!infile2.is_open()) {
      WriteIntoLog("Ошибка: файл базы данных не может быть открыт");
      //std::cout << "Ошибка: не удалось открыть файл" << std::endl;
    }
    else {
      while (!infile2.eof()) { //Считаем первую строку в файлах с БД, и хватит
        getline(infile2, dat);
        if (dat == "") break;
        std::cout << dat << std::endl;
        dat.erase(0, 32);
        dat.erase(2, 3);
        if (dat != example) {
          WriteIntoLog("Найдены устаревшие записи");
          infile2.close();
          std::fstream in(mystr, std::ios::out);
          in.close();
          WriteIntoLog("Устаревшие записи удалены");
        }
        break;
      }
      infile2.close();
    }
  }
  infile.close();
  WriteIntoLog("Удаление старых записей завершено");
};

