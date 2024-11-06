#pragma once


#include <string>

using namespace std;




const string g_entryMeters = "meters";
const string g_entryUsers = "users";
const string g_entryConnections = "connections";
const string g_entrySerial = "serial";
const string g_entryVersion = "version";
const string g_entryProduct = "product";
const string g_entryUpdate = "update";
const string g_entryDate = "activationdate";
const string g_entryMachine = "machine";
const string g_sectionGeneral = "General";
const string g_sectionFeatures = "Features";
const string g_entryKey = "key";


struct _LicOptions
{
    string m_serial;
    string m_activation;
    string m_guid;
    string m_options;
    string m_machine;
    string m_domain;
    string m_cpu;
    int  m_users = 0;
    int  m_meters = 0;
    int  m_connections = 0;
    int m_update = 0;
    int m_product = 0;
    int m_version = 0;

    int m_nTempo = 0;
    int m_nTempoDuration = 0;
    int m_nTempoExpYear = 0;
    int m_nTempoExpMonth = 0;
    int m_nTempoExpDay = 0;
    int m_nTempoStartYear = 0;
    int m_nTempoStartMonth = 0;
    int m_nTempoStartDay = 0;
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
    int CheckFile(const string& filePath, const string& fileOutput);
    int CheckoutLicense(const string& filePath, const string& fileouput, _LicOptions& options);
    int CheckMachineFingerPrint(const string& szfingerprint, const string& szmachine, const string& szdomain, const string& szCPU);
    int CompareFingerPrint(const string& szfingerprint);
    string GetMAcAdress(int& error);
    string getMachineName();
    string getComputerDomain();
    string getCPU();
    bool RetrieveSystemInformation();
    int ExtractOptions(const string& szoptins, int& meters, int& users, int& connections, int& product, int& update, int& version, int& tempo, int& duration, int& expYear, int& expMonth, int& expDay, int& startYear, int& startMonth, int& startDay);
    string GetRandomGUId();
    string GetOptions();

    CIniFilmeMgr    m_inifleMgr;
    _LicOptions m_licfeatures;
  


};

