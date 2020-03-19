// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "Data.h"
#include "BuildIn.h"

#pragma data_seg("PublicData")      // 声明共享数据段，并命名该数据段
//代表储存数据的进度
extern "C" DLLAPI double saveProcess = 0.0;
//共享模块句柄
HMODULE hMod = NULL;
//储存原文字符串
extern "C" DLLAPI wchar_t ms_str[3096] = { 0 };
//储存GetGlyphoutLine的输出记录
extern "C" DLLAPI wchar_t ns_str[6192] = { 0 };
//储存译文字符串
extern "C" DLLAPI wchar_t cn_str[1024] = { 0 };
//当前的ID
extern "C" DLLAPI int nID = -1;
//目标进程PID
extern "C" DLLAPI DWORD tPid = 0;
//函数地址
extern "C" DLLAPI DWORD m_Addr = 0xFFFFFFFF;
//暴力提取的模式
extern "C" DLLAPI DWORD VioMode = 0;
//是否成功获取到译文
extern "C" DLLAPI bool IsSuccess = false;
//框架备用
extern "C" DLLAPI bool blockRestoreSrc = false;
#pragma data_seg()
#pragma comment(linker, "/section:PublicData,rws")


MicroData* Index = NULL;
MicroBinary* Data = NULL;
char IpfData[16];
#define PutInt(a) _itoa_s(a,IpfData,10);MessageBoxA(0,IpfData,"num",0);
extern HMODULE SelfHandle ;
extern bool start_falg ;
extern "C" DLLAPI void TestA(){}
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
