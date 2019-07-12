#include "pch.h"
#include "Interface.h"


bool ADMIN_INTERFACE::Logged = 0;
bool ADMIN_INTERFACE::ExitFlag = 0;
SOCKET_CONNECTION CON1 = SOCKET_CONNECTION();


void ADMIN_INTERFACE::Autorise() {
  std::cout << "Начата процедура авторизации" << std::endl;
  std::cout << "Введите логин: " << std::endl;
  std::string login;
  std::cin >> login;
  std::cout << "Введите пароль: " << std::endl;
  std::string pass;
  std::cin >> pass;
  if (IsRegistered(login, pass, "logadmins.txt")) {
    std::cout << "Администратор успешно авторизован" << std::endl;
    Sleep(1000);
    Logged = 1;
    //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Interface, &Logged, NULL, NULL);
    Interface(Logged);
  }
  else {
    std::cout << "Авторизации не произошло, эх" << std::endl;
    Sleep(1000);
    LoginScreen();
  }
};


void ADMIN_INTERFACE::UsersList() {
  std::cout << "Список пользователей системы" << std::endl;
  std::string mystr;
  std::ifstream infile("logusers.txt");
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
  Interface(Logged);
};


void ADMIN_INTERFACE::SeeFileContent(std::string filename) {
  std::string mystr;
  std::ifstream infile(filename);
  while (!infile.eof()) 
  {
    getline(infile, mystr);
    std::cout << mystr << std::endl;
  }
  infile.close();
  Sleep(2000);
  Interface(Logged);
};


void ADMIN_INTERFACE::WatchDB() {
  std::cout << "Введите логин пользователя" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;
  std::cout << "Ищем вот это:" << templ << std::endl;
  if (find_word("logusers.txt", templ)) {
    login += ".txt";
    SeeFileContent(login);
  }
  else {
    std::cout << "Такого пользователя нет" << std::endl;
    Sleep(1000);
    Interface(Logged);
  }
};


void ADMIN_INTERFACE::CleanDB() {
  std::cout << "Введите логин пользователя" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;
  std::cout << "Ищем вот это:" << templ << std::endl;

  if (find_word("logusers.txt", templ)) {
    login += ".txt";
    std::fstream in(login, std::ios::out);
    in.close();
    Sleep(2000);
    Interface(Logged);
  }
  else {
    std::cout << "Такого пользователя нет" << std::endl;
    Sleep(1000);
    Interface(Logged);
  }
};


void ADMIN_INTERFACE::AddUser() {
  std::cout << "Начата процедура добавления пользователя" << std::endl;
  std::cout << "Введите логин пользователя" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;
  std::cout << "Ищем вот это:" << templ << std::endl;

  if (find_word("logusers.txt", templ)) {
    std::cout << "Ошибка: пользователь с таким логином уже есть" << std::endl;
    AddUser();
  }
  else {
    std::cout << "Введите пароль пользователя" << std::endl;
    std::string pass;
    std::cin >> pass;
    std::cout << "Записываем данные..." << std::endl;
    AddNewUser(FormId(), templ, pass);

    if (find_word("logusers.txt", templ)) {
      std::cout << "Данные успешно записаны" << std::endl;
      Sleep(1000);
      Interface(Logged);
    }
    else {
      std::cout << "Ошибка: не удалось записать данные" << std::endl;
      Sleep(1000);
      AddUser();
    }
  }
};


int LineOfLogin(std::string filename, std::string login) {
  std::ifstream f;
  f.open(filename);
  if (!f.is_open())
  {
    std::cout << "Error open file" << std::endl;
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
  f.close();
};


void DeleteLineWithIdChanging(std::string filename, int number) {
  std::ifstream f;
  f.open(filename);
  std::ofstream out;
  out.open("out.txt");

  if (!f.is_open())
  {
    std::cout << "Error open file" << std::endl;
  }
  bool isdeleted = 0;
  int lines_count = 0;
  std::string line;

  while (getline(f, line))
  {
    if (!std::all_of(line.begin(), line.end(), isspace)) ++lines_count;
    if (lines_count == number) { //Просто пропустим её
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

  //Часть с удалением старого файла с данными и переименованием файла вывода
  if (remove("logusers.txt")) {
    std::cout << "Ошибка: не удалось удалить файл" << std::endl;
    Sleep(1000);
  }
  std::cout << "Удаление завершено" << std::endl;
  int result;
  char oldname[] = "out.txt";
  char newname[] = "logusers.txt";
  result = rename(oldname, newname);
  if (result == 0) {
    std::cout << "Переименование успешно завершено" << std::endl;
    Sleep(1000);
  }
  else {
    std::cout << "Ошибка: не удалось переименовать файл" << std::endl;
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
  std::cout << "Введите логин пользователя" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;

  if (find_word("logusers.txt", templ)) {
    int line = LineOfLogin("logusers.txt", login);
    std::cout << "Приступаем к удалению" << std::endl;
    DeleteLineWithIdChanging("logusers.txt", line);
    Sleep(1000);
  }
  else {
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
    std::ifstream infile2(mystr);
    if (!infile2.is_open()) {
      std::cout << "Ошибка: не удалось открыть файл" << std::endl;
    }
    else {
      while (!infile2.eof()) { //Считаем первую строку в файлах с БД, и хватит
        getline(infile2, dat);
        if (dat == "") break;
        std::cout << dat << std::endl;
        dat.erase(0, 32);
        dat.erase(2, 3);
        if (dat != example) {
          infile2.close();
          std::fstream in(mystr, std::ios::out);
          in.close();
        }
        break;
      }
      infile2.close();
    }
  }
  infile.close();
};

