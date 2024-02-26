#include "pch.h"
#include "CLicMgr.h"


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>

using namespace std;

#include "toolbox.h"

extern "C" __declspec(dllexport) int CheckOut(char* query, char* presponse, char* serial, int* meters, int* users, int* connections, int* update, int* product, int* version, char* filepath)
{
	string str(query);

	string szpath(filepath);
	int error = 0;
	map<string, map<string, string>> iniData = parseIniFile("", szpath, error);

	if (error != 0)
		return error;

	for (const auto& section : iniData)
	{
		string asection = section.first;
		for (const auto& entry : section.second)
		{
			string aentry = entry.first;
			if (aentry.compare(g_meters) == 0)
			{
				string avalue = entry.second;
				*meters = stoi(avalue);
			}
			else if (aentry.compare(g_users) == 0)
			{
				string avalue = entry.second;
				*users = stoi(avalue);
			}
			else if (aentry.compare(g_connections) == 0)
			{
				string avalue = entry.second;
				*connections = stoi(avalue);
			}
			else if (aentry.compare(g_connections) == 0)
			{
				string avalue = entry.second;
				*connections = stoi(avalue);
			}
			else if (aentry.compare(g_version) == 0)
			{
				string avalue = entry.second;
				*version = stoi(avalue);
			}
			else if (aentry.compare(g_product) == 0)
			{
				string avalue = entry.second;
				*product = stoi(avalue);
			}
			else if (aentry.compare(g_serial) == 0)
			{
				string avalue = entry.second;
				strcpy_s(serial, avalue.length() + 1, avalue.c_str());
			}
			else if (aentry.compare(g_update) == 0)
			{
				string avalue = entry.second;
				*update = stoi(avalue);
			}
		}
	}

	string sz = sha256(str);
	strcpy_s(presponse, sz.length() + 1, sz.c_str());
	return 1;
}

