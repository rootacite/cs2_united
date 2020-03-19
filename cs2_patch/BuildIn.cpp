#include "pch.h"
#include "BuildIn.h"
#include "Data.h"
#include "Replace.h"

/*********声明符号***********/

extern "C" extern DLLAPI wchar_t ns_str[6192];
extern HMODULE hMod;
extern "C" extern DLLAPI wchar_t ms_str[3096];
extern "C" extern DLLAPI int nID;
extern "C" extern DLLAPI DWORD tPid;

extern MicroData* Index;
extern MicroBinary* Data;

extern "C" extern DLLAPI DWORD m_Addr;
extern "C" extern DLLAPI DWORD VioMode;
extern "C" extern DLLAPI bool IsSuccess;
signed int (*Sur_Sub)() = (signed int(*)(void))m_Addr;//real function point
HMODULE SelfHandle = NULL;

bool start_falg = false;
bool start_g_flag = false;
bool start_t_flag = false;

extern char IpfData[16];
#define PutInt(a) _itoa_s(a,IpfData,10);MessageBoxA(0,IpfData,"num",0);


TESTDATA* pNewDf = NULL;
PVOID GetProcAddressEx(HANDLE hProc, HMODULE hModule, LPCSTR lpProcName)
{
    PVOID pAddress = NULL;
    SIZE_T OptSize;
    IMAGE_DOS_HEADER DosHeader;
    SIZE_T ProcNameLength = lstrlenA(lpProcName) + sizeof(CHAR);//'\0'
    //读DOS头
    if (ReadProcessMemory(hProc, hModule, &DosHeader, sizeof(DosHeader), &OptSize))
    {
        IMAGE_NT_HEADERS NtHeader;
        //读NT头
        if (ReadProcessMemory(hProc, (PVOID)((SIZE_T)hModule + DosHeader.e_lfanew), &NtHeader, sizeof(NtHeader), &OptSize))
        {
            IMAGE_EXPORT_DIRECTORY ExpDir;
            SIZE_T ExportVirtualAddress = NtHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
            //读输出表
            if (ExportVirtualAddress && ReadProcessMemory(hProc, (PVOID)((SIZE_T)hModule + ExportVirtualAddress), &ExpDir, sizeof(ExpDir), &OptSize))
            {
                if (ExpDir.NumberOfFunctions)
                {
                    //x64待定:地址数组存放RVA的数据类型是4字节还是8字节???
                    SIZE_T* pProcAddressTable = (SIZE_T*)GlobalAlloc(GPTR, ExpDir.NumberOfFunctions * sizeof(SIZE_T));
                    //读函数地址表
                    if (ReadProcessMemory(hProc, (PVOID)((SIZE_T)hModule + ExpDir.AddressOfFunctions), pProcAddressTable, ExpDir.NumberOfFunctions * sizeof(PVOID), &OptSize))
                    {
                        //x64待定:名称数组存放RVA的数据类型是4字节还是8字节???
                        SIZE_T* pProcNamesTable = (SIZE_T*)GlobalAlloc(GPTR, ExpDir.NumberOfNames * sizeof(SIZE_T));
                        //读函数名称表
                        if (ReadProcessMemory(hProc, (PVOID)((SIZE_T)hModule + ExpDir.AddressOfNames), pProcNamesTable, ExpDir.NumberOfNames * sizeof(PVOID), &OptSize))
                        {
                            CHAR* pProcName = (CHAR*)GlobalAlloc(GPTR, ProcNameLength);
                            //遍历函数名称
                            for (DWORD i = 0; i < ExpDir.NumberOfNames; i++)
                            {
                                if (ReadProcessMemory(hProc, (PVOID)((SIZE_T)hModule + pProcNamesTable[i]), pProcName, ProcNameLength, &OptSize))
                                {
                                    if (RtlEqualMemory(lpProcName, pProcName, ProcNameLength))
                                    {
                                        //x64待定:函数在地址数组索引的数据类型是2字节还是???
                                        WORD NameOrdinal;
                                        //获取函数在地址表的索引
                                        if (ReadProcessMemory(hProc, (PVOID)((SIZE_T)hModule + ExpDir.AddressOfNameOrdinals + sizeof(NameOrdinal) * i), &NameOrdinal, sizeof(NameOrdinal), &OptSize))
                                        {
                                            pAddress = (PVOID)((SIZE_T)hModule + pProcAddressTable[NameOrdinal]);
                                        }
                                        break;//for
                                    }
                                }
                            }
                            GlobalFree(pProcName);
                        }
                        GlobalFree(pProcNamesTable);
                    }
                    GlobalFree(pProcAddressTable);
                }
            }
        }
    }
    return pAddress;
}
BOOL InjectDLL(HANDLE hProcess, LPCWSTR dllFilePathName)
{
    if (!hProcess)
        return FALSE;
    int cch = 1 + lstrlenW(dllFilePathName);
    int cb = cch * sizeof(wchar_t);
    LPWSTR PszLibFileRemote = (LPWSTR)VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
    if (!PszLibFileRemote)
        return FALSE;
    if (!WriteProcessMemory(hProcess, PszLibFileRemote, (LPVOID)dllFilePathName, cb, NULL))
        return FALSE;
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)
        LoadLibraryW, PszLibFileRemote, 0, NULL);
    if (!hThread) return FALSE;
    WaitForSingleObject(hThread, INFINITE);
    return TRUE;
}

DWORD(WINAPI* pGetGlyphOutlineW)(
    _In_ HDC hdc,
    _In_ UINT uChar,
    _In_ UINT fuFormat,
    _Out_ LPGLYPHMETRICS lpgm,
    _In_ DWORD cjBuffer,
    _Out_writes_bytes_opt_(cjBuffer) LPVOID pvBuffer,
    _In_ CONST MAT2* lpmat2
    ) = GetGlyphOutlineW;
BOOL(WINAPI* pTextOutW)(_In_ HDC hdc, _In_ int x, _In_ int y, _In_reads_(c) LPCWSTR lpString, _In_ int c) = TextOutW;

/*****************************/
int(*TranSpt)(DWORD);
HANDLE InjectSelfTo(LPCSTR inptr)
{
 //   MessageBoxA(0, inptr,"",0);
    HANDLE currentThread = NULL;
    LPPROCESS_INFORMATION info = new PROCESS_INFORMATION;
    STARTUPINFOA si = { sizeof(si) };
    do {
        BOOL hF = CreateProcessA(inptr, NULL,
            NULL, NULL, FALSE,
            CREATE_SUSPENDED, NULL, NULL, &si, info);
        if (!hF) {
            MessageBoxA(0, "Failed to create Process!", inptr, MB_ICONERROR);
            return 0;
        }
        //   MessageBox(0, L"1", L"", 0);
        wchar_t m_Path[MAX_PATH];
        GetModuleFileName(hMod, m_Path, MAX_PATH);
        if (!InjectDLL(info->hProcess, m_Path)) {
            MessageBoxA(0, "", "", 0);
            return 0;
        }
        currentThread = info->hThread;
        tPid = info->dwProcessId;
    } while (0);
    HANDLE hHookStart = CreateRemoteThread(info->hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)
        ::GetProcAddressEx(info->hProcess, hMod, "LoadExerte"), 0, 0, NULL);
    if (!hHookStart)
    {
        PutInt(GetLastError());
        MessageBox(0, L"Failed to create remote thread(LoadExerte)", L"error", MB_ICONERROR);
        return 0;
    }
    WaitForSingleObject(hHookStart, 0xFFFFFFFF);
    ResumeThread(info->hThread);
    delete info;
    lstrcpyW(ms_str, L" ");
    ns_str[0] = L'\0';

    return  currentThread;
}
DWORD lecx;
signed int Fake_Sub()
{
    __asm {
        mov dword ptr[lecx], ecx
    }
    // SetWindowTextW(m_hWnd, L"����hook");
 //   MessageBoxA(0,"","",0);
    TranSpt(lecx);

    //  SetWindowTextW(m_hWnd, L"hook����");
    __asm {
        mov ecx, dword ptr[lecx]
    }
    return Sur_Sub();
}
DWORD WINAPI Th(LPVOID lp) {
    char a[16];
    while (1) {
        _itoa_s(nID, a, 10);
        SetWindowTextA(GetForegroundWindow(), a);
        Sleep(100);
    }
    return 0;
}
DWORD WINAPI fGetGlyphOutlineW(
    _In_ HDC hdc,
    _In_ UINT uChar,
    _In_ UINT fuFormat,
    _Out_ LPGLYPHMETRICS lpgm,
    _In_ DWORD cjBuffer,
    _Out_writes_bytes_opt_(cjBuffer) LPVOID pvBuffer,
    _In_ CONST MAT2* lpmat2
)
{
    wstring loca = L"";
    loca += (WCHAR)uChar;
    if(lstrlenW(ns_str)>=5999)
        ns_str[0] = L'\0';
    lstrcatW(ns_str, loca.c_str());
    return pGetGlyphOutlineW(hdc, L' ', fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}
BOOL  WINAPI fTextOutW(_In_ HDC hdc, _In_ int x, _In_ int y, _In_reads_(c) LPCWSTR lpString, _In_ int c)
{
    return pTextOutW(hdc, x, y, L"\0", c);
}


void start()
{
   // MessageBoxA(0,"b","",0);
    if (start_falg)
        return;
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)Sur_Sub, Fake_Sub);
    DetourTransactionCommit();
 
    start_falg = TRUE;
   // CreateThread(0,0,Th,0,0,0);
}
void end()
{
   // MessageBoxA(0,"a","",0);
    if (!start_falg)
        return;
    start_falg = FALSE;
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)Sur_Sub, Fake_Sub);
    DetourTransactionCommit();
 
}
 void start_g()
{
    if (start_g_flag)
        return;
    start_g_flag = TRUE;
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)pGetGlyphOutlineW, fGetGlyphOutlineW);
    DetourTransactionCommit();
}
 void end_g()
{
    if (!start_g_flag)
        return;
    start_g_flag = FALSE;
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)pGetGlyphOutlineW, fGetGlyphOutlineW);
    DetourTransactionCommit();
}
 void start_t()
 {
     if (start_t_flag)
         return;
     start_t_flag = TRUE;
     DetourRestoreAfterWith();
     DetourTransactionBegin();
     DetourUpdateThread(GetCurrentThread());
     DetourAttach(&(PVOID&)pTextOutW, fTextOutW);
     DetourTransactionCommit();
 }
 void end_t()
 {
     if (!start_t_flag)
         return;
     start_t_flag = FALSE;
     DetourTransactionBegin();
     DetourUpdateThread(GetCurrentThread());
     DetourDetach(&(PVOID&)pTextOutW, fTextOutW);
     DetourTransactionCommit();
 }


void StartReplace()
{
    HANDLE terp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tPid);
    HANDLE hHookStart = CreateRemoteThread(terp, NULL, 0, (LPTHREAD_START_ROUTINE)
        ::GetProcAddressEx(terp,hMod, "start"), 0, 0, NULL);
    if (!hHookStart)
    {
        MessageBox(0, L"Failed to create remote thread(StartReplace)", L"error", MB_ICONERROR);
        return;
    }
    WaitForSingleObject(hHookStart, 0xFFFFFFFF);
    if (VioMode == 0) {

        HANDLE hgHookStart = CreateRemoteThread(terp, NULL, 0, (LPTHREAD_START_ROUTINE)
            ::GetProcAddressEx(terp, hMod, "end_g"), 0, 0, NULL);
        if (!hgHookStart)
        {
            MessageBox(0, L"Failed to create remote thread(StartReplace)", L"error", MB_ICONERROR);
            return;
        }
        WaitForSingleObject(hgHookStart, 0xFFFFFFFF);
    }
    if (VioMode == 1) {

        HANDLE hgHookStart = CreateRemoteThread(terp, NULL, 0, (LPTHREAD_START_ROUTINE)
            ::GetProcAddressEx(terp, hMod, "end_t"), 0, 0, NULL);
        if (!hgHookStart)
        {
            MessageBox(0, L"Failed to create remote thread(StartReplace)", L"error", MB_ICONERROR);
            return;
        }
        WaitForSingleObject(hgHookStart, 0xFFFFFFFF);
    }
    CloseHandle(terp);

}
void EndReplace()
 {
     HANDLE terp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tPid);
     HANDLE hHookStart = CreateRemoteThread(terp, NULL, 0, (LPTHREAD_START_ROUTINE)
         ::GetProcAddressEx(terp, hMod, "end"), 0, 0, NULL);
    if (!hHookStart)
    {
        MessageBox(0, L"Failed to create remote thread(EndReplace)", L"error", MB_ICONERROR);
        return;
    }
    WaitForSingleObject(hHookStart, 0xFFFFFFFF);

    if (VioMode == 0) {
        HANDLE hgHookStart = CreateRemoteThread(terp, NULL, 0, (LPTHREAD_START_ROUTINE)
            ::GetProcAddressEx(terp, hMod, "start_g"), 0, 0, NULL);
        if (!hgHookStart)
        {
            MessageBox(0, L"Failed to create remote thread(EndReplace)", L"error", MB_ICONERROR);
            return;
        }
        WaitForSingleObject(hgHookStart, 0xFFFFFFFF);

    }
    if (VioMode == 1) {

        HANDLE hgHookStart = CreateRemoteThread(terp, NULL, 0, (LPTHREAD_START_ROUTINE)
            ::GetProcAddressEx(terp, hMod, "start_t"), 0, 0, NULL);
        if (!hgHookStart)
        {
            MessageBox(0, L"Failed to create remote thread(StartReplace)", L"error", MB_ICONERROR);
            return;
        }
        WaitForSingleObject(hgHookStart, 0xFFFFFFFF);
    }
    CloseHandle(terp);
 }
void ChangeGToT()
 {
      HANDLE terp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tPid);
      HANDLE hgHookStart = CreateRemoteThread(terp, NULL, 0, (LPTHREAD_START_ROUTINE)
          ::GetProcAddressEx(terp, hMod, "end_g"), 0, 0, NULL);
      if (!hgHookStart)
      {
          MessageBox(0, L"Failed to create remote thread(StartReplace)", L"error", MB_ICONERROR);
          return;
      }
      WaitForSingleObject(hgHookStart, 0xFFFFFFFF);

      HANDLE htHookStart = CreateRemoteThread(terp, NULL, 0, (LPTHREAD_START_ROUTINE)
          ::GetProcAddressEx(terp, hMod, "start_t"), 0, 0, NULL);
      if (!htHookStart)
      {
          MessageBox(0, L"Failed to create remote thread(StartReplace)", L"error", MB_ICONERROR);
          return;
      }
      WaitForSingleObject(htHookStart, 0xFFFFFFFF);
      CloseHandle(terp);
 }
void ChangeTToG()
 {
      HANDLE terp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tPid);
      HANDLE hgHookStart = CreateRemoteThread(terp, NULL, 0, (LPTHREAD_START_ROUTINE)
          ::GetProcAddressEx(terp, hMod, "end_t"), 0, 0, NULL);
      if (!hgHookStart)
      {
          MessageBox(0, L"Failed to create remote thread(StartReplace)", L"error", MB_ICONERROR);
          return;
      }
      WaitForSingleObject(hgHookStart, 0xFFFFFFFF);

      HANDLE htHookStart = CreateRemoteThread(terp, NULL, 0, (LPTHREAD_START_ROUTINE)
          ::GetProcAddressEx(terp, hMod, "start_g"), 0, 0, NULL);
      if (!htHookStart)
      {
          MessageBox(0, L"Failed to create remote thread(StartReplace)", L"error", MB_ICONERROR);
          return;
      }
      WaitForSingleObject(htHookStart, 0xFFFFFFFF);
      CloseHandle(terp);
 }
void LoadExerte()
{
    Index = new MicroData(L"Index.ax", sizeof(IndexData));
    Data = new MicroBinary(L"Data.ax");
    Index->Load();
    Data->Load();

    TranSpt = (int(*)(DWORD))::GetProcAddress(SelfHandle, "TranSplete");
}
