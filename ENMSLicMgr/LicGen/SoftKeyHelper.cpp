#include "pch.h"
#include "inifile.h"
#include "SoftKeyHelper.h"
#include "HashAlgorithms.h"
#include "Cryptor.h"
#include "EncryptionMgr.h"

#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

extern std::string CStringToString(const CString& cstr);
extern CString StringToCString(const std::string& str);

/**************************************
        Class CSoftInfo
**************************************/

CSoftLicInfo::CSoftLicInfo()
{
}

/**************************************
        Class CSoftKeyHelper
**************************************/

CSoftKeyHelper::CSoftKeyHelper(void)
{
    m_licfileName = _T("lic.dat");

    // {3FE256DF-C7CB-4A6C-991E-3DF81444B38B}
    static const GUID aguid =
    { 0x3fe256df, 0xc7cb, 0x4b6c, { 0x99, 0x1e, 0x3d, 0xf8, 0x14, 0x44, 0xa3, 0x8b } };

    m_Extradata1 = CStringFromGUID(aguid);
}


CSoftKeyHelper::~CSoftKeyHelper(void)
{
}

BOOL CSoftKeyHelper::RetrieveSystemInformation()
{
    m_sVolume = GetWindowsKey();
    m_sMachine = GetTargetName();
    m_sMac = GetMacAddress();
    m_sMacAdvanced = GetMacAddressAdvanced();

    m_sDomain = GetDomain();
    m_sCPU= GetCPUID();

    CString SignatureSoft = m_sMachine + _T("|") + m_sVolume;
    CString SignatureHard = m_sMac;

    m_SoftSignature = EncryptedPassword(SignatureSoft);
    m_HardSignature = EncryptedPassword(m_sMac);

    m_HadSignatureAdvanced = EncryptString(_T("fig1;1;") + m_sMacAdvanced);
    m_CPUID = EncryptString(m_sCPU);
     

    m_CRC = EncryptedPassword(SignatureSoft + m_sMac);


    return TRUE;
}

CString CSoftKeyHelper::EncryptString(LPCTSTR pszDecryptedPassword)
{
    CustomCypher cypher;
    string sz = CStringToString(pszDecryptedPassword);
    std::string str = cypher.encrypt(sz);
    return CString(str.c_str());
}



// Windows Product Key Extract
CString CSoftKeyHelper::GetWindowsKey()
{
    HKEY  hRegistryKey;          //Registry Handler 
    DWORD DataLength = 50;            //Digital Product Key Length 
    BYTE  ProductKeyExtract[51]; //Extract Key 
    bool  bOk = false;

    int nreturn = ERROR_SUCCESS;

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\MICROSOFT\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hRegistryKey) == ERROR_SUCCESS)
    {

        nreturn = RegQueryValueEx(hRegistryKey, _T("ProductId"), NULL, NULL, ProductKeyExtract, &DataLength);
        RegCloseKey(hRegistryKey);
    }
    else
    {
        return _T("");
    }

    if (nreturn == ERROR_SUCCESS)
    {
        CString szrsult(ProductKeyExtract);
        return szrsult;
    }
    else
    {
        return _T("");
    }
}

CString CSoftKeyHelper::GetTargetName()
{
    TCHAR   szComputerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD   dw = MAX_COMPUTERNAME_LENGTH + 1;

    BOOL bOK = ::GetComputerName(szComputerName, &dw);

    if (bOK)
        return CString(szComputerName);
    else
        return _T("");
}

CString CSoftKeyHelper::GetMacAddress()
{
    IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information for up to 16 NICs
    DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);                  // [in] size of receive data buffer
    if (dwStatus != ERROR_SUCCESS)
    {
        return _T("");
    }

    CString szReturn = _T("");

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to  current adapter info

    FillMacAddress(szReturn, pAdapterInfo->Address);
    /*do
    {
       FillMacAddress(szReturn, pAdapterInfo->Address);
       pAdapterInfo = pAdapterInfo->Next;    // Progress through linked list
    } while (pAdapterInfo);                    // Terminate if last adapter*/

    return szReturn;
}

CString CSoftKeyHelper::GetMacAddressAdvanced()
{
    IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information for up to 16 NICs
    DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);                  // [in] size of receive data buffer
    if (dwStatus != ERROR_SUCCESS)
    {
        return _T("");
    }

    CString szReturn = _T("");

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to  current adapter info

    do
    {
       FillMacAddress(szReturn, pAdapterInfo->Address);
       pAdapterInfo = pAdapterInfo->Next;    
    } while (pAdapterInfo);                  

    return szReturn;
}


BOOL CSoftKeyHelper::FillMacAddress(CString& sz, BYTE* p)
{
    if (sz.GetLength())
        sz += _T("|");

    CString sztemp;
    sztemp.Format(_T("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X")
        , p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

    sz += sztemp;
    return TRUE;
}

CString CSoftKeyHelper::GetDomain()
{
    DWORD bufSize = MAX_PATH;
    TCHAR domainNameBuf[MAX_PATH];
    GetComputerNameEx(ComputerNameDnsDomain, domainNameBuf, &bufSize);
    CString szreturn(domainNameBuf);
    return szreturn;
}

CString  CSoftKeyHelper::GetCPUID()
{
    int CPUInfo[4] = { -1 };
    __cpuid(CPUInfo, 0);

    CString szReturn;
    szReturn.Format(_T("%d-%d-%d-%d"), CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
    return szReturn;
}

void CSoftKeyHelper::ByteToRandomizedBytes(BYTE bData, BYTE& bRandomizedData1, BYTE& bRandomizedData2)
{
    static BOOL bFirstTime = TRUE;

    if (bFirstTime)
    {
        ::srand((unsigned)::GetTickCount());
        bFirstTime = FALSE;
    }

    BYTE bRandom = (BYTE)(::rand() % 256);

    bRandomizedData1 = (bData & 0xC0) | ((bData >> 2) & 0x0C) | ((bRandom >> 2) & 0x30) | ((bRandom >> 4) & 0x03);
    bRandomizedData2 = ((bData << 4) & 0xC0) | ((bData << 2) & 0x0C) | ((bRandom << 2) & 0x30) | (bRandom & 0x03);
}

/////////////////////////////////////////////////////////////////////////////

BYTE CSoftKeyHelper::RandomizedBytesToByte(BYTE bRandomizedData1, BYTE bRandomizedData2)
{
    BYTE bData = (bRandomizedData1 & 0xC0) | ((bRandomizedData1 << 2) & 0x30) | ((bRandomizedData2 >> 4) & 0x0C) | ((bRandomizedData2 >> 2) & 0x03);

    return bData;
}

/////////////////////////////////////////////////////////////////////////////

CString CSoftKeyHelper::EncryptedPassword(LPCTSTR pszDecryptedPassword)
{
    TCHAR szCryptData[512];
    _tcscpy_s(szCryptData, pszDecryptedPassword);

    DWORD cbPassword = _tcslen(szCryptData);

    //    sesCryptData (SES_ALGID_90, m_pszCryptKey, szCryptData, cbPassword);

    TCHAR szEncryptedPassword[512];
    szEncryptedPassword[0] = _T('\0');

    TCHAR szHexa[3];

    for (DWORD n = 0; n < cbPassword; n++)
    {
        BYTE bRandomizedData1, bRandomizedData2;

        ByteToRandomizedBytes(szCryptData[n], bRandomizedData1, bRandomizedData2);

        _stprintf_s(szHexa, _T("%02X"), (UINT)bRandomizedData1);
        _tcscat_s(szEncryptedPassword, szHexa);

        _stprintf_s(szHexa, _T("%02X"), (UINT)bRandomizedData2);
        _tcscat_s(szEncryptedPassword, szHexa);
    }

    return szEncryptedPassword;
}

/////////////////////////////////////////////////////////////////////////////

CString CSoftKeyHelper::DecryptedPassword(LPCTSTR pszEncryptedPassword)
{
    DWORD cbEncryptedPassword = _tcslen(pszEncryptedPassword);

    TCHAR szDecryptedPassword[512];
    TCHAR szHexa[3];

    for (DWORD n = 0, i = 0; n < cbEncryptedPassword; n += 4)
    {
        szHexa[0] = pszEncryptedPassword[n];
        szHexa[1] = pszEncryptedPassword[n + 1];
        szHexa[2] = _T('\0');

        BYTE bRandomizedData1 = (BYTE)_tcstoul(szHexa, NULL, 16);

        szHexa[0] = pszEncryptedPassword[n + 2];
        szHexa[1] = pszEncryptedPassword[n + 3];
        szHexa[2] = _T('\0');

        BYTE bRandomizedData2 = (BYTE)_tcstoul(szHexa, NULL, 16);

        szDecryptedPassword[i++] = RandomizedBytesToByte(bRandomizedData1, bRandomizedData2);
    }

    DWORD cbDecryptedPassword = cbEncryptedPassword / 4;

    //    sesCryptData (SES_ALGID_90, m_pszCryptKey, szDecryptedPassword, cbDecryptedPassword);

    szDecryptedPassword[cbDecryptedPassword] = _T('\0');

    return szDecryptedPassword;
}

int CSoftKeyHelper::ValidLicenseFileExists(WORD wCount, LPWORD lpwValues, CString strLicFilePath)
{
    if (!IsFileExist(strLicFilePath))
        return -1;

    CString g_szSectionGeneral = _T("General");
    CString g_szSectionLicense = _T("License");
    CString g_szOrganisation = _T("Organization");

    m_Licfile.SetFileName(strLicFilePath);

    BOOL bOK = CheckSectionKey(m_Licfile, g_szSectionGeneral, NULL, m_Extradata1);

    if (!bOK)
        return -2;

    bOK = CheckSectionKey(m_Licfile, g_szSectionLicense, NULL, m_Extradata1);

    if (!bOK)
        return -3;

    CString sz = _T("4");
    bOK = ExtractFeatures(g_szSectionLicense, sz, 0, lpwValues);

    if (!bOK)
        return -4;

    sz = _T("1");
    bOK = ExtractFeatures(g_szSectionLicense, sz, 10, lpwValues);
    if (!bOK)
        return -4;

    sz = _T("3");
    bOK = ExtractFeatures(g_szSectionLicense, sz, 20, lpwValues);
    if (!bOK)
        return -4;

    sz = _T("7");
    bOK = ExtractFeatures(g_szSectionLicense, sz, 30, lpwValues);
    if (!bOK)
        return -4;

    sz = _T("6");
    bOK = ExtractFeatures(g_szSectionLicense, sz, 40, lpwValues);
    if (!bOK)
        return -4;

    sz = _T("9");
    bOK = ExtractFeatures(g_szSectionLicense, sz, 50, lpwValues);

    sz = _T("10");
    bOK = ExtractFeatures(g_szSectionLicense, sz, 60, lpwValues);

    bOK = m_Licfile.GetEntryValue(g_szSectionGeneral, g_szOrganisation, sz);

    if (bOK)
    {
        m_szCustomer = sz;
    }

    bOK = m_Licfile.GetEntryValue(g_szSectionGeneral, _T("computer"), sz);

    if (bOK)
    {
        if (sz.GetLength())
        {
            CString sComputer = GetTargetName();

            if (sComputer.CompareNoCase(sz) != 0)
                return -7;
        }
    }
    else
    {
        return -9;
    }

    bOK = m_Licfile.GetEntryValue(g_szSectionGeneral, _T("domain"), sz);

    if (bOK)
    {
        CString sDomain = GetDomain();
        if (sz.GetLength())
        {

            if (sDomain.CompareNoCase(sz) != 0)
            {
                return -8;
            }
        }
        else
        {
            if (sDomain.GetLength())
            {
                return -10;
            }
        }
    }


    bOK = m_Licfile.GetEntryValue(g_szSectionGeneral, _T("s2"), sz);

    if (bOK)
    {
        if (sz.GetLength())
        {
            CString szVolume = GetWindowsKey();
            CString sMachine = GetTargetName();
            CString SignatureSoft = sMachine + _T("|") + szVolume;

            CString szSoftSignature = DecryptedPassword(sz);

            if (szSoftSignature.CompareNoCase(SignatureSoft) != 0)
                return -5;
        }
    }

    bOK = m_Licfile.GetEntryValue(g_szSectionGeneral, _T("s1"), sz);

    if (bOK)
    {
        if (sz.GetLength())
        {
            CString szHard = GetMacAddress();
            CString szHardSignature = DecryptedPassword(sz);

            if (szHardSignature.CompareNoCase(szHard) != 0)
                return -6;
        }
    }


    return 0;
}


int CSoftKeyHelper::ExtractLicenseGlobalInfo(CSoftLicInfo& softlicinfo)
{
    if (!IsFileExist(softlicinfo.m_szLicPath))
        return -1;

    CString g_szSectionGeneral = _T("General");

    m_Licfile.SetFileName(softlicinfo.m_szLicPath);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("Serial"), softlicinfo.m_szSerial);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("Version"), softlicinfo.m_szVersion);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("Reference"), softlicinfo.m_szReference);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("ActivationDate"), softlicinfo.m_szActivationDate);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("ExpirationDate"), softlicinfo.m_szExpirationDate);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("Organization"), softlicinfo.m_szOrganisation);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("Project"), softlicinfo.m_szProject);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("Location"), softlicinfo.m_szLocation);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("Description"), softlicinfo.m_szDescription);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("Computer"), softlicinfo.m_szComputer);
    m_Licfile.GetEntryValue(g_szSectionGeneral, _T("Domain"), softlicinfo.m_szDomain);

    return 0;
}


BOOL CSoftKeyHelper::CheckSectionKey(const CIniFile& IniFile, LPCTSTR pszSection, LPCTSTR pszKeyEntry /* = NULL */, LPCTSTR pszExtraData /* = NULL */)
{
    CString strInfos;
    CString strKeyEntry(pszKeyEntry);
    CString strExtraData(pszExtraData);
    CString strEntry;
    CString strEntryValue;
    CStringList EntryList;

    if (strKeyEntry.IsEmpty())
        strKeyEntry = _T("Key");

    strInfos.Format(_T("[%s]%s"), pszSection, (LPCTSTR)strExtraData);

    IniFile.EnumEntries(pszSection, EntryList);

    POSITION pos = EntryList.GetHeadPosition();

    while (pos)
    {
        strEntry = EntryList.GetNext(pos);

        if (strEntry.CompareNoCase(strKeyEntry) == 0)
            continue;

        if (!IniFile.GetEntryValue(pszSection, strEntry, strEntryValue))
            continue;

        if (strEntryValue.IsEmpty())
            continue;

        strInfos += strEntryValue;
    }

    CString strKeyValue;

    IniFile.GetEntryValue(pszSection, strKeyEntry, strKeyValue);

    if (strKeyValue.IsEmpty())
        return FALSE;

    TCHAR szExpectedKey[MD5_MSG_DIGEST_STRING_LEN + 1];
    ComputeHashMD5(strInfos, szExpectedKey, _countof(szExpectedKey));

    return (strKeyValue.CompareNoCase(szExpectedKey) == 0) ? TRUE : FALSE;
}

BOOL CSoftKeyHelper::IsFileExist(LPCTSTR pszFilePath)
{
    DWORD dwFileAttributes = ::GetFileAttributes(pszFilePath);

    return ((dwFileAttributes == 0xFFFFFFFF) || (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) ? FALSE : TRUE;
}

BOOL CSoftKeyHelper::IsDirExist(LPCTSTR pszFilePath)
{
    DWORD dwFileAttributes = ::GetFileAttributes(pszFilePath);

    return ((dwFileAttributes != 0xFFFFFFFF) && ((dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? TRUE : FALSE));
}

BOOL CSoftKeyHelper::ExtractFeatures(CString szSection, CString szEntry, WORD wcount, LPWORD lpwValues)
{
    CString szinf;

    BOOL bOK = m_Licfile.GetEntryValue(szSection, szEntry, szinf);

    if (!bOK)
    {
        int nIndex = wcount;
        for (int i = wcount; i < wcount + 10; i++)
            lpwValues[i] = 0;
        return FALSE;
    }

    CString szsep = _T("-");

    int nIndex = wcount;
    for (int i = 0; i < 10; i++)
    {
        int npos = szinf.Find(szsep);
        if (npos > 0)
        {
            CString sztemp = szinf.Left(npos);
            szinf = szinf.Mid(npos + 1);
            WORD w = (WORD)_ttoi(sztemp);
            lpwValues[nIndex] = w;
        }
        else
        {
            WORD w = (WORD)_ttoi(szinf);
            lpwValues[nIndex] = w;
            break;
        }

        nIndex = nIndex + 1;
    }

    return (nIndex == wcount + 9) ? TRUE : FALSE;
}

CString CSoftKeyHelper::CStringFromGUID(const GUID& guid)
{
    OLECHAR wszGUID[38 + 1];
    ::StringFromGUID2(guid, wszGUID, _countof(wszGUID));
    return CString(wszGUID);
}


