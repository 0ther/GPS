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
	void StartGenerating();
	void StopGenerating();
	static void Generate_RMC(bool&);
};