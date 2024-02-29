/////////////////////////////////////////////////////////////////////////////
//
// IniFile.h: Interface for the CIniFile class.
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////

class CIniFile
{
public:
    enum
    {
        dirNone = 0,    // No specific directory.
        dirProcess = 1  // Calling process directory.
    };

    enum
    {
        formatDecimal = 0,
        formatHexadecimal = 1
    };

// Construction
public:

    CIniFile ();
    CIniFile (LPCTSTR pszFileName, int nDir = dirNone);

// Attributes
public:

    // Retrieves the full file path of the initialization file.
    virtual CString GetFilePath () const;

    // Determines if the specified entry exists.
    virtual BOOL IsValidEntry (LPCTSTR pszSection, LPCTSTR pszEntry) const;

    // Determines if the specified section exists.
    virtual BOOL IsValidSection (LPCTSTR pszSection) const;

// Operations
public:

    // Specifies an new initialization file.
    virtual BOOL SetFileName (LPCTSTR pszFileName, int nDir = dirNone);

    // Retrieves the value of an entry from the initialization file.
    virtual BOOL GetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, int & nValue, int nDefault = 0) const;
    virtual BOOL GetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, __int64 & i64Value, __int64 i64Default = 0) const;
    virtual BOOL GetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, DWORD & dwValue, DWORD dwDefault = 0) const;
    virtual BOOL GetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, CString & strValue, LPCTSTR pszDefault = NULL) const;
    virtual BOOL GetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, LPVOID pvStruct, UINT uSizeStruct) const;

    // Sets the value of an entry from the initialization file.
    virtual BOOL SetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, int nValue);
    virtual BOOL SetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, __int64 i64Value);
    virtual BOOL SetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, DWORD dwValue, int nFormat = formatDecimal);
    virtual BOOL SetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszValue);
    virtual BOOL SetEntryValue (LPCTSTR pszSection, LPCTSTR pszEntry, LPVOID pvStruct, UINT uSizeStruct);

    // Replaces the entries and values under the specified section in the initialization file.
    virtual BOOL ReplaceSectionEntries (LPCTSTR pszSection, const CStringList & EntryList);

    // Enumerates the entries under the specified section in the initialization file.
    virtual int EnumEntries (LPCTSTR pszSection, CStringList & EntryList) const;

    // Enumerates the sections in the initialization file.
    virtual int EnumSections (CStringList & SectionList) const;

    // Removes an entry from the initialization file.
    virtual BOOL RemoveEntry (LPCTSTR pszSection, LPCTSTR pszEntry);

    // Removes an entire section from the initialization file, including all entries within the section.
    virtual BOOL RemoveSection (LPCTSTR pszSection);

    // Flushes the cached version of the initialization file (sometimes needed under Windows 9x).
    virtual BOOL FlushSections ();

// Implementation
public:
    ~CIniFile ();

protected:
    CString m_strIniFileName;   // Name of file in file system
};

/////////////////////////////////////////////////////////////////////////////
