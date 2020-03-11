#pragma once
#include <Windows.h>
#define DLLAPI __declspec(dllexport)



BOOL CreateDataByIDEx(LPCWSTR name, LPCWSTR name2, int ID, LPCWSTR jpBuff, int ljp, LPCWSTR cnBuffer, int lcn);
extern "C" DLLAPI void CreateDataExport(WCHAR data[]);
BOOL CreateDataByID(int ID, LPCWSTR jpBuff, int ljp, LPCWSTR cnBuffer, int lcn);
BOOL GetDataByJP(int* ID, LPCWSTR jpBuff, LPWSTR cnBuffer);
BOOL GetDataByID(int ID, LPWSTR jpBuff, LPWSTR cnBuffer);
int GEtLargestID();