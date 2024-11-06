#include "pch.h"



#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>
#include <format>
#include <Windows.h>

#include <vector>
#include <locale>
#include <codecvt>
#include <intrin.h>
#include <chrono>
#include <ctime>


using namespace std;

#include "SoftLicenseMgr.h"
#include "EncryptionMgr.h"

#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

void logStringToFile(const std::string& message, const std::string& filename) {
    std::ofstream outputFile(filename, std::ios_base::app); // Open file in append mode
    if (outputFile.is_open()) {
        outputFile << message << std::endl; // Write message to file
        outputFile.close(); // Close the file
        std::cout << "Message logged to file successfully." << std::endl;
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}


// Function to trim leading and trailing whitespaces
string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    if (first == std::string::npos || last == std::string::npos)
        return "";
    return str.substr(first, last - first + 1);
}

// Function to parse INI file
std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> parseIniFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> iniData;
    std::string currentSection;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // Trim leading and trailing whitespaces
            line = trim(line);

            // Ignore empty lines and comments
            if (line.empty() || line[0] == ';' || line[0] == '#')
                continue;

            // Check for section
            if (line[0] == '[' && line.back() == ']') {
                currentSection = line.substr(1, line.size() - 2);
                iniData.push_back({ currentSection, std::vector<std::pair<std::string, std::string>>() });
            }
            // Check for key-value pair
            else {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = trim(line.substr(0, pos));
                    std::string value = trim(line.substr(pos + 1));
                    if (!currentSection.empty()) {
                        iniData.back().second.push_back({ key, value });
                    }
                }
            }
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    return iniData;
}

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



string CSoftLicenseMgr::GetOptions()
{
    string szReturn;
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

int CSoftLicenseMgr::CheckFile(const string& filePath, const string& fileOutput)
{
    string filename = fileOutput;
    remove(filename.c_str());

    ifstream file(filePath);
    if (!file.good())
        return ERROR_FILE_DONT_EXIST;

    vector<pair<string, vector<pair<string, string>>>> iniData = parseIniFile(filePath);

    stringstream ssHash;

    bool bcheck = FALSE;

    // Display parsed data
    for (const auto& section : iniData)
    {
        ssHash.str("");

        string szwork = "[" + section.first + "]";
        ssHash << szwork;
        ssHash << section.first;

        for (const auto& entry : section.second)
        {
            string aentry = entry.first;
            string avalue = entry.second;

            if (entry.first != "key")
            {
                ssHash << aentry;
                ssHash << avalue;
            }
            else
            {
                CustomCypher    acypher;
                string szHash = ssHash.str();
                string sz = acypher.HashInput(szHash);
                if (sz == avalue)
                {
                    bcheck = true;
                }
                else
                {
                    bcheck = false;
                    return ERROR_FILEINTEGRITY;
                }
            }
        }
    }

    return ERROR_NOERROR;
}


bool CSoftLicenseMgr::RetrieveSystemInformation()
{
    /* m_sMachine = GetTargetName();
     m_sMac = GetMacAddress();
     m_sMacAdvanced = GetMacAddressAdvanced();

     /*CString sDomain = GetDomain();
     CString szCPU = GetCPUID();

     CString SignatureSoft = m_sMachine + _T("|") + m_sVolume;
     CString SignatureHard = m_sMac;

     m_SoftSignature = EncryptedPassword(SignatureSoft);
     m_HardSignature = EncryptedPassword(m_sMac);
     m_HadSignatureAdvanced = EncryptedPassword(m_sMacAdvanced);

     m_CRC = EncryptedPassword(SignatureSoft + m_sMac);

 */
    return TRUE;
}


string CSoftLicenseMgr::GetMAcAdress(int& error)
{
    string szreturn;

    IP_ADAPTER_INFO AdapterInfo[16];
    DWORD dwBufLen = sizeof(AdapterInfo);

    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
    if (dwStatus != ERROR_SUCCESS)
    {
        error = ERROR_RETRIEVING_FINGERPRINt;
        return szreturn;
    }

    std::stringstream macAddresses;
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    while (pAdapterInfo) {
        macAddresses << std::hex << std::uppercase << std::setfill('0');
        for (int i = 0; i < 8/*pAdapterInfo->AddressLength*/; ++i) {
            macAddresses << std::setw(2) << static_cast<int>(pAdapterInfo->Address[i]);
            if (i < 7 /*pAdapterInfo->AddressLength - 1*/)
                macAddresses << ":";
        }
        if (pAdapterInfo->Next)
            macAddresses << "|";
        pAdapterInfo = pAdapterInfo->Next;
    }

    return macAddresses.str();
}


std::string CSoftLicenseMgr::getMachineName()
{
    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

    if (::GetComputerNameW(computerName, &size) == FALSE)
    {
        return "";
    }

    std::wstring wideString(computerName);
    return std::string(wideString.begin(), wideString.end());
}

std::string CSoftLicenseMgr::getComputerDomain()
{
    DWORD bufSize = MAX_PATH;
    wchar_t domainNameBuf[MAX_PATH];
    GetComputerNameEx(ComputerNameDnsDomain, domainNameBuf, &bufSize);

    std::wstring wideString(domainNameBuf);
    return std::string(wideString.begin(), wideString.end());
}

std::string CSoftLicenseMgr::getCPU()
{
    int CPUInfo[4] = { -1 };
    __cpuid(CPUInfo, 0);

    string szreturn = std::to_string(CPUInfo[0]);
    for (int i = 1; i < 4; i++)
    {
        szreturn = szreturn + "-" + std::to_string(CPUInfo[i]);
    }

    return szreturn;
}

int CSoftLicenseMgr::CompareFingerPrint(const string& szfingerprint)
{
    int error = ERROR_NOERROR;

    string sMACAdress = GetMAcAdress(error);

    if (error != ERROR_NOERROR)
        return error;

    //proceed signature
    istringstream currentfingerprint(sMACAdress);
    string token;
    vector<string> tokenCurrent;
    while (getline(currentfingerprint, token, '|'))
    {
        tokenCurrent.push_back(token);
    }

    istringstream givenfingerprint(szfingerprint);
    vector<string> tokenGiven;
    while (getline(givenfingerprint, token, '|'))
    {
        tokenGiven.push_back(token);
    }

    size_t sizegiven = tokenGiven.size();
    size_t sizecurrent = tokenCurrent.size();

    bool bfind = false;

    int nFind = 0;
    int nNotfind = 0;

    for (int i = 0; i < tokenGiven.size(); i++)
    {
        bfind = false;

        for (int j = 0; j < tokenCurrent.size(); j++)
        {
            if (tokenCurrent[j].compare(tokenGiven[i]) == 0)
            {
                bfind = true;
                nFind += 1;
                break;
            }
        }

        if (!bfind)
        {
            nNotfind += 1;
        }
    }

    if (nFind == 0)
        return ERROR_FINGERPRINT;

    if(nNotfind == 0)
        return ERROR_NOERROR;

    if (nNotfind > nFind + 2)
        return ERROR_FINGERPRINT;

    return ERROR_PARTIAL_FINGERPRINT;
}

int CSoftLicenseMgr::ExtractOptions(const string& szoptions, int& meters, int& users, int& connections, int& product, int& update, int& version, int& tempo, int& duration, int& expYear, int& expMonth,int& expDay, int& startYear, int& startMonth, int& startDay)
{
    //proceed signature
    istringstream streamoptions(szoptions);
    string token;
    vector<string> tokenoptions;
    while (getline(streamoptions, token, ';'))
    {
        tokenoptions.push_back(token);
    }

    size_t s = tokenoptions.size();
    if (s < 7)
        return ERROR_INVALIDFILE;

    try
    {
        version = ::atoi(tokenoptions[1].c_str());
        meters = ::atoi(tokenoptions[2].c_str());
        users = ::atoi(tokenoptions[3].c_str());
        connections = ::atoi(tokenoptions[4].c_str());
        update = ::atoi(tokenoptions[5].c_str());
        product = ::atoi(tokenoptions[6].c_str());
    }
    catch (...)
    {
        return ERROR_INVALIDFILE;
    }

    if (s == 14)
    {
        duration = ::atoi(tokenoptions[7].c_str());
        expYear = ::atoi(tokenoptions[8].c_str());
        expMonth = ::atoi(tokenoptions[9].c_str());
        expDay = ::atoi(tokenoptions[10].c_str());
        startYear = ::atoi(tokenoptions[11].c_str());
        startMonth = ::atoi(tokenoptions[12].c_str());
        startDay = ::atoi(tokenoptions[13].c_str());
    }

    return ERROR_NOERROR;
}

int CSoftLicenseMgr::CheckMachineFingerPrint(const string& szfingerprint, const string& szmachine, const string& szdomain, const string& szcpu)
{
    if (szfingerprint.length() == 0)
        return ERROR_INVALIDFILE;

    //proceed signature
    istringstream ss(szfingerprint);
    string token;
    vector<string> tokens;

    while (getline(ss, token, ';'))
    {
        tokens.push_back(token);
    }

    size_t ts = tokens.size();

    if (ts < 3)
        return ERROR_INVALIDFILE;

    if (tokens[0].compare("fig1") != 0)
        return ERROR_INVALIDFILE;

    int nSigVersion = 0;

    try
    {
        nSigVersion = ::atoi(tokens[1].c_str());
    }
    catch (...)
    {
        return ERROR_INVALIDFILE;
    }

    int error = ERROR_NOERROR;

    //finger print
    int errorFingerPrint = CompareFingerPrint(tokens[2]);

    if (errorFingerPrint == ERROR_FINGERPRINT)
        return ERROR_FINGERPRINT;

    //machine name
    int errorMachine = ERROR_NOERROR;
    std::string szCurrentMachine = getMachineName();
    if (szCurrentMachine.compare(szmachine) != 0)
    {
        errorMachine = ERROR_MACHINE;
    }

    //domain
    int errorDomaine = ERROR_NOERROR;
    std::string szCurrentDomain = getComputerDomain();
    if (szCurrentDomain.compare(szdomain) != 0)
    {
        errorDomaine = ERROR_DOMAIN;
    }

    //CPU
    int errorCPU = ERROR_NOERROR;

    if (szcpu.length())
    {
    std::string szCurrentCPU = getCPU();
    if (szCurrentCPU.compare(szcpu) != 0)
    {
        errorCPU = ERROR_CPU;
    }
    }

    if (errorFingerPrint == ERROR_NOERROR)
    {
        bool bMAchineOK = (errorMachine == ERROR_NOERROR) ? TRUE : FALSE;
        bool bCPUOK = (errorCPU == ERROR_NOERROR) ? TRUE : FALSE;

        if (bMAchineOK || bCPUOK)
            return ERROR_NOERROR;
    }
    else
    {
            if ((errorMachine == ERROR_NOERROR) && (errorCPU == ERROR_NOERROR))
                return ERROR_NOERROR;

        if (szdomain.length())
        {
            if ((errorMachine == ERROR_NOERROR) && (errorDomaine == ERROR_NOERROR))
                return ERROR_NOERROR;

            if ((errorCPU == ERROR_NOERROR) && (errorDomaine == ERROR_NOERROR))
                return ERROR_NOERROR;
        }
    }

    return ERROR_FINGERPRINT;
}

int CSoftLicenseMgr::CheckoutLicense(const string& szpath, const string& fileouput, _LicOptions& options, bool version1)
{
    std::remove(fileouput.c_str());

    int error = ERROR_NOERROR;

    error = CheckFile(szpath, fileouput);

    if (error != ERROR_NOERROR)
        return error;

    vector<pair<string, vector<pair<string, string>>>> iniData = parseIniFile(szpath);

    CustomCypher    acypher;

    string serial, szSignature, szOptions, szmachine, szdomain, szCPU;

    for (const auto& section : iniData)
    {
        string asection = section.first;

        for (const auto& entry : section.second)
        {
            string aentry = entry.first;
            if (asection == g_sectionGeneral)
            {
                if (aentry.compare(g_entryMeters) == 0)
                {
                    //          *meters = stoi(avalue);
                }
                else if (aentry.compare(g_entryUsers) == 0)
                {
                    //         *users = stoi(avalue);
                }
                else if (aentry.compare(g_entryConnections) == 0)
                {
                    //         *connections = stoi(avalue);
                }
                else if (aentry.compare(g_entryVersion) == 0)
                {
                    //        *version = stoi(avalue);
                }
                else if (aentry.compare(g_entryProduct) == 0)
                {
                    //         *product = stoi(avalue);
                }
                else if (aentry.compare(g_entrySerial) == 0)
                {
                     serial = entry.second;
                }
                else if (aentry.compare(g_entryUpdate) == 0)
                {
                    //        *update = stoi(avalue);
                }
            }
            else if (asection == g_sectionFeatures)
            {
                if (aentry.compare(g_S2) == 0)
                {
                    CustomCypher cipher;
                    szSignature = cipher.decrypt(entry.second);
                }
                else if (aentry.compare(g_T1) == 0)
                {
                    CustomCypher cipher;
                    szOptions = cipher.decrypt(entry.second);
                }
                else if (aentry.compare(g_E4) == 0)
                {
                    CustomCypher acypherM(cypherType_machine);
                    szmachine = acypherM.decrypt(entry.second);
                }
                else if (aentry.compare(g_F1) == 0)
                {
                    CustomCypher acypherM(cypherType_domain);
                    szdomain = acypherM.decrypt(entry.second);
                }
                else if (aentry.compare(g_OZ) == 0)
                {
                    if (entry.second.length())
                    {
                        CustomCypher cipher;
                        szCPU = cipher.decrypt(entry.second);
                }
                };
            }
        }
    }

    error = CheckMachineFingerPrint(szSignature, szmachine, szdomain, szCPU);
    if (error != ERROR_NOERROR)
        return error;

    error = ExtractOptions(szOptions, options.m_meters, options.m_users, options.m_connections, options.m_product, options.m_update,
        options.m_version, options.m_TemporaryLicense, options.m_TemporaryLicenseDuration, options.m_expirationYear, options.m_expirationMonth, options.m_expirationDay,
        options.m_activationYear, options.m_activationMonth, options.m_activationDay);
    if (error != ERROR_NOERROR)
        return error;

    options.m_serial = serial;

    if (version1)
        return ERROR_NOERROR;

    if (options.m_version && (options.m_TemporaryLicenseDuration>0) )
    {
        tm date = {};
        date.tm_year = options.m_expirationYear - 1900;
        date.tm_mon = options.m_expirationMonth-1;
        date.tm_mday = options.m_expirationDay;
        date.tm_hour = 23;
        date.tm_min = 59;
        date.tm_sec = 59;

        tm Actdate = {};
        Actdate.tm_year = options.m_activationYear - 1900;
        Actdate.tm_mon = options.m_activationMonth-1;
        Actdate.tm_mday = options.m_activationDay;
        Actdate.tm_hour = 0;
        Actdate.tm_min = 0;
        Actdate.tm_sec = 0;

        time_t expirationTime = mktime(&date);
        time_t activationtime = mktime(&Actdate);
        time_t now = std::time(nullptr);

        tm localTime;
        localtime_s(&localTime, &now);
       
        if (now > expirationTime)
            return ERROR_LICENSE_EXPIRED;

        if (activationtime > now)
            return ERROR_ACTIVATION_TIME;
    }
    return ERROR_NOERROR;
}
