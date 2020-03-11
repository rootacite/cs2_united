#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

#include "MicroFile.h"
#pragma comment(lib,"MicroFile.lib")

#include "include/detours.h"
#pragma comment (lib,"detours.lib")

#include <stdlib.h>
#include <string>
using namespace std;

#define DLLAPI __declspec(dllexport)

struct IndexData {
    int Id;
    DWORD JpBass;
    DWORD JpLength;
    DWORD CnBass;
    DWORD CnLength;
};



char IpfData[16];
#define PutInt(a) _itoa_s(a,IpfData,10);MessageBoxA(0,IpfData,"num",0);




