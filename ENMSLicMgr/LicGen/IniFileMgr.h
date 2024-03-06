
#pragma once

#include <iostream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>

const string g_SectionGeneral = "General";
const string g_SectionFeatures = "Features";
const string g_licenseNumber = "serial";
const string g_version = "version";
const string g_meters = "meters";
const string g_users = "users";
const string g_connections = "connections";
const string g_update = "update";
const string g_product = "product";
const string g_activation = "activationdate";
const string g_Machine= "machine";
const string g_key = "key";

const string g_S1 = "S1";
const string g_S2 = "S2";
const string g_T1 = "T1";
const string g_E4 = "E4";
const string g_F1 = "F1";

struct LicenseFeatures;

class CIniFilmeMgr
{
public:
    CIniFilmeMgr() {};
    string readIniEntry(const string& filePath, const string& section, const string& key);
    void writeIniEntry(const string& filePath, const string& section, const string& key, const string& value);
    int ProtectSection(const string& filePath, const string& section, const string& key, const string& salt);

   


};
