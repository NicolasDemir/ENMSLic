////////////////////////////////////////////////////////////////////////////////
//
// HashAlgorithms.h
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __HASH_ALGORITHMS_H__
#define __HASH_ALGORITHMS_H__

#pragma once

////////////////////////////////////////////////////////////////////////////////
// MD5 (Message Digest 5) 128-bit hashing algorithm.

#define MD5_MSG_DIGEST_SIZE         16  // Size in bytes of a MD5 structure
#define MD5_MSG_DIGEST_STRING_LEN   32  // Length in characters of a printable MD5 string

typedef struct
{
    BYTE abBytes [MD5_MSG_DIGEST_SIZE]; // A 128-bit message digest
}
MD5_MSG_DIGEST;

void ComputeHashMD5 (LPCTSTR pszInput, MD5_MSG_DIGEST * pMsgDigest);
void ComputeHashMD5 (LPCTSTR pszInput, LPTSTR pszMsgDigestString, DWORD cchMsgDigestString);

#ifdef _MFC_VER
CString ComputeHashMD5 (LPCTSTR pszInput);
#endif

////////////////////////////////////////////////////////////////////////////////

#endif  // __HASH_ALGORITHMS_H__