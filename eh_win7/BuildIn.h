#pragma once
#include <Windows.h>
#define DLLAPI __declspec(dllexport)
BOOL InjectDLL(HANDLE hProcess, LPCWSTR dllFilePathName);//Inject dll to the signal process
extern "C" DLLAPI UINT  InjectSelfTo(LPCSTR inptr);//Inject self





extern "C" DLLAPI void start();//start hook
extern "C" DLLAPI void end();//end hook

extern "C" DLLAPI void LoadExerte();


PVOID GetProcAddressEx(HANDLE hProc, HMODULE hModule, LPCSTR lpProcName);