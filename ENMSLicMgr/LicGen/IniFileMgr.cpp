#include "pch.h"
#include "licenseerror.h"

#include <iostream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>


using namespace std;

#include "IniFileMgr.h"
#include "SoftLicenseMgr.h"



string CIniFilmeMgr::readIniEntry(const string& filePath, const string& section, const string& key)
{
    ifstream iniFile(filePath);
    string line;
    string targetSection = "[" + section + "]";
    bool sectionFound = false;

    while (getline(iniFile, line)) {
        // Check for section
        if (!line.empty() && line == targetSection) {
            sectionFound = true;
            continue;
        }

        // Read key-value pairs within the section
        if (sectionFound && line.find('=') != string::npos) {
            istringstream iss(line);
            string currentKey;
            string value;
            getline(iss, currentKey, '=');
            getline(iss, value);
            // Remove leading and trailing whitespaces from key
            currentKey.erase(0, currentKey.find_first_not_of(" \t"));
            currentKey.erase(currentKey.find_last_not_of(" \t") + 1);
            if (currentKey == key) {
                return value;
            }
        }

        // Check for next section or end of file
        if (sectionFound && line.find('[') != string::npos) {
            break; // next section encountered
        }
    }

    return ""; // Key not found
}

// Function to write an entry to an INI file
void CIniFilmeMgr::writeIniEntry(const string& filePath, const string& section, const string& key, const string& value)
{
    ifstream inFile(filePath);
    ofstream outFile(filePath + ".temp");

    string currentSection;
    bool sectionFound = false;
    bool keyFound = false;

    string targetSection = "[" + section + "]";
    string line;

    while (getline(inFile, line)) {
        if (!sectionFound && line == targetSection) {
            sectionFound = true;
            currentSection = line;
            outFile << line << '\n';
            continue;
        }

        if (sectionFound && line.find('[') != string::npos) {
            sectionFound = false;
        }

        if (sectionFound && !keyFound && line.find('=') != string::npos) {
            istringstream iss(line);
            string currentKey;
            getline(iss, currentKey, '=');
            // Remove leading and trailing whitespaces from key
            currentKey.erase(0, currentKey.find_first_not_of(" \t"));
            currentKey.erase(currentKey.find_last_not_of(" \t") + 1);
            if (currentKey == key) {
                outFile << key << "=" << value << '\n';
                keyFound = true;
                continue;
            }
        }

        outFile << line << '\n';
    }

    if (!keyFound) {
        if (!sectionFound) {
            outFile << '\n';
            outFile << targetSection << '\n';
        }
        outFile << key << "=" << value << '\n';
    }

    inFile.close();
    outFile.close();

    // Replace original file with the temporary file
    remove(filePath.c_str());
    rename((filePath + ".temp").c_str(), filePath.c_str());
}



int CIniFilmeMgr::ProtectSection(const string& filePath, const string& Thesection, const string& key, const string& salt)
{
/*    string szpath(filePath);
   
    int error = ERROR_NOERROR;

    map<string, map<string, string>> iniData = parseIniFile(Thesection, szpath, error);

    if (error != ERROR_NOERROR)
        return error;

    string szHash;

    for (const auto& section : iniData)
    {
        string asection = section.first;
        if (asection.compare(Thesection))
            continue;

        szHash = asection + salt;

         for (const auto& entry : section.second)
        {
            string aentry = entry.first;
            if (aentry.compare(key) == 0)
                continue;
            string avalue = entry.second;
            szHash = szHash + aentry + avalue;
        }
    }

    */
    return ERROR_NOERROR;
}



