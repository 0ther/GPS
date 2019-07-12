#include "pch.h"
#include "Movingman.h"


MOVINGMAN::MOVINGMAN(float A, float B) {
  Latitude = A;
  Longitude = B;
};


void MOVINGMAN::Move() {
  SYSTEMTIME st;
  GetLocalTime(&st);
  if ((st.wMinute*st.wSecond) % 3 == 0) Latitude += 0.0001;
  else Longitude += 0.0001;
};


float MOVINGMAN::Get_Lat() { return Latitude; };


float MOVINGMAN::Get_Lon() { return Longitude; };