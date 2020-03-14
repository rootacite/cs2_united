// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include "Data.h"
#include "BuildIn.h"

#pragma data_seg("PublicData")      // 声明共享数据段，并命名该数据段

extern "C" DLLAPI double saveProcess = 0.0;
HMODULE hMod = NULL;
extern "C" DLLAPI wchar_t ms_str[3096] = { 0 };
extern "C" DLLAPI int nID = 0;
extern "C" DLLAPI DWORD tPid = 0;

#pragma data_seg()
#pragma comment(linker, "/section:PublicData,rws")


MicroData* Index = NULL;
MicroBinary* Data = NULL;

char IpfData[16];
#define PutInt(a) _itoa_s(a,IpfData,10);MessageBoxA(0,IpfData,"num",0);


extern HMODULE SelfHandle ;
extern bool start_falg ;



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SelfHandle = hModule;
        hMod = hModule;
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        if (start_falg)
            end();
        break;
    }
    return TRUE;
}

