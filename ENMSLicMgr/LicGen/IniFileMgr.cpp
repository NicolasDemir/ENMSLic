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

map<string, map<string, string>> CIniFilmeMgr::parseIniFile(const string section, const string& filename, int& error)
{
    map<string, map<string, string>> iniData;

    error = 0;

    ifstream file(filename);
    if (!file.is_open()) {
        error = ERROR_FILE_DONT_EXIST;
        return iniData;
    }

    string line;
    string currentSection;

    while (getline(file, line))
    {
        if (section.length() && currentSection.length())
        {
            if (section.compare(currentSection) != 0)
                continue;
        }

        // Skip empty lines and comments
        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        // Check if this line represents a section
        if (line[0] == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.size() - 2);
            continue;
        }

        // Find key-value pairs
        size_t delimiterPos = line.find('=');
        if (delimiterPos == string::npos)
            continue;

        string key = line.substr(0, delimiterPos);
        string value = line.substr(delimiterPos + 1);

        // Remove leading/trailing whitespaces from key and value
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // Store key-value pair in map
        iniData[currentSection][key] = value;
    }

    file.close();
    return iniData;
}

int CIniFilmeMgr::ProtectSection(const string& filePath, const string& Thesection, const string& key, const string& salt)
{
    string szpath(filePath);
   
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

    
    return ERROR_NOERROR;
}

int CIniFilmeMgr::GenerateLicenseFile(const string& szpath, LicenseFeatures licfeatures)
{
    ofstream iniFile(szpath);

    // Check if the file opened successfully
    if (!iniFile.is_open())
    {
        return ERROR_CREATEFILE;
    }

    string szwork = "[" + g_SectionGeneral + "]";
    iniFile << szwork << std::endl;

    iniFile << g_licenseNumber << "=" << licfeatures.m_serial << endl;
    iniFile << g_version << "=" << licfeatures.m_version << endl;
    iniFile << g_meters << "=" << licfeatures.m_meters << endl;
    iniFile << g_users << "=" << licfeatures.m_users << endl;
    iniFile << g_connections << "=" << licfeatures.m_connections << endl;
    iniFile << g_product << "=" << licfeatures.m_product << endl;
    iniFile << g_update << "=" << licfeatures.m_update << endl;
    iniFile << g_activation << "=" << licfeatures.m_activation << endl;
    szwork = "";
    iniFile << g_key << "=" << szwork << endl;

    szwork = "[" + g_SectionFeatures + "]";
    iniFile << szwork << std::endl;

    szwork = "";
    iniFile << g_S1 << "=" << licfeatures.m_guid << endl;
    iniFile << g_T1 << "=" << licfeatures.m_options << endl;
    iniFile << g_S2 << "=" << licfeatures.m_signature << endl;
    iniFile << g_E4 << "=" << szwork << endl;
    iniFile << g_F1 << "=" << szwork << endl;
    iniFile << g_key << "=" << szwork << endl;


    iniFile.flush();
    iniFile.close();

    std::cout << "INI file created successfully." << std::endl;
    return ERROR_NOERROR;
}



