#pragma once
#include <Windows.h>
#define DLLAPI __declspec(dllexport)
BOOL InjectDLL(HANDLE hProcess, LPCWSTR dllFilePathName);//Inject dll to the signal process
extern "C" DLLAPI HANDLE InjectSelfTo(LPCSTR inptr);//Inject self
signed int Fake_Sub();//hooked function point


DWORD WINAPI fGetGlyphOutlineW(
    _In_ HDC hdc,
    _In_ UINT uChar,
    _In_ UINT fuFormat,
    _Out_ LPGLYPHMETRICS lpgm,
    _In_ DWORD cjBuffer,
    _Out_writes_bytes_opt_(cjBuffer) LPVOID pvBuffer,
    _In_ CONST MAT2* lpmat2
);
BOOL  WINAPI fTextOutW(_In_ HDC hdc, _In_ int x, _In_ int y, _In_reads_(c) LPCWSTR lpString, _In_ int c);


extern "C" DLLAPI void start();//start hook
extern "C" DLLAPI void end();//end hook

extern "C" DLLAPI void start_g();//start hook
extern "C" DLLAPI void end_g();//end hook

extern "C" DLLAPI void start_t();//start hook
extern "C" DLLAPI void end_t();//end hook

extern "C" DLLAPI void StartReplace();
extern "C" DLLAPI void EndReplace();

extern "C" DLLAPI void ChangeGToT();
extern "C" DLLAPI void ChangeTToG();

extern "C" DLLAPI void LoadExerte();

PVOID GetProcAddressEx(HANDLE hProc, HMODULE hModule, LPCSTR lpProcName);