#include "pch.h"
#include "BuildIn.h"
#include "Data.h"

extern MicroData Index;
extern MicroBinary Data;

extern HMODULE hMod;
extern "C" extern DLLAPI wchar_t ms_str[3096];
extern "C" extern DLLAPI int nID;


signed int (*sub_5FC1C0)() = (signed int(*)(void))0x5FC1C0;//real function point
HMODULE SelfHandle = NULL;
bool start_falg = false;

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

DLLAPI HANDLE InjectSelfTo(wchar_t inptr[])
{

    HANDLE currentThread = NULL;

    LPPROCESS_INFORMATION info = new PROCESS_INFORMATION;
    STARTUPINFO si = { sizeof(si) };
    do {
        wchar_t m_CommandLine[] = L"";
        BOOL hF = CreateProcess(inptr, NULL,
            NULL, NULL, FALSE,
            CREATE_SUSPENDED, NULL, NULL, &si, info);
        if (!hF) {
            MessageBox(0, L"创建进程失败", L"错误", MB_ICONERROR);
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
    } while (0);

    HANDLE hHookStart = CreateRemoteThread(info->hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)
        ::GetProcAddress(hMod, "start"), 0, 0, NULL);

    if (!hHookStart)
    {
        MessageBox(0, L"无法创建远程线程(IAT HOOK)", L"错误", MB_ICONERROR);
        return  0;
    }
    WaitForSingleObject(hHookStart, 0);
    ResumeThread(info->hThread);
    delete info;

    lstrcpyW(ms_str, L" ");

    Index.Load();
    Data.Load();
    return  currentThread;
}

signed int Fakesub_5FC1C0()
{
    DWORD leax, lebx, lecx, ledx, lesi, ledi;
    __asm {
        mov dword ptr[leax], eax
        mov dword ptr[lebx], ebx
        mov dword ptr[lecx], ecx
        mov dword ptr[ledx], edx
        mov dword ptr[lesi], esi
        mov dword ptr[ledi], edi
    }
    // SetWindowTextW(m_hWnd, L"进入hook");
    ((int(*)(DWORD))::GetProcAddress(SelfHandle, "TranSplete"))(lecx);
    //  SetWindowTextW(m_hWnd, L"hook结束");
    __asm {
        mov  eax, dword ptr[leax]
        mov  ebx, dword ptr[lebx]
        mov ecx, dword ptr[lecx]
        mov edx, dword ptr[ledx]
        mov  esi, dword ptr[lesi]
        mov edi, dword ptr[ledi]
    }

    return sub_5FC1C0();
}

DLLAPI void start()
{

    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)sub_5FC1C0, Fakesub_5FC1C0);
    DetourTransactionCommit();

    start_falg = TRUE;
}

void end()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)sub_5FC1C0, Fakesub_5FC1C0);
    DetourTransactionCommit();
}
