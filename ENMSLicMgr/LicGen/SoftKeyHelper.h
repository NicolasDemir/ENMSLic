#pragma once

class CSoftLicInfo
{
public:
    CSoftLicInfo();
    CString m_szLicPath;
    CString m_szSerial;
    CString m_szVersion;
    CString m_szReference;
    CString m_szActivationDate;
    CString m_szExpirationDate;
    CString m_szOrganisation;
    CString m_szProject;
    CString m_szLocation;
    CString m_szDescription;

    CString m_szComputer;
    CString m_szDomain;
};


class CIniFile;

class CSoftKeyHelper
{
public:
    CSoftKeyHelper(void);
    ~CSoftKeyHelper(void);

    CString m_sVolume;
    CString m_sMachine;
    CString m_sMac;
    CString m_sMacAdvanced;


    BOOL    RetrieveSystemInformation();
    CString GetWindowsKey();
    CString GetTargetName();
    CString GetMacAddress();
    CString GetMacAddressAdvanced();
    BOOL    FillMacAddress(CString& sz, BYTE* p);
    CString GetDomain();
    CString  GetCPUID();

    CString DecryptedPassword(LPCTSTR pszEncryptedPassword);
    CString EncryptedPassword(LPCTSTR pszDecryptedPassword);
    void ByteToRandomizedBytes(BYTE bData, BYTE& bRandomizedData1, BYTE& bRandomizedData2);
    BYTE RandomizedBytesToByte(BYTE bRandomizedData1, BYTE bRandomizedData2);

    CString m_SoftSignature;
    CString m_HardSignature;
    CString m_HadSignatureAdvanced;
    CString m_CRC;

    CString         m_licfileName;
    CIniFile		m_Licfile;

    CString m_Extradata1;
    CString m_key;
    CString m_szCustomer;

    CString GetHardSignature() { return m_HardSignature; }
    CString GetSoftSignature() { return m_SoftSignature; }
    CString GetHardSignatureAdvanced() { return m_HadSignatureAdvanced; }
    CString GetCRC() { return m_CRC; }

    BOOL CheckSectionKey(const CIniFile& IniFile, LPCTSTR pszSection, LPCTSTR pszKeyEntry /* = NULL */, LPCTSTR pszExtraData /* = NULL */);
    BOOL IsFileExist(LPCTSTR pszFilePath);
    BOOL IsDirExist(LPCTSTR pszFilePath);
    int  ValidLicenseFileExists(WORD wCount, LPWORD lpwValues, CString szFilePath);
    BOOL ExtractFeatures(CString szSection, CString szEntry, WORD wcount, LPWORD lpwValues);

    int ExtractLicenseGlobalInfo(CSoftLicInfo& softlicinfo);

    CString CStringFromGUID(const GUID& guid);
    CString GetCustomer() { return m_szCustomer; }
};

#define ERROR_READING_FILE					100
#define ERROR_INVALID_FILE					200
#define ERROR_SERIAL_KEY_NUMBER_MISMATCH	201
#define ERROR_ARC_KEY_NUMBER_MISMATCH		202
#define ERROR_INVALID_KEY					203
#define ERROR_ARC_KEY_NUMBER_MISMATCH_1		204

