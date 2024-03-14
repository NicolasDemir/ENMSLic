#pragma once
extern "C"
{
	int __declspec(dllexport) CheckOut(char* query, char* presponse, char* serial, int* meters, int* users, int* connections, int* update, int* product, int* version, char* filepath);
}



