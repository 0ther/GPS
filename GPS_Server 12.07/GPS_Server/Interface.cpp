#include "pch.h"
#include "Interface.h"


bool ADMIN_INTERFACE::Logged = 0;
bool ADMIN_INTERFACE::ExitFlag = 0;
SOCKET_CONNECTION CON1 = SOCKET_CONNECTION();


void ADMIN_INTERFACE::Autorise() {
  std::cout << "������ ��������� �����������" << std::endl;
  std::cout << "������� �����: " << std::endl;
  std::string login;
  std::cin >> login;
  std::cout << "������� ������: " << std::endl;
  std::string pass;
  std::cin >> pass;
  if (IsRegistered(login, pass, "logadmins.txt")) {
    std::cout << "������������� ������� �����������" << std::endl;
    Sleep(1000);
    Logged = 1;
    //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Interface, &Logged, NULL, NULL);
    Interface(Logged);
  }
  else {
    std::cout << "����������� �� ���������, ��" << std::endl;
    Sleep(1000);
    LoginScreen();
  }
};


void ADMIN_INTERFACE::UsersList() {
  std::cout << "������ ������������� �������" << std::endl;
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
  std::cout << "������� ����� ������������" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;
  std::cout << "���� ��� ���:" << templ << std::endl;
  if (find_word("logusers.txt", templ)) {
    login += ".txt";
    SeeFileContent(login);
  }
  else {
    std::cout << "������ ������������ ���" << std::endl;
    Sleep(1000);
    Interface(Logged);
  }
};


void ADMIN_INTERFACE::CleanDB() {
  std::cout << "������� ����� ������������" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;
  std::cout << "���� ��� ���:" << templ << std::endl;

  if (find_word("logusers.txt", templ)) {
    login += ".txt";
    std::fstream in(login, std::ios::out);
    in.close();
    Sleep(2000);
    Interface(Logged);
  }
  else {
    std::cout << "������ ������������ ���" << std::endl;
    Sleep(1000);
    Interface(Logged);
  }
};


void ADMIN_INTERFACE::AddUser() {
  std::cout << "������ ��������� ���������� ������������" << std::endl;
  std::cout << "������� ����� ������������" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;
  std::cout << "���� ��� ���:" << templ << std::endl;

  if (find_word("logusers.txt", templ)) {
    std::cout << "������: ������������ � ����� ������� ��� ����" << std::endl;
    AddUser();
  }
  else {
    std::cout << "������� ������ ������������" << std::endl;
    std::string pass;
    std::cin >> pass;
    std::cout << "���������� ������..." << std::endl;
    AddNewUser(FormId(), templ, pass);

    if (find_word("logusers.txt", templ)) {
      std::cout << "������ ������� ��������" << std::endl;
      Sleep(1000);
      Interface(Logged);
    }
    else {
      std::cout << "������: �� ������� �������� ������" << std::endl;
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
      std::cout << "����� ������ � " << lines_count << " ������" << std::endl;
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
    if (lines_count == number) { //������ ��������� �
      isdeleted = 1;
    }
    else { //� ��� ��������� ������� ������� � ����
      if (isdeleted) { //����� ��������� id, ������ ��� � ������� ���������� ��������
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

  //����� � ��������� ������� ����� � ������� � ��������������� ����� ������
  if (remove("logusers.txt")) {
    std::cout << "������: �� ������� ������� ����" << std::endl;
    Sleep(1000);
  }
  std::cout << "�������� ���������" << std::endl;
  int result;
  char oldname[] = "out.txt";
  char newname[] = "logusers.txt";
  result = rename(oldname, newname);
  if (result == 0) {
    std::cout << "�������������� ������� ���������" << std::endl;
    Sleep(1000);
  }
  else {
    std::cout << "������: �� ������� ������������� ����" << std::endl;
    Sleep(1000);
  }
};


void ADMIN_INTERFACE::DeleteUser() {
//������� ������������
  /*
  ������� ���� � �������� ��������
  ����� ������ � ������ ������� �������
  ������� ��� ������
  �������� ���� ���� �������������, ������� ����, �� 1
  ������� ���� � �������� ��������
  */
  std::cout << "������� ����� ������������" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;

  if (find_word("logusers.txt", templ)) {
    int line = LineOfLogin("logusers.txt", login);
    std::cout << "���������� � ��������" << std::endl;
    DeleteLineWithIdChanging("logusers.txt", line);
    Sleep(1000);
  }
  else {
    std::cout << "������: ������������ � ����� ������� ��� � �������" << std::endl;
    Sleep(1000);
    DeleteUser();
  }
};


void ADMIN_INTERFACE::Interface(bool& Flag) {
  while (Flag) {
    system("cls");
    std::cout << "������� ��������������, ������ 1.0" << std::endl;
    std::cout << "��������� �����:" << std::endl;
    std::cout << "1. ��������� ������" << std::endl;
    std::cout << "2. ������� ������ �������������" << std::endl;
    std::cout << "3. ������� ������� ������ �������������" << std::endl;
    std::cout << "4. ����������� ������� ����������� ������������" << std::endl;
    std::cout << "5. �������� ������� ����������� ������������" << std::endl;
    std::cout << "6. �������� ������������" << std::endl;
    std::cout << "7. ������� ������������" << std::endl;
    std::cout << "8. �������������" << std::endl;
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
      std::cout << "������ GPS-�������� ������������ ���! ���� �������?" << std::endl;
      std::cout << "�������� ��������� �����: " << std::endl;
      std::cout << "1. �������������� � �������" << std::endl;
      std::cout << "2. �������� �������� ������" << std::endl;
      std::cout << "3. ����� �� ���������" << std::endl;
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
        std::cout << "�������� ���������� ����" << std::endl;
        Sleep(1000);
        break;
      }
    }
  }
};


void ADMIN_INTERFACE::Refresh() {//����� �� ��� .txt ����� ������������� � �������� ��, ���� ����� ��������
  
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
    getline(infile, mystr); //������ ������� � ��������
    if (mystr == "") break;
    mystr.erase(0, 12);
    mystr.erase(6, mystr.npos);
    mystr += ".txt";
    std::ifstream infile2(mystr);
    if (!infile2.is_open()) {
      std::cout << "������: �� ������� ������� ����" << std::endl;
    }
    else {
      while (!infile2.eof()) { //������� ������ ������ � ������ � ��, � ������
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

