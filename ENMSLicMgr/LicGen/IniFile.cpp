/////////////////////////////////////////////////////////////////////////////
//
// IniFile.cpp: Implementation of the CIniFile class.
//
/////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////

#define SECTION_BUFFER_SIZE 65535

/////////////////////////////////////////////////////////////////////////////

CIniFile::CIniFile ()
{
}

/////////////////////////////////////////////////////////////////////////////

CIniFile::CIniFile (LPCTSTR pszFileName, int nDir /* = dirNone */)
{
    SetFileName(pszFileName, nDir);
}

/////////////////////////////////////////////////////////////////////////////

CIniFile::~CIniFile ()
{
}

/////////////////////////////////////////////////////////////////////////////

CString CIniFile::GetFilePath () const
{
    return m_strIniFileName;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::IsValidEntry (LPCTSTR pszSection, LPCTSTR pszEntry) const
{
    ASSERT(_tcslen (m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);

    CStringList EntryList;

    if (EnumEntries(pszSection, EntryList))
    {
        CString strEntry;
        POSITION pos = EntryList.GetHeadPosition();

        while (pos)
        {
            strEntry = EntryList.GetNext(pos);

            if (strEntry.CompareNoCase(pszEntry) == 0)
                return TRUE;
        }
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::IsValidSection (LPCTSTR pszSection) const
{
    ASSERT(_tcslen (m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);

    CStringList SectionList;

    if (EnumSections(SectionList))
    {
        CString strSection;
        POSITION pos = SectionList.GetHeadPosition();

        while (pos)
        {
            strSection = SectionList.GetNext(pos);

            if (strSection.CompareNoCase(pszSection) == 0)
                return TRUE;
        }
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::SetFileName (LPCTSTR pszFileName, int nDir /* = dirNone */)
{
    switch (nDir)
    {
        case dirProcess :
        {
            TCHAR szProcessPathName[_MAX_PATH];
            ::GetModuleFileName(NULL, szProcessPathName, _countof(szProcessPathName));

            CString strProcessDir = szProcessPathName;
            int n = strProcessDir.ReverseFind(_T('\\'));

            m_strIniFileName = strProcessDir.Left(n + 1);
            m_strIniFileName += pszFileName;
            break;
        }

        case dirNone :

            m_strIniFileName = pszFileName;
            break;

        default :

            return FALSE;
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::GetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, int & nValue, int nDefault /* = 0 */) const
{
    ASSERT(_tcslen (m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);

    nValue = (int)::GetPrivateProfileInt(pszSection, pszEntry, nDefault, m_strIniFileName);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::GetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, __int64 & i64Value, __int64 i64Default /* = 0 */) const
{
    ASSERT(_tcslen (m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);

    TCHAR szDefault[40];
    _stprintf_s(szDefault, _T("%I64d"), i64Default);

    TCHAR szT[4096];

    DWORD dw = ::GetPrivateProfileString(pszSection, pszEntry, szDefault, szT, sizeof(szT), m_strIniFileName);
    ASSERT(dw < sizeof(szT) - 1);

    i64Value = _ttoi64(szT);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::GetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, DWORD & dwValue, DWORD dwDefault /* = 0 */) const
{
    ASSERT(_tcslen (m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);

    dwValue = (DWORD)::GetPrivateProfileInt(pszSection, pszEntry, (int)dwDefault, m_strIniFileName);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::GetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, CString & strValue, LPCTSTR pszDefault /* = NULL */) const
{
    ASSERT(_tcslen (m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);

    if (pszDefault == NULL)
        pszDefault = _T("");    // don't pass in NULL

    TCHAR szT[4096];

    DWORD dw = ::GetPrivateProfileString(pszSection, pszEntry, pszDefault, szT, sizeof(szT), m_strIniFileName);
    ASSERT(dw < sizeof(szT) - 1);

    strValue = szT;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::GetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, LPVOID pvStruct, UINT uSizeStruct) const
{
    ASSERT(_tcslen(m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);
    ASSERT(pvStruct != NULL);

    return ::GetPrivateProfileStruct(pszSection, pszEntry, pvStruct, uSizeStruct, m_strIniFileName);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::SetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, int nValue)
{
    ASSERT(_tcslen(m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);

    TCHAR szT[20];
    _stprintf_s(szT, _T("%d"), nValue);

    return ::WritePrivateProfileString(pszSection, pszEntry, szT, m_strIniFileName);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::SetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, __int64 i64Value)
{
    ASSERT(_tcslen(m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);

    TCHAR szT[40];
    _stprintf_s(szT, _T("%I64d"), i64Value);

    return ::WritePrivateProfileString(pszSection, pszEntry, szT, m_strIniFileName);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::SetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, DWORD dwValue, int nFormat /* = formatDecimal */)
{
    ASSERT(_tcslen(m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);

    LPCTSTR pszFormat;

    switch (nFormat)
    {
        case formatDecimal :

            pszFormat = _T("%d");
            break;

        case formatHexadecimal :

            pszFormat = _T("0x%08X");
            break;

        default :

            return FALSE;
    }

    TCHAR szT[20];
    _stprintf_s(szT, pszFormat, dwValue);

    return ::WritePrivateProfileString(pszSection, pszEntry, szT, m_strIniFileName);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::SetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszValue)
{
    ASSERT(_tcslen(m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);
    ASSERT(pszValue != NULL);

    return ::WritePrivateProfileString(pszSection, pszEntry, pszValue, m_strIniFileName);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::SetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, LPVOID pvStruct, UINT uSizeStruct)
{
    ASSERT(_tcslen(m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);
    ASSERT(pvStruct != NULL);

    return ::WritePrivateProfileStruct(pszSection, pszEntry, pvStruct, uSizeStruct, m_strIniFileName);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::ReplaceSectionEntries (LPCTSTR pszSection, const CStringList & EntryList)
{
    ASSERT(_tcslen(m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);

    if (EntryList.IsEmpty())
        return ::WritePrivateProfileString(pszSection, NULL, NULL, m_strIniFileName);

    TCHAR szT[SECTION_BUFFER_SIZE];
    LPTSTR pszT = &szT[0];
    size_t remainingSize = _countof(szT);
    CString	strEntry;
    POSITION pos = EntryList.GetHeadPosition();

    while (pos)
    {
        strEntry = EntryList.GetNext(pos);
        size_t entryLength = strEntry.GetLength() + 1;

        if (entryLength >= remainingSize)
            break;

        _tcscpy_s(pszT, remainingSize, strEntry);

        pszT += entryLength;
        remainingSize -= entryLength;
    }

    *pszT = _T('\0');   // Add the final null character

    return ::WritePrivateProfileSection(pszSection, szT, m_strIniFileName);
}

/////////////////////////////////////////////////////////////////////////////

int CIniFile::EnumEntries (LPCTSTR pszSection, CStringList & EntryList) const
{
    ASSERT(_tcslen(m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);

    TCHAR szT[SECTION_BUFFER_SIZE];

    DWORD dw = ::GetPrivateProfileString(pszSection, NULL, _T(""), szT, sizeof(szT), m_strIniFileName);
    ASSERT(dw < sizeof(szT) - 1);

    EntryList.RemoveAll();

    LPTSTR pszT = szT;

    while (*pszT)
    {
        EntryList.AddTail (pszT);
        pszT += _tcslen (pszT) + 1;
    }

    return EntryList.GetCount ();
}

/////////////////////////////////////////////////////////////////////////////

int CIniFile::EnumSections (CStringList & SectionList) const
{
    ASSERT(_tcslen(m_strIniFileName) != 0);

    TCHAR szT[SECTION_BUFFER_SIZE];

    DWORD dw = ::GetPrivateProfileString(NULL, NULL, _T(""), szT, sizeof(szT), m_strIniFileName);
    ASSERT(dw < sizeof(szT) - 1);

    SectionList.RemoveAll();

    LPTSTR pszT = szT;

    while (*pszT)
    {
        SectionList.AddTail (pszT);
        pszT += _tcslen (pszT) + 1;
    }
    return SectionList.GetCount();
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::RemoveEntry (LPCTSTR pszSection, LPCTSTR pszEntry)
{
    ASSERT(_tcslen (m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);
    ASSERT(pszEntry != NULL);

    return ::WritePrivateProfileString(pszSection, pszEntry, NULL, m_strIniFileName);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::RemoveSection (LPCTSTR pszSection)
{
    ASSERT(_tcslen (m_strIniFileName) != 0);
    ASSERT(pszSection != NULL);

    return ::WritePrivateProfileString(pszSection, NULL, NULL, m_strIniFileName);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIniFile::FlushSections ()
{
    return ::WritePrivateProfileSection(NULL, NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
