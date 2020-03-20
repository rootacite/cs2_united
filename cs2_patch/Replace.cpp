#include "pch.h"
#include "Replace.h"


#include<algorithm>
#include<iostream>
#include<set>

extern HMODULE hModL;
extern "C" extern DLLAPI wchar_t ms_str[3096];
extern "C" extern DLLAPI int nID;
extern "C" extern DLLAPI wchar_t cn_str[1024];
extern "C" extern DLLAPI bool IsSuccess;
extern TESTDATA* pNewDf;

extern char IpfData[16];
#define PutInt(a) _itoa_s(a,IpfData,16);MessageBoxA(0,IpfData,"num",0);

multiset<DWORD> AddrList;

extern "C" DLLAPI void BuildCV(TESTDATA* pcv, TESTDATA flt, DWORD posinCross, DWORD posInLine, DWORD m_count, WCHAR nchar)
{
    pcv->ForCast = flt.ForCast;
    pcv->nLine = posinCross;
    pcv->nChar = nchar;
    pcv->nCross = posInLine;
    pcv->Flag1 = flt.Flag1;
    pcv->Flag2 = 0xFFC8C8C8;
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



void ApplyStringToCV(LPCWSTR ws, DWORD pDf)
{
   // return;
   // SetWindowTextW(GetForegroundWindow(),ws);
    TESTDATA loadFirst = *((TESTDATA*)(*(DWORD*)pDf));
    if (pNewDf&& pNewDf== (TESTDATA*)*(DWORD*)pDf)
        free(pNewDf);
    pNewDf = (TESTDATA*)malloc(1024 * 96);



    for (int i = 0; i < 1024; i++)
        BuildCV(&pNewDf[i], loadFirst, 0XD, 0, 0, 0);
 
    wstring localString = ws;
    if (!pDf)return;
    if (!ws)return;

  
    *(DWORD*)pDf = (DWORD)pNewDf;
    int posInLine = 0;//����λ��
    int posinCross = 0xD;//����λ��
  //  SetWindowTextW(m_hWnd, L"����ѭ��");
    int PJ = 0;
    for (int i = 0; i < localString.length(); i++)
    {
        if (localString[i] == L'\n')
        {
            BuildCV(&pNewDf[PJ], loadFirst, posinCross, posInLine, i * 2, 0);
            PJ++;
            BuildCV(&pNewDf[PJ], loadFirst, posinCross, posInLine, i * 2, 0XFFFF);
            PJ++;
            posinCross += 0x28;
            posInLine = 0;
        }
        else
        {
            BuildCV(&pNewDf[PJ], loadFirst, posinCross, posInLine, i * 2, localString[i]);
            PJ++;
            posInLine += 0X1A;
        }
    }
    PJ++;
    //  SetWindowTextW(m_hWnd, L"����ѭ��");
    *(DWORD*)(pDf + 4) = (DWORD)&pNewDf[PJ];

    int sized = *(DWORD*)(pDf + 4) - *(DWORD*)pDf;
   // PutInt(sized);
    lstrcpyW(cn_str, ws);
}

static bool InitOnce = true;

 int TranSplete(DWORD lp)
{
   //  MessageBoxA(0,"","",0);
     //定义缓冲区
     WCHAR sjp[1024];
     WCHAR scn[1024];
     wstring nStr;


     /*第一部分 排除无效的调用*/
    if (!lp)return 1;
    DWORD ppDf = *(DWORD*)(lp + 0x2B0);
    if (!ppDf)return 1;
    DWORD pDf = *(DWORD*)(ppDf + 8);
    if (!pDf)return 1;
    DWORD Df = *(DWORD*)pDf;
    if (!Df)return 1;
    DWORD pEnd = *(DWORD*)(pDf + 4);
    if (!pEnd)return 1;
    /*************************/

    TESTDATA* nCView = (TESTDATA*)Df;//定义指向当前数据的指针
    DWORD Num = (pEnd - Df) / 96;//定义长度

    if (Num == 0) {
        return 1;
   }


    /*先扫描一次数据内存，把字符逐一加入nStr*/
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
    /***************************************/

    if (lstrcmpW(nStr.c_str(), cn_str) == 0) {//如果和上一句的中文相同，则直接返回。
        return -1;
    }
   // PutInt(*(DWORD*)pDf);
  //  PutInt(Num);
   // MessageBoxW(0, nStr.c_str(), L"", 0);
    /******排除较短的语气词******/
    if (wcsstr(nStr.c_str(), L"………") && lstrlenW(nStr.c_str()) < 8)
        return -1;
    if (wcsstr(nStr.c_str(), L"…ああ")&&lstrlenW(nStr.c_str())<8)
        return -1;
    if (wcsstr(nStr.c_str(), L"ふふふっ") && lstrlenW(nStr.c_str()) < 8)
        return -1;
    /****************************/

    //应用当前句到msstr
    lstrcpyW(ms_str, nStr.c_str());

   // MessageBoxA(0,"","",0);
    int ppdid = nID + 1;
   
    if (!GetDataByID(ppdid, sjp, scn)) {//尝试获取当前ID的数据
        //如果失败
        int pID = 0;
        if (GetDataByJP(&pID, nStr.c_str(), scn)) {//尝试从原文获取翻译
            //成功
            nID = pID;
           // MessageBoxW(0,scn,L"",0);
           ApplyStringToCV(scn, pDf);
         // MessageBoxA(0,"0","",0);
            return 0;//直接返回
        }
        else {
            //失败
            if (nID == -1) {
                nID = 0;
                return -1;
            }
            if (GetDataByID(nID, sjp, scn))
                nID = GEtLargestID() + 1;
          //  MessageBoxA(0, "1", "", 0);
            return -1;
        }
    }
    else
    {
        //如果成功
        if (lstrcmpW(sjp, nStr.c_str()) == 0) {//检查是否是正确的翻译
            //如果是则直接应用
           // MessageBoxW(0, scn, L"", 0);
           ApplyStringToCV(scn, pDf);
            nID = ppdid;
         //  MessageBoxA(0, "2", "", 0);
            return 0;
        }
        else
        {
            //如果不是则
            int pID = 0;
            if (GetDataByJP(&pID, nStr.c_str(), scn)) {//再次尝试用原文获取数据
                //如果成功
               // MessageBoxW(0, scn, L"", 0);
                ApplyStringToCV(scn, pDf);
                nID = pID;
             //   MessageBoxA(0, "3", "", 0);
                return 0;//直接返回
            }
            else {
                //如果不是
                nID = GEtLargestID()+1;
             //   MessageBoxA(0, "4", "", 0);
                return -1;
            }

        }
    }
    
    return -1;
}
