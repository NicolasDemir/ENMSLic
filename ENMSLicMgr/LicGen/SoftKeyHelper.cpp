#include "pch.h"


#include <iostream>
#include <Windows.h>
#include <iphlpapi.h>
#include <string>

using namespace std;

#pragma comment(lib, "IPHLPAPI.lib")

#include "SoftKeyHelper.h"


string CSoftKeyHelper::GetTargetName()
{
    string strreturn = "";
    return strreturn;
}

string CSoftKeyHelper::GetMacAddress()
{
    string strreturn = "";
 
    return strreturn;

}

string CSoftKeyHelper::GetFingerPrint(int& nError)
{
    string strreturn = "";
    string sMachine = GetTargetName();
    string sMac = GetMacAddress();

    nError = 0;
    return strreturn;
}
