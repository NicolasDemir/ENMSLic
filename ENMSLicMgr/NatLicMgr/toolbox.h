
#ifndef FIRST_MODULE_H
#define FIRST_MODULE_H

#define ERROR_NOERROR          1
#define ERROR_FILE_DONT_EXIST  100
#define ERROR_FILEINTEGRITY    101


const string g_entryMeters = "meters";
const string g_entryUsers = "users";
const string g_entryConnections= "connections";
const string g_entrySerial = "serial";
const string g_entryVersion = "version";
const string g_entryProduct = "product";
const string g_entryUpdate= "update";
const string g_entryDate = "activationdate";
const string g_entryMachine = "machine";
const string g_sectionGeneral= "General";
const string g_sectionFeatures= "Features";
const string g_entryKey = "key";


vector<pair<string, vector<pair<string, string>>>> parseIniFile(const string& filename, int& error);
string trim(const string& str);


#endif // FIRST_MODULE_H

