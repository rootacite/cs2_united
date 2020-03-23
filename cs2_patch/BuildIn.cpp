#include "pch.h"
#include "BuildIn.h"
#include "Data.h"
struct UxData {
    DWORD a;
    DWORD b;
    WCHAR c;
};

/*********声明符号***********/
extern HMODULE hMod;
extern "C" extern DLLAPI wchar_t ms_str[3096];
extern "C" extern DLLAPI int nID;
extern "C" extern DLLAPI DWORD tPid;

extern MicroData* Index;
extern MicroBinary* Data;

DWORD m_Addr;
extern "C" extern DLLAPI bool IsSuccess;

char (*Sur_Sub)(const char* , UxData& , ULONG ) = (char(*)(const char*, UxData&, ULONG))0;//real function point
char Fake_Sub(const char*, UxData&, ULONG);//hooked function point

bool start_falg = false;
HMODULE SelfHandle = NULL;
extern char IpfData[16];
#define PutInt(a) _itoa_s(a,IpfData,10);MessageBoxA(0,IpfData,"num",0);

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

UINT InjectSelfTo(LPCSTR inptr)
{
   // MessageBoxA(0, inptr,"",0);
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

    ResumeThread(info->hThread);

    WaitForSingleObject(hHookStart, 0xFFFFFFFF);
    delete info;
    lstrcpyW(ms_str, L" ");

    return  (UINT)currentThread;
}
DWORD lecx;


char Fake_Sub(const char* a, UxData& b,ULONG c)
{
    __asm {
        mov dword ptr[lecx], ecx
    }
    // SetWindowTextW(m_hWnd, L"����hook");
 //   MessageBoxA(0,"","",0);
   // TranSpt(lecx);

    MessageBoxA(0,(LPCSTR)a,"",0);

    //  SetWindowTextW(m_hWnd, L"hook����");
    __asm {
        mov ecx, dword ptr[lecx]
    }
    return Sur_Sub("123456788909",b,c);
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
 

void LoadExerte()
{
    Index = new MicroData(L"Index.ax", sizeof(IndexData));
    Data = new MicroBinary(L"Data.ax");
    Index->Load();
    Data->Load();

    HMODULE hmRent = NULL;
    while (!hmRent) {
        hmRent = GetModuleHandleA("resident.dll");
    }

    m_Addr = (DWORD)::GetProcAddress(hmRent, "?printSub@RetouchPrintManager@@AAE_NPBDAAVUxPrintData@@K@Z");
    Sur_Sub = (char(*)(const char*, UxData&, ULONG))m_Addr;

    start();
}
