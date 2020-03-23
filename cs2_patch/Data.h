#pragma once
#include <Windows.h>
#define DLLAPI __declspec(dllexport)
extern "C" DLLAPI void CreateDataExport(WCHAR scr[],WCHAR data[]);
BOOL CreateDataByID(int ID, LPCWSTR jpBuff, int ljp, LPCWSTR cnBuffer, int lcn);
BOOL GetDataByJP(int* ID, LPCWSTR jpBuff, LPWSTR cnBuffer);
BOOL GetDataByID(int ID, LPWSTR jpBuff, LPWSTR cnBuffer);
int GEtLargestID();
extern "C" DLLAPI int CreateDataExportEx(LPVOID path);