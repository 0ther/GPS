#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


class MOVINGMAN {
private:
	float Latitude;
	float Longitude;
public:
	MOVINGMAN() {};
	MOVINGMAN(float A, float B);
	void Move();
	float Get_Lat();
	float Get_Lon();
};