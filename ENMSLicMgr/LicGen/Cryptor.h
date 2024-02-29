////////////////////////////////////////////////////////////////////////////////
//
// Cryptor.h
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////

class CCryptor
{
// Construction

public:
    CCryptor ();

// Operations

public:
    void SetKey (LPCTSTR pszKey);
    void TransformByte (LPBYTE pbTarget);
    void WeakTransformByte (LPBYTE pbTarget);

// Implementation

protected:
    const DWORD m_dwMask_A;
    const DWORD m_dwMask_B;
    const DWORD m_dwMask_C;
    const DWORD m_dwRot0_A;
    const DWORD m_dwRot0_B;
    const DWORD m_dwRot0_C;
    const DWORD m_dwRot1_A;
    const DWORD m_dwRot1_B;
    const DWORD m_dwRot1_C;
    DWORD m_dwLFSR_A;
    DWORD m_dwLFSR_B;
    DWORD m_dwLFSR_C;
};

////////////////////////////////////////////////////////////////////////////////
