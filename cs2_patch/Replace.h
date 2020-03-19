#pragma once
#include <Windows.h>
#include "Data.h"
#define DLLAPI __declspec(dllexport)
typedef unsigned long long QWORD;
struct TESTDATA {
    DWORD ForCast;//������;������
    wchar_t nChar;
    WORD Flag1;//������;����ΪBAAD
    DWORD Flag2;//FFC8C8C8
    DWORD Flag3;//FF352519
    DWORD nCross;
    DWORD nLine;
    DWORD Flag4; //0000001A
    DWORD Flagn4;//0000001A
    QWORD Flag5; //0
    DWORD Flag6; //FFFFFFFF
    DWORD count; //mayba it shows the position of string
    DWORD Flag7;
    DWORD Flag8; //0000000100000001
    DWORD Flagn8;
    DWORD FlagnW;//1
    QWORD FlagnQ;//0
    DWORD ntime;//1E
    DWORD ntime2;//1E
    DWORD nWQ;//0
    DWORD Over1;
    QWORD Over2;
};
extern "C" DLLAPI  void BuildCV(TESTDATA* pcv, TESTDATA flt, DWORD posinCross, DWORD posInLine, DWORD m_count, WCHAR nchar);
void ApplyStringToCV(LPCWSTR ws, DWORD pDf);
extern "C" DLLAPI int TranSplete(DWORD lp);
