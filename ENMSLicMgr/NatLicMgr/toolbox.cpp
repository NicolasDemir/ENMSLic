#include "pch.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <vector>


using namespace std;

#include "toolbox.h"

// Function to trim leading and trailing whitespaces
string trim(const string& str)
{
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	if (first == string::npos || last == string::npos)
		return "";
	return str.substr(first, last - first + 1);
}

// Function to parse INI file
vector<pair<string, vector<pair<string, string>>>> parseIniFile(const string& filename, int& error)
{
	error = ERROR_NOERROR;

	ifstream file(filename);

	vector<pair<string, vector<pair<string, string>>>> iniData;

	if (file.bad())
	{
		error = ERROR_FILE_DONT_EXIST;
		return iniData;
	}

	string currentSection;

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			// Trim leading and trailing whitespaces
			line = trim(line);

			// Ignore empty lines and comments
			if (line.empty() || line[0] == ';' || line[0] == '#')
				continue;

			// Check for section
			if (line[0] == '[' && line.back() == ']') {
				currentSection = line.substr(1, line.size() - 2);
				iniData.push_back({ currentSection, vector<pair<string, string>>() });
			}
			// Check for key-value pair
			else {
				size_t pos = line.find('=');
				if (pos != string::npos) {
					string key = trim(line.substr(0, pos));
					string value = trim(line.substr(pos + 1));
					if (!currentSection.empty()) {
						iniData.back().second.push_back({ key, value });
					}
				}
			}
		}
		file.close();
	}
	else
	{
		error = ERROR_FILE_DONT_EXIST;
	}
	return iniData;
}