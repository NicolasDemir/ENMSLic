
#ifndef FIRST_MODULE_H
#define FIRST_MODULE_H

#define ERROR_FILE_DONT_EXIST  101


const string g_meters = "meters";
const string g_users = "users";
const string g_connections= "connections";
const string g_serial = "licenseNumber";
const string g_version = "version";
const string g_product = "product";
const string g_update= "update";


// Declare global functions
map<string, map<string, string>> parseIniFile(const string section, const string& filename, int& error);
std::string sha256(const std::string& input);

#endif // FIRST_MODULE_H

