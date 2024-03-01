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
#include <format>
#include <Windows.h>


#include <locale>
#include <codecvt>

using namespace std;

#include "inifilemgr.h"
#include "SoftLicenseMgr.h"
#include "EncryptionMgr.h"


string CSoftLicenseMgr::GetEncryptedString(const string& input, int key, string salt, int randomlength)
{
    CustomCypher cipher(key, salt, randomlength);
    string szReturn = cipher.encrypt(input);
    return szReturn;
}

string CSoftLicenseMgr::GetHash(int Tyoe, string input)
{
    CustomCypher    acypher;
    return acypher.HashInput(input);
}

string CSoftLicenseMgr::GetRandomGUId()
{
    GUID guid;
    CoCreateGuid(&guid);

    wchar_t guidString[39]; // String representation of GUID will be 38 characters + NULL terminator
    if (StringFromGUID2(guid, guidString, sizeof(guidString)) == 0) 
    {
        return "";
    }

    wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    string sresuklt = converter.to_bytes(guidString);
  
    string szreturn = GetEncryptedString(sresuklt, 4, "guid", 6);
 
    return szreturn;
}

string CSoftLicenseMgr::GetOptions()
{
    string szReturn ;
    szReturn = "lic;";

    string szDelimiteur = ";";

    stringstream ss;

    ss << "lic;";
    ss << m_licfeatures.m_version;
    ss << szDelimiteur;
    ss << m_licfeatures.m_meters;
    ss << szDelimiteur;
    ss << m_licfeatures.m_users;
    ss << szDelimiteur;
    ss << m_licfeatures.m_connections;
    ss << szDelimiteur;
    ss << m_licfeatures.m_update;
    ss << szDelimiteur;
    ss << m_licfeatures.m_product;
    ss << szDelimiteur;

    string szreturn = GetEncryptedString(ss.str(), 6, "options", 8);
    return szreturn;
}


int CSoftLicenseMgr::GenerateLicenseFile(const string& filePath)
{
    string szpath = filePath + "\\" + "lic-" + m_licfeatures.m_serial + ".dat";

    m_licfeatures.m_guid = GetRandomGUId();
    m_licfeatures.m_options = GetOptions();

    ofstream iniFile(szpath);

    // Check if the file opened successfully
    if (!iniFile.is_open())
    {
        return ERROR_CREATEFILE;
    }

    string szwork = "[" + g_SectionGeneral + "]";
    iniFile << szwork << std::endl;


    stringstream ssHash;
    ssHash << szwork;
    ssHash << g_SectionGeneral; 
 
    iniFile << g_licenseNumber << "=" << m_licfeatures.m_serial << endl;

    ssHash << g_licenseNumber;
    ssHash << m_licfeatures.m_serial;

    iniFile << g_version << "=" << m_licfeatures.m_version << endl;

    ssHash << g_version;
    ssHash << m_licfeatures.m_version;

    iniFile << g_meters << "=" << m_licfeatures.m_meters << endl;

    ssHash << g_meters;
    ssHash << m_licfeatures.m_meters;

    iniFile << g_users << "=" << m_licfeatures.m_users << endl;

    ssHash << g_users;
    ssHash << m_licfeatures.m_users;

    iniFile << g_connections << "=" << m_licfeatures.m_connections << endl;

    ssHash << g_connections;
    ssHash << m_licfeatures.m_connections;

    iniFile << g_product << "=" << m_licfeatures.m_product << endl;

    ssHash << g_product;
    ssHash << m_licfeatures.m_product;

    iniFile << g_update << "=" << m_licfeatures.m_update << endl;

    ssHash << g_update;
    ssHash << m_licfeatures.m_update;

    iniFile << g_activation << "=" << m_licfeatures.m_activation << endl;
    
    ssHash << g_activation;
    ssHash << m_licfeatures.m_activation;

   
    string szhashed = GetHash(1, ssHash.str());
    iniFile << g_key << "=" << szhashed << endl;


    szwork = "[" + g_SectionFeatures + "]";
    iniFile << szwork << std::endl;

    szwork = "";
    iniFile << g_S1 << "=" << m_licfeatures.m_guid << endl;
    iniFile << g_T1 << "=" << m_licfeatures.m_options << endl;
    iniFile << g_S2 << "=" << m_licfeatures.m_signature << endl;
    iniFile << g_E4 << "=" << szwork << endl;
    iniFile << g_F1 << "=" << szwork << endl;
    iniFile << g_key << "=" << szwork << endl;


    iniFile.flush();
    iniFile.close();

    std::cout << "INI file created successfully." << std::endl;
    return ERROR_NOERROR;
}


   