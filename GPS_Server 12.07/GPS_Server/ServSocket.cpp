#include "pch.h"
#include "Servsocket.h"


bool SOCKET_CONNECTION::Sending_Flag = 0;
bool SOCKET_CONNECTION::Escape_Flag = 0;
SOCKET SOCKET_CONNECTION::Sock = NULL;
std::string SOCKET_CONNECTION::login = "";
std::string SOCKET_CONNECTION::password = "";


void SOCKET_CONNECTION::Connect() {
  //WSAStartup
  WSAData wsaData;
  WORD DLLVersion = MAKEWORD(2, 1);
  if (WSAStartup(DLLVersion, &wsaData) != 0) {
    std::cout << "Error" << std::endl;
    exit(1);
  }

  SOCKADDR_IN addr;
  int sizeofaddr = sizeof(addr);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr.sin_port = htons(1111);
  addr.sin_family = AF_INET;

  SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
  bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
  listen(sListen, SOMAXCONN);

  SOCKET newConnection;
  std::cout << "Ожидаем подключений..." << std::endl;
  newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

  if (newConnection == 0) {
    std::cout << "Error #2\n";
  }
  else {
    std::cout << "Client Connected!\n";
    Sock = newConnection;
    //Sorting();
  }
}


bool switch_flag(bool &input) {
  if (input == true) input = false;
  if (input == false) input = true;
  return input;
}


std::string JSONtoSTRING(std::ifstream& input) { //Вот эта функция работает супер круто

  if (!input.is_open()) {
    return "";
  }
  else {
    std::string Ranswer, answer;
    while (getline(input, answer))
    {
      if (answer.size() == 0) continue;
      //std::cout << answer << std::endl;
      answer.erase(0, 1);
      if (answer.size() == 0) continue;
      if (answer == "4") { //Вот этот способ прерывания потока не работает
        std::cout << "Отправка файлов прервана " << std::endl;
        break;
        
      }
      for (int i = 0; i < 3; i++)
      {
        answer.erase(0, answer.find_first_of("\"") + 1);
        //if (answer.size() == 0) continue;
      }
      if (answer.size() == 0) continue;
      else {
        if (answer.find_first_of("\"") != -1) {
          answer.erase(answer.find_first_of("\""), answer.npos);
          //std::cout << answer << std::endl;
        }
      }
      Ranswer += answer;
      Ranswer += " ";
    }
    //std::cout << std::endl << "Размер строки: " << Ranswer.size() << std::endl;
    return Ranswer;
  }
}


void WriteIntoDatabase(std::string login, std::string value) {
  std::string filename = login + ".txt";
  std::ofstream out;
  out.open(filename, std::ios::app);
  out << value << std::endl;
}


void SOCKET_CONNECTION::WaitingForABreak() {
  char* msg = new char[1];
  recv(Sock, msg, 1, NULL);
  int choice = *msg - '0';
  if (msg == "4") {
    Sending_Flag = 0;
    Sorting();
  }
}


void SOCKET_CONNECTION::ClientHandler(bool& Rec_Flag) {

  while (Rec_Flag) {
    Sleep(790);

    int msg_size;
    int i = 8;
    std::string DB = login + ".json";
    std::ofstream Database_1(DB);

    while (i > 0) {
      //recv(Sock, (char*)&msg_size, sizeof(int), NULL);
      //if (msg_size < 2) break;
      char msg[60] = {};
      //msg[msg_size] = '\0';
      recv(Sock, msg, sizeof(msg), NULL);
      //std::cout << msg << std::endl;
      Sleep(30);
      Database_1 << msg << std::endl; //Записали сообщение
      i--;
    }
    if (Sending_Flag == 0) break;
    std::ifstream Database_2(DB);
    std::string Aw = JSONtoSTRING(Database_2);
    //std::cout << "Загружаемая строка: " << Aw << std::endl;
    if (Aw.size() == 38) WriteIntoDatabase(login, Aw); //Это условие отсекает неровные строки (а почему возникают неровные строки?)
  }
}


bool find_word(const std::string filename, const std::string word) //Бинарничек для нахождения слова
{
  std::ifstream file(filename);
  std::string tmp;
  while (file >> tmp)
  {
    if (word == tmp)
      return true;
  }
  file.close();
  return false;
}


std::string TOString(char *ptr, int length)
{
  std::string answer = {};
  for (int i = 0; i < length; i++) {
    answer += ptr[i];

  }

  return answer;
}


std::string FormId() {
  std::cout << "Формирование  id: " << std::endl;
  std::ifstream f;
  f.open("logusers.txt");
  if (!f.is_open())
  {
    std::cerr << "Error open file" << std::endl;
  }

  std::string line;
  int lines_count = 0;
  while (getline(f, line))
  {
    if (!std::all_of(line.begin(), line.end(), isspace)) ++lines_count;
  }

  std::cout << "Not empty lines - " << lines_count << std::endl;
  lines_count++;
  std::string answer = {};
  if (lines_count < 10) answer += "0";
  answer += std::to_string(lines_count);

  std::cout << "Сформирован id: " << lines_count << std::endl;
  return answer;
}


void AddNewUser(std::string id, std::string login, std::string pass) {
  std::ofstream out;
  out.open("logusers.txt", std::ios::app);
  std::cout << "Сформирована запись: " << std::endl;
  std::cout << "id=" << id << " " << login << " pass=" << pass << "\n";
  out << "id=" << id << " " << login << " pass=" << pass << "\n";
  out.close();
}


bool IsRegistered(std::string login, std::string pass, std::string filename) {
  std::string example1 = {};
  std::string example2 = {};
  example1 += "login=";
  example1 += login;
  example2 += "pass=";
  example2 += pass;

  std::cout << "Строка выглядит так: " << example1 << " " << example2 << std::endl;

  if (find_word(filename, example1) && find_word(filename, example2)) return true;
  else return false;
}


void SOCKET_CONNECTION::catch_aut() {
  char blogin[10] = {};
  recv(Sock, blogin, sizeof(blogin), NULL);
  recv(Sock, blogin, sizeof(blogin), NULL);
  std::cout << blogin << std::endl;

  char* now = new char[6];
  std::string Looogin;
  for (int i = 0; i < 6; i++) {
    now[i] = blogin[i];
    Looogin += now[i];
  }

  std::cout << Looogin << std::endl;
  login = Looogin;

  char pass[10] = {};
  recv(Sock, pass, sizeof(pass), NULL);
  recv(Sock, pass, sizeof(pass), NULL);
  std::cout << pass << std::endl;

  char* noow = new char[6];
  std::string Paass;
  for (int i = 0; i < 6; i++) {
    noow[i] = pass[i];
    Paass += noow[i];
  }

  std::cout << Paass << std::endl;
  password = Paass;

  if (IsRegistered(Looogin, Paass, "logusers.txt")) {
    std::cout << "Такой пользователь зарегистрирован" << std::endl;
    const char msg2[] = "1";
    send(Sock, msg2, sizeof(msg2), NULL);
    std::cout << "Прошел проверку" << std::endl;
    Sorting();
  }
  else {
    const char errmsg4[] = "Сервер: Не удалось авторизоваться\n";
    send(Sock, errmsg4, sizeof(errmsg4), NULL);
    std::cout << "ne Прошел проверку" << std::endl;
    Sorting();
  }
}


void SOCKET_CONNECTION::catch_reg() {

  char login[10] = {};
  recv(Sock, login, sizeof(login), NULL);
  recv(Sock, login, sizeof(login), NULL); //Почему-то приём идёт именно здесь, а не в первом вызове
  //std::cout << login << std::endl;
  char* now = new char[6];
  std::string Looogin = "login=";
  for (int i = 0; i < 6; i++) {
    now[i] = login[i];
    Looogin += now[i];
  }
  //std::cout << Looogin << std::endl;

  if (find_word("logusers.txt", Looogin)) {
    const char errmsg1[] = "Сервер: Пользователь с таким логином уже есть, давай по-новой\n";
    send(Sock, errmsg1, sizeof(errmsg1), NULL);
    catch_reg();
  }
  else {
    const char msg2[] = "1";
    send(Sock, msg2, sizeof(msg2), NULL);

    char pass[10];
    recv(Sock, pass, sizeof(pass), NULL);
    //recv(Sock, pass, sizeof(pass), NULL);
    //std::cout << pass << std::endl;

    char* noow = new char[6];
    std::string Paass;
    for (int i = 0; i < 6; i++) {
      noow[i] = pass[i];
      Paass += noow[i];
    }

    //std::cout << Paass << std::endl;

    AddNewUser(FormId(), Looogin, Paass);

    std::string alittlebitlonger = "login=";
    alittlebitlonger += Looogin;

    if (find_word("logusers.txt", alittlebitlonger)) {
      const char msg3[] = "1";
      send(Sock, msg3, sizeof(msg3), NULL);
      std::cout << "Процедура регистрации завершена успешно" << std::endl;
      Sorting();
    }
    else {
      const char errmsg2[] = "0";
      send(Sock, errmsg2, sizeof(errmsg2), NULL);
      Sorting();
    }
  }
}


void SOCKET_CONNECTION::catch_files() {
  std::cout << "Начинаю приём файлов" << std::endl;
  std::cout << "Прием файлов будет продолжаться, пока не переключишь" << std::endl;
  CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, &Sending_Flag, NULL, NULL);
  //Sorting();
}


void SOCKET_CONNECTION::Sorting() {
  char* msg = new char[1];
  recv(Sock, msg, 1, NULL);
  int choice = *msg - '0';

  switch (choice) {
  case 1: //Сообщения этого типа передаются при регистрации
    std::cout << "Начата процедура регистрации" << std::endl;
    catch_reg();
    break;
  case 2: //Сообщения этого типа передаются при автoризации
    std::cout << "Начата процедура аутентификации" << std::endl;
    catch_aut();
    break;
  case 3: //Сообщения этого типа передаются при подготовке к передаче данных
  {
    if (login.length() == 0) {
      const char errmsg5[] = "Сервер: Пользователь не авторизован\n";
      send(Sock, errmsg5, sizeof(errmsg5), NULL);
      Sorting();
    }
    Sending_Flag = 1;
    catch_files();
    break;
  }
  case 4: //Сообщения этого типа передаются для окончания приема файлов
  {
    Sending_Flag = 0;
    std::cout << "Прием файлов прекращен" << std::endl;
    Sorting();
    break;
  }
  case 6: //Клиент выходит из программы, его сокет закрывается
  {
    closesocket(Sock);
    break;
  }
  case 9:
  {
    std::cout << "Пользователь разлогинился" << std::endl;
    Escape_Flag = 1;
    Sorting();
    break;
  }
  default: //Для всего остального
    std::cout << "Фигню передал какую-то" << std::endl;
    Sorting();
    break;
  }
}
