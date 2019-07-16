#pragma once

#include <iostream>
#include <string>

using namespace std;

template <typename T> class COLUMN {
private:
	string Name;
	T* values;
public:
	COLUMN() {};
	COLUMN(string Name, T* values) : Name(Name), values(values);

};










class DATABASE_TABLE {
private:
	string* Column_Names;
public:


};