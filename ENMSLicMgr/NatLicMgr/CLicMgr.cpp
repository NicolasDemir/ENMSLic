#include "pch.h"
#include "CLicMgr.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>
#include <fstream> // For file handling
#include <vector>

using namespace std;

#include "toolbox.h"
#include "EncryptionMgr.h"
#include "SoftLicenseMgr.h"


struct TempLicInfo
{
	int ExpirationYear;
	int ExpirationMonth;
	int ExpirationDay;
	int ActivationYear;
	int ActivationMonth;
	int ActivationDay;
};


extern "C" __declspec(dllexport) int CheckOut(char* query, char* presponse, char* serial, int* meters, int* users, int* connections, int* update, int* product, int* version, char* filepath)
{
	string afile = filepath;
	string afileOutput = "liclog.txt";
	

	CSoftLicenseMgr helper;
	_LicOptions options;
	int nreturn = helper.CheckoutLicense(filepath, afileOutput, options, true);

	string str(query);

	CustomCypher    acypher;
	string sz = acypher.HashInput(str);
	strcpy_s(presponse, sz.length() + 1, sz.c_str()); 

	strcpy_s(serial, options.m_serial.length() + 1, options.m_serial.c_str());
	*meters = options.m_meters;
	*users = options.m_users;
	*connections = options.m_connections;
	*update = options.m_update;
	*product = options.m_product;
	*version = options.m_version;

	if (nreturn >= ERROR_LICENSE_EXPIRED)
		nreturn = ERROR_NOERROR;

	return nreturn;
}

extern "C" __declspec(dllexport) int CheckOut2(char* query, char* presponse, char* serial, int* meters, int* users, int* connections, int* update, int* product, int* version, int* TempLicense, TempLicInfo* TempInfo, char* filepath)
{
	string afile = filepath;
	string afileOutput = "liclog.txt";

	CSoftLicenseMgr helper;
	_LicOptions options;
	int nreturn = helper.CheckoutLicense(filepath, afileOutput, options, false);

	string str(query);

	CustomCypher    acypher;
	string sz = acypher.HashInput(str);
	strcpy_s(presponse, sz.length() + 1, sz.c_str());

	strcpy_s(serial, options.m_serial.length() + 1, options.m_serial.c_str());
	*meters = options.m_meters;
	*users = options.m_users;
	*connections = options.m_connections;
	*update = options.m_update;
	*product = options.m_product;
	*version = options.m_version;
	*TempLicense = (options.m_TemporaryLicenseDuration > 0) ? 1 : 0;
	TempLicInfo info;
	info.ExpirationYear = options.m_expirationYear;
	info.ExpirationMonth = options.m_expirationMonth;
	info.ExpirationDay = options.m_expirationDay;
	info.ActivationYear = options.m_activationYear;
	info.ActivationMonth = options.m_activationMonth;
	info.ActivationDay = options.m_activationDay;

	*TempInfo = info;

	return nreturn;
}


