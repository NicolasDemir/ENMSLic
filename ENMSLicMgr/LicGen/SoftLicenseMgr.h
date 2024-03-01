#pragma once


#include <string>

using namespace std;

struct LicenseFeatures
{
    string m_serial;
    string m_activation;
    string m_guid;
    string m_options;
    int  m_users = 0;
    int  m_meters = 0;
    int  m_connections = 0;
    int m_update = 0;
    int m_product = 0;
    int m_version = 0;

    string m_signature = "";
};

class CIniFilmeMgr;

class CSoftLicenseMgr
{
public : 

    CSoftLicenseMgr() {};
    int GenerateLicenseFile(const string& fileFolder);
    string GetEncryptedString(const string& input, int key, string salt, int randomlength);
    string GetHash(int Tyoe, string input);

    string GetRandomGUId();
    string GetOptions();

    CIniFilmeMgr    m_inifleMgr;
    LicenseFeatures m_licfeatures;
  


};

