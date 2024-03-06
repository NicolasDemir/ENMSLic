#include "pch.h"
#include "CLicMgr.h"


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>
#include <fstream> // For file handling
#include <vector>

using namespace std;

#include "toolbox.h"
#include "EncryptionMgr.h"

void logStringToFile(const string& message, const string& filename)
{
	ofstream outputFile(filename, ios_base::app); // Open file in append mode
	if (outputFile.is_open()) 
	{
		outputFile << message << endl; // Write message to file
		outputFile.close(); // Close the file
		cout << "Message logged to file successfully." << endl;
	}
	else
	{
		cerr << "Unable to open file: " << filename << endl;
	}
}



int CheckIntegrity(const string& filePath)
{
	int nError = ERROR_NOERROR;

	std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> iniData = parseIniFile(filePath, nError);

	if (nError != ERROR_NOERROR)
		return nError;

	stringstream ssHash;
	
	// Display parsed data
	for (const auto& section : iniData)
	{
		ssHash.str("");

		string szwork = "[" + section.first + "]";
		ssHash << szwork;
		ssHash << section.first;

		for (const auto& entry : section.second)
		{
			string aentry = entry.first;
			string avalue = entry.second;

			if (entry.first != "key")
			{
				ssHash << aentry;
				ssHash << avalue;
			}
			else
			{
				CustomCypher    acypher;
				string szHash = ssHash.str();
				string sz = acypher.HashInput(szHash);
				if (sz != avalue)
				{
					return ERROR_FILEINTEGRITY;
				}
			}
		}
	}

	return ERROR_NOERROR;
}


extern "C" __declspec(dllexport) int CheckOut(char* query, char* presponse, char* serial, int* meters, int* users, int* connections, int* update, int* product, int* version, char* filepath)
{
	std::string filename = "logfile.txt";
	std::remove(filename.c_str());

	string szpath(filepath);
	int error = ERROR_NOERROR;

	error = CheckIntegrity(szpath);

	if (error != ERROR_NOERROR)
		return error;

	vector<pair<string, vector<pair<string, string>>>> iniData = parseIniFile(szpath, error);

	if (error != ERROR_NOERROR)
		return error;

	CustomCypher    acypher;
	string str(query);

	for (const auto& section : iniData)
	{
		string asection = section.first;
		for (const auto& entry : section.second)
		{
			string aentry = entry.first;
			if (aentry.compare(g_entryMeters) == 0)
			{
				string avalue = entry.second;
				*meters = stoi(avalue);
			}
			else if (aentry.compare(g_entryUsers) == 0)
			{
				string avalue = entry.second;
				*users = stoi(avalue);
			}
			else if (aentry.compare(g_entryConnections) == 0)
			{
				string avalue = entry.second;
				*connections = stoi(avalue);
			}
			else if (aentry.compare(g_entryVersion) == 0)
			{
				string avalue = entry.second;
				*version = stoi(avalue);
			}
			else if (aentry.compare(g_entryProduct) == 0)
			{
				string avalue = entry.second;
				*product = stoi(avalue);
			}
			else if (aentry.compare(g_entrySerial) == 0)
			{
				string avalue = entry.second;
				strcpy_s(serial, avalue.length() + 1, avalue.c_str());
			}
			else if (aentry.compare(g_entryUpdate) == 0)
			{
				string avalue = entry.second;
				*update = stoi(avalue);
			}
		}
	}

	string szwork;

	


	/*for (const auto& section : iniData)
   {
		stringstream ssHash;

		string asection = section.first;
		ssHash << "[" + asection + "]";
		ssHash << asection;
		logStringToFile("section: " + asection, filename);
		string szkey;
		for (const auto& entry : section.second)
		{
			string aentry = entry.first;
			string avalue = entry.second;

			if (entry.first != "key")
			{
				ssHash << aentry;
				ssHash << avalue;
		
			}
			else
			{
				szkey = avalue;
			}
		}

		int ierr = 0;

		string szHash = ssHash.str();
		string sz = acypher.HashInput(szHash);
		logStringToFile("ssHash stream: " + szHash, filename);
		logStringToFile("ssHash courant: " + sz, filename);
		logStringToFile("value hash: " + szkey, filename);

		if (sz.compare(szkey) != 0)
		{
			*meters = -1200;
		}
		else
		{
			*meters = 1400;
		}
	
		ssHash.clear();
	}*/

	return 1;

/*	if (error != 0)
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

	string sz = acypher.HashInput(str);
	strcpy_s(presponse, sz.length() + 1, sz.c_str());*/
	return 1;
}

