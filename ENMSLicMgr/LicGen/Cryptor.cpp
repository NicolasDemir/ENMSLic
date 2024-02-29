////////////////////////////////////////////////////////////////////////////////
//
// Cryptor.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "Cryptor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
//
// Source code inspired by an article published in C/C++ Users Journal,
// Volume 16 Issue 9, September 1998.
//
// http://www.drdobbs.com/applying-stream-encryption/184403554
//
// Copyright (c) 1998 by Warren Ward. Permission is granted to use this
// source code as long as this copyright notice appears in all source files
// that now include it.
//
////////////////////////////////////////////////////////////////////////////////

CCryptor::CCryptor () :

    // Initialize LFSRs to non-zero values
    m_dwLFSR_A(0x13579BDF),
    m_dwLFSR_B(0x2468ACE0),
    m_dwLFSR_C(0xFDB97531),

    // Initialize feedback masks to magic numbers
    m_dwMask_A(0x80000062),
    m_dwMask_B(0x40000020),
    m_dwMask_C(0x10000002),

    // Set up LFSR "rotate" masks
    m_dwRot0_A(0x7FFFFFFF),
    m_dwRot0_B(0x3FFFFFFF),
    m_dwRot0_C(0x0FFFFFFF),
    m_dwRot1_A(0x80000000),
    m_dwRot1_B(0xC0000000),
    m_dwRot1_C(0xF0000000)
{
}

////////////////////////////////////////////////////////////////////////////////

// Note that the key size MUST BE exactly 12 characters long.
#define CRYPTOR_KEY_SIZE 12

////////////////////////////////////////////////////////////////////////////////

void CCryptor::SetKey (LPCTSTR pszKey)
{
    TCHAR szSeed[CRYPTOR_KEY_SIZE + 1];

    if (pszKey == NULL || _tcslen(pszKey) == 0)
    {
        // Set default key if none provided.
        _tcscpy_s(szSeed, _T("Default Seed"));
    }
    else
    {
        // If the key has more than 12 characters, only the first 12 are used.
        _tcsncpy_s(szSeed, pszKey, _TRUNCATE);

        // If the key is shorter, it is repeated until it reaches 12 characters in length.
        size_t length = _tcslen(szSeed);

        for (int i = 0; length < CRYPTOR_KEY_SIZE; i++, length++)
        {
            szSeed[length] = szSeed[i];
            szSeed[length+1] = _T('\0');
        }
    }

#ifdef _UNICODE
// #error TODO: if _UNICODE is defined, the key must be converted from a wide character string to a single character string.
#endif

    // LFSR A, B, and C get the first, second, and third four bytes of the seed, respectively

    for (int i = 0; i < 4; i++)
    {
        m_dwLFSR_A = (m_dwLFSR_A <<= 8) | (DWORD)(szSeed[i + 0]);
        m_dwLFSR_B = (m_dwLFSR_B <<= 8) | (DWORD)(szSeed[i + 4]);
        m_dwLFSR_C = (m_dwLFSR_C <<= 8) | (DWORD)(szSeed[i + 8]);
    }

    // If any LFSR contains 0x00000000, load a non-zero default value instead

    if (m_dwLFSR_A == 0x00000000)
        m_dwLFSR_A = 0x13579BDF;

    if (m_dwLFSR_B == 0x00000000)
        m_dwLFSR_B = 0x2468ACE0;

    if (m_dwLFSR_C == 0x00000000)
        m_dwLFSR_C = 0xFDB97531;
}

////////////////////////////////////////////////////////////////////////////////

void CCryptor::TransformByte (LPBYTE pbTarget)
{
    DWORD dwOut_B = m_dwLFSR_B & 0x00000001;
    DWORD dwOut_C = m_dwLFSR_C & 0x00000001;
    BYTE bCrypto = 0;

    // Cycle the LFSRs eight times to get eight pseudo-random bits.
    // Assemble these into a single random character (bCrypto).

    for (int n = 0; n < 8; n++)
    {
        if (m_dwLFSR_A & 0x00000001)
        {
            // The least-significant bit of LFSR A is "1". XOR LFSR A with its feedback mask
            m_dwLFSR_A = (m_dwLFSR_A ^ (m_dwMask_A >> 1)) | m_dwRot1_A;
            
            // Clock shift register B once
            if (m_dwLFSR_B & 0x00000001)
            {
                // The LSB of LFSR B is "1". XOR LFSR B with its feedback mask
                m_dwLFSR_B = (m_dwLFSR_B ^ (m_dwMask_B >> 1)) | m_dwRot1_B;
                dwOut_B = 0x00000001;
            }
            else
            {
                // The LSB of LFSR B is "0". Rotate the LFSR contents once
                m_dwLFSR_B = (m_dwLFSR_B >> 1) & m_dwRot0_B;
                dwOut_B = 0x00000000;
            }
        }
        else
        {
            // The LSB of LFSR A is "0". Rotate the LFSR contents once
            m_dwLFSR_A = (m_dwLFSR_A >> 1) & m_dwRot0_A;

            // Clock shift register C once
            if (m_dwLFSR_C & 0x00000001)
            {
                // The LSB of LFSR C is "1". XOR LFSR C with its feedback mask
                m_dwLFSR_C = (m_dwLFSR_C ^ (m_dwMask_C >> 1)) | m_dwRot1_C;
                dwOut_C = 0x00000001;
            }
            else
            {
                // The LSB of LFSR C is "0". Rotate the LFSR contents once
                m_dwLFSR_C = (m_dwLFSR_C >> 1) & m_dwRot0_C;
                dwOut_C = 0x00000000;
            }
        }

        // XOR the output from LFSRs B and C and rotate it into the right bit of bCrypto
        bCrypto = (bCrypto << 1) | (BYTE)(dwOut_B ^ dwOut_C);
    }

    // XOR the resulting character with the input character to encrypt/decrypt it
    *pbTarget = *pbTarget ^ bCrypto;
}

////////////////////////////////////////////////////////////////////////////////

void CCryptor::WeakTransformByte (LPBYTE pbTarget)
{
    DWORD dwOut_A = 0x00000000;
    BYTE bCrypto = 0;

    // Cycle the LFSR eight times to get eight pseudo-random bits.
    // Assemble these into a single random character (bCrypto).

    for (int n = 0; n < 8; n++)
    {
        if (m_dwLFSR_A & 0x00000001)
        {
            // The least-significant bit of LFSR A is "1". XOR LFSR A with its feedback mask
            m_dwLFSR_A = (m_dwLFSR_A ^ (m_dwMask_A >> 1)) | m_dwRot1_A;
            dwOut_A = 0x00000001;
        }
        else
        {
            // The LSB of LFSR A is "0". Rotate the LFSR contents once
            m_dwLFSR_A = (m_dwLFSR_A >> 1) & m_dwRot0_A;
            dwOut_A = 0x00000000;
        }

        // Rotate the output bit into bCrypto.
        bCrypto = (bCrypto << 1) | (BYTE)dwOut_A;
    }

    // XOR the resulting character with the input character to encrypt/decrypt it
    *pbTarget = *pbTarget ^ bCrypto;
}

////////////////////////////////////////////////////////////////////////////////
