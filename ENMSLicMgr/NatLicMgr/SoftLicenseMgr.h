#pragma once

#include <string>

using namespace std;

#define ERROR_NOERROR          1
#define ERROR_FILE_DONT_EXIST  100
#define ERROR_FILEINTEGRITY    101
#define ERROR_INVALIDFILE      102
#define ERROR_FINGERPRINT       150
#define ERROR_PARTIAL_FINGERPRINT   151
#define ERROR_RETRIEVING_FINGERPRINt   155
#define ERROR_MACHINE          160
#define ERROR_DOMAIN           161
#define ERROR_CPU              162


#define ERROR_LICENSE_EXPIRED  200
#define ERROR_ACTIVATION_TIME  210
#define ERROR_BLANCK_LICENSE   300


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

const string g_S1 = "S1";
const string g_S2 = "S2";
const string g_T1 = "T1";
const string g_E4 = "E4";
const string g_F1 = "F1";
const string g_OZ = "OZ";


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
    int m_TemporaryLicenseDuration = 0;
    int m_expirationYear = 0;
    int m_expirationMonth = 0;
    int m_expirationDay = 0;
    int m_activationYear = 0;
    int m_activationMonth = 0;
    int m_activationDay = 0;
    int m_TemporaryLicense = 0;
    string m_signature = "";
};

class CSoftLicenseMgr
{
public:

    CSoftLicenseMgr() {};
    string GetEncryptedString(const string& input, int key, string salt, int randomlength);
    string GetHash(int Tyoe, string input);
    int CheckFile(const string& filePath, const string& fileOutput);
    int CheckoutLicense(const string& filePath, const string& fileouput, _LicOptions& options, bool version1);
    int CheckMachineFingerPrint(const string& szfingerprint, const string& szmachine, const string& szdomain, const string& szCPU);
    int CompareFingerPrint(const string& szfingerprint);
    string getMachineName();
    string getComputerDomain();
    string getCPU();
    string GetMAcAdress(int& error);
    int ExtractOptions(const string& szoptions, int& meters, int& users, int& connections, int& product, int& update, int& version, int& tempo, int& duration, int& expYear, int& expMonth, int& expDay, int& startYear, int& startMonth, int& startDay);
    int WriteToFile(string szFile, string szContent);
    string GetOptions();
    string GetProcessDirectory();

    _LicOptions m_licfeatures;
};

