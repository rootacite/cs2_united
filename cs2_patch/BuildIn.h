#pragma once
#include <Windows.h>
#define DLLAPI __declspec(dllexport)


BOOL InjectDLL(HANDLE hProcess, LPCWSTR dllFilePathName);//Inject dll to the signal process
extern "C" DLLAPI HANDLE InjectSelfTo(wchar_t inptr[]);//Inject self


signed int Fakesub_5FC1C0();//hooked function point

extern "C" DLLAPI void start();//start hook
void end();//end hook