#pragma once

enum MESSAGESFROMSERVER { ERR, GOODCONNECT, GOODAUTH, PROVE, ECHO }; //���� ���������, ���������� � �������
enum MESSAGETYPES { SERVICE, LOG, NMEA }; //��� ��� �� ������
enum SERVICETYPES { BEGIN, STARTSEND, STOPSEND, End }; //��� ���� ��������� ���������
enum INTMSGTYPES { ERRTYPE, SUCREG, SUCCON, BADCON, LOGOUT, END }; //��� ��� � ������������


class DATAMODEL {
public:
  void ParseJSON(int);

};