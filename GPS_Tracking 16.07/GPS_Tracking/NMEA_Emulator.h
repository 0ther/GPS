#pragma once

#include <string>
#include <fstream>
#include <Windows.h>
#include <cmath>
#pragma once

#include <iostream>
#include <Windows.h>
#include "Movingman.h"


struct RMC {
	std::string Preamble, Time, Latitude, Longitude, Date, LaFlag, LoFlag, Sec1, Sec2;
};


class GPS_EMULATOR {
private:
	static bool Gen_Flag;
public:
	GPS_EMULATOR() {};
	void Set_Gen_Flag(bool in) { Gen_Flag = in; }
	void StartGenerating();
	void StopGenerating();
	static void Generate_RMC(bool&);
};


class JSON_TRANSLATOR {
private:
	static bool Tran_Flag;
public:
	JSON_TRANSLATOR() {};
	void Set_Tran_Flag(bool in) { Tran_Flag - in; }
	void StartTranslating();
	void StopTranslating();
	static void Translate_RMC(bool&);
};