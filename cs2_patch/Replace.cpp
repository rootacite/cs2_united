#include "pch.h"
#include "Replace.h"

int npID = 0;
extern HMODULE hModL;
extern "C" extern DLLAPI wchar_t ms_str[3096];
extern "C" extern DLLAPI int nID;

void BuildCV(TESTDATA* pcv, TESTDATA flt, DWORD posinCross, DWORD posInLine, DWORD m_count, WCHAR nchar)
{
    pcv->ForCast = flt.ForCast;
    pcv->nLine = posinCross;
    pcv->nChar = nchar;
    pcv->nCross = posInLine;
    pcv->Flag1 = flt.Flag1;
    pcv->Flag2 = flt.Flag2;
    pcv->Flag3 = flt.Flag3;
    pcv->Flag4 = flt.Flag4;
    pcv->Flagn4 = flt.Flagn4;
    pcv->Flag5 = flt.Flag5;
    pcv->Flag6 = flt.Flag6;
    pcv->count = m_count;
    pcv->Flag7 = posInLine;
    pcv->Flag8 = flt.Flag8;
    pcv->Flagn8 = flt.Flagn8;

    pcv->FlagnW = flt.FlagnW;
    pcv->FlagnQ = flt.FlagnQ;
    pcv->nWQ = flt.nWQ;
    pcv->ntime = flt.ntime;
    pcv->ntime2 = flt.ntime2;

    pcv->Over1 = flt.Over1;
    pcv->Over2 = flt.Over2;

    return;
}

void ApplyStringToCV(LPCWSTR ws, DWORD pDf, int id)
{


    wstring localString = ws;
    if (!pDf)return;
    if (!ws)return;
    DWORD Df = *(DWORD*)pDf;//Df 数据的指针
    if (!Df)return;//为零则终止函数
    TESTDATA* nCView = (TESTDATA*)Df;//使一个TESTDATA*指向Df
    DWORD pOld;
    VirtualProtect(nCView, 10 + (localString.length() * 2), PAGE_READWRITE, &pOld);
    TESTDATA loadFirst = *nCView;//保存第一个数据
    int posInLine = 0;//横排位置
    int posinCross = 0xD;//竖排位置

  //  SetWindowTextW(m_hWnd, L"进入循环");
    for (int i = 0; i < localString.length(); i++)
    {
        if (localString[i] == L'\n')
        {
            BuildCV(nCView, loadFirst, posinCross, posInLine, i * 2, 0);
            nCView++;
            BuildCV(nCView, loadFirst, posinCross, posInLine, i * 2, 0XFFFF);
            nCView++;
            posinCross += 0x28;
            posInLine = 0;
        }
        else
        {
            BuildCV(nCView, loadFirst, posinCross, posInLine, i * 2, localString[i]);
            nCView++;
            posInLine += 0X1A;
        }
    }
    //  SetWindowTextW(m_hWnd, L"跳出循环");
    *(DWORD*)(pDf + 4) = (DWORD)nCView;

    VirtualProtect(nCView, 10 + (localString.length() * 2), pOld, NULL);
}

 int TranSplete(DWORD lp)
{
    // MessageBox(0,L"Transplete start",L"",0);
    if (!lp)return -1;
    DWORD ppDf = *(DWORD*)(lp + 0x2B0);
    if (!ppDf)return -1;
    DWORD pDf = *(DWORD*)(ppDf + 8);
    if (!pDf)return -1;
    DWORD Df = *(DWORD*)pDf;
    if (!Df)return -1;
    DWORD pEnd = *(DWORD*)(pDf + 4);
    if (!pEnd)return -1;

    WCHAR sjp[1024];
    WCHAR scn[1024];

    DWORD Num = (pEnd - Df) / 96;

    wstring nStr;
    TESTDATA* nCView = (TESTDATA*)Df;

    for (DWORD i = 0; i < Num; i++) {
        if (nCView->nChar == 0xFFFF) {

        }
        else if (nCView->nChar == 0) {
            nStr += L'\n';
        }
        else {
            nStr += nCView->nChar;
        }

        nCView++;
    }
    npID++;
    // MessageBoxW(0,L"0",L"",0);
    WCHAR csjp[1024];
    WCHAR cscn[1024];
    int ppId = 0;
    if (!GetDataByID(nID - 1, csjp, cscn)) {
        if (nID != 0)
            if (!GetDataByJP(&ppId, nStr.c_str(), cscn))
                return -1;
    }
    // MessageBoxW(0, L"1", L"", 0);
    if (lstrcmpW(nStr.c_str(), cscn) == 0) {

        //     MessageBoxW(0, nStr.c_str(), L"", 0);
        return -1;
    }

    //  MessageBoxW(0, L"2", L"", 0);
    if (wcsstr(nStr.c_str(), L"………")) {

        return -1;
    }
    if (wcsstr(nStr.c_str(), L"…ああ"))
        return -1;

    if (wcsstr(nStr.c_str(), L"ふふふっ"))
        return -1;


    lstrcpyW(ms_str, nStr.c_str());
    if (!GetDataByID(nID, sjp, scn)) {
        int pID = 0;
        if (GetDataByJP(&pID, nStr.c_str(), scn)) {
            nID = pID + 1;
            ApplyStringToCV(scn, pDf, pID);
            return 0;
        }
    }
    else
    {
        if (lstrcmpW(sjp, nStr.c_str()) == 0) {
            ApplyStringToCV(scn, pDf, nID);
        }
        else
        {
            int pID = 0;
            if (GetDataByJP(&pID, nStr.c_str(), scn)) {
                nID = pID + 1;
                ApplyStringToCV(scn, pDf, pID);
                return 0;
            }
            else
                nID = pID;
        }
    }
    nID++;
    return 0;
}
