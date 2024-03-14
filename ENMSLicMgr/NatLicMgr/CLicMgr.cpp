#include "pch.h"
#include "CLicMgr.h"


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>
#include <fstream> // For file handling
#include <vector>

using namespace std;

#include "toolbox.h"
#include "EncryptionMgr.h"
#include "softlichelper.h"





extern "C" __declspec(dllexport) int CheckOut(char* query, char* presponse, char* serial, int* meters, int* users, int* connections, int* update, int* product, int* version, char* filepath)
{
	string afile = filepath;
	string afileOutput = "liclog.txt";

	CSoftLicHelper helper;
	_LicOptions options;
	int nreturn = helper.CheckoutLicense(filepath, afileOutput, options);

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

	return nreturn;
}

