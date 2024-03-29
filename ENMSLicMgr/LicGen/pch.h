

/*
// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#endif //PCH_H
*/
// stdafx.h : Include file for standard system include files, or project specific
//            include files that are used frequently, but are changed infrequently.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

// Workaround for <gdiplus.h> that does not compile when NOMINMAX is defined
// Note that <afxcontrolbars.h> also includes <gdiplus.h>
#ifdef NOMINMAX
#include <algorithm>
namespace Gdiplus
{
    using std::min;
    using std::max;
}
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxcmn.h>         // MFC support for Windows Common Controls
#include <afxcontrolbars.h> // MFC support for ribbons and control bars
#include <afxdialogex.h>

#define SSP_ENABLE_ADVANCED_FEATURES    // Enable support for advanced features of Sentinel SuperPro
#define SSP_ENABLE_WRITING_FEATURES     // Enable support for writing features of Sentinel SuperPro

// Enable the application to use visual styles
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
