#pragma once

enum MESSAGESFROMSERVER { ERR, GOODCONNECT, GOODAUTH, PROVE, ECHO }; //Типы сообщений, приходящих с сервера
enum MESSAGETYPES { SERVICE, LOG, NMEA }; //Это идёт на сервер
enum SERVICETYPES { BEGIN, STARTSEND, STOPSEND, End }; //Это типы сервисных сообщений
enum INTMSGTYPES { ERRTYPE, SUCREG, SUCCON, BADCON, LOGOUT, END }; //Это идёт к пользователю


class DATAMODEL {
public:
  void ParseJSON(int);

};