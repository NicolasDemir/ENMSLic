#pragma once
extern "C"
{
	struct TempLicInfo;
	int __declspec(dllexport) CheckOut(char* query, char* presponse, char* serial, int* meters, int* users, int* connections, int* update, int* product, int* version, char* filepath);
	int __declspec(dllexport) CheckOut2(char* query, char* presponse, char* serial, int* meters, int* users, int* connections, int* update, int* product, int* version, int* TempLicense, TempLicInfo* TempInfo, char* filepath);
}



