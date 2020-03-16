#include "pch.h"
#include "Data.h"
extern MicroData* Index;
extern MicroBinary* Data;
extern "C" extern DLLAPI double saveProcess;
extern HMODULE hMod;
extern "C" extern DLLAPI wchar_t ms_str[3096];
extern "C" extern DLLAPI int nID;
extern "C" extern DLLAPI DWORD tPid;
DWORD CreateDataExportEx(LPCWSTR data) 
{
   // MessageBoxW(0, data,L"",0);
    WCHAR sjp[3096];
    WCHAR scn[3096];
    int lasger = GEtLargestID();
    if (nID == 0) {
        MessageBox(0, L"the ID value seems not available,therefore this action has been refused", L"error", MB_ICONERROR);
        saveProcess = 1.0;
        return 1;
    }
    if (nID - lasger > 2) {
        MessageBox(0, L"the ID value seems not available,therefore this action has been refused", L"error", MB_ICONERROR);
        saveProcess = 1.0;
        return 1;
    }
    if (!GetDataByID(nID - 1, sjp, scn)) {
        CreateDataByID(nID - 1, ms_str, 2 * (lstrlenW(ms_str) + 1), data, 2 * (lstrlenW(data) + 1));
        WCHAR abv[16];
        _itow_s(nID - 1, abv, 10);
        wstring str;
        str += L"Apply ID:";
        str += abv;
        str += L"\n";
        str += ms_str;
        str += L"->";
        str += data;
        MessageBoxW(NULL, str.c_str(), L"successed to add rule", MB_ICONINFORMATION);
        saveProcess = 1.0;
        return 1;
    }
    else {
        wstring nString = L"This rule is already exist,do you want still to replace it?(according to your PC,it may take you a short time)\n";
        WCHAR abv[16];
        _itow_s(nID - 1, abv, 10);
        nString += L"ID:";
        nString += abv;
        nString += L"\n";
        nString += sjp;
        saveProcess = 0.0;
        int result = MessageBoxW(NULL, nString.c_str(), L"information", MB_ICONINFORMATION | MB_OKCANCEL);
        if (result != IDOK) {
            saveProcess = 1.0;
            return 1;
        }
        int p = 0;
        MicroData *_Index=new MicroData(L"~Index.ax", sizeof(IndexData));
        MicroBinary* _Data = new MicroBinary(L"~Data.ax");
        IndexData createData;
        while (GetDataByID(p, sjp, scn)) {
            if (p == (nID - 1)) {
                createData.Id = p;
                createData.JpLength = 2 * (lstrlenW(sjp) + 1);
                createData.CnLength = 2 * (lstrlenW(data) + 1);
                createData.JpBass = _Data->Size();
                _Data->Push(sjp, 2 * (lstrlenW(sjp) + 1));
                createData.CnBass = _Data->Size();
                _Data->Push(data, 2 * (lstrlenW(data) + 1));
                _Index->Push(&createData);
                _Index->Save();
                _Data->Save();
            }
            else
            {
                createData.Id = p;
                createData.JpLength = 2 * (lstrlenW(sjp) + 1);
                createData.CnLength = 2 * (lstrlenW(scn) + 1);
                createData.JpBass = _Data->Size();
                _Data->Push(sjp, 2 * (lstrlenW(sjp) + 1));
                createData.CnBass = _Data->Size();
                _Data->Push(scn, 2 * (lstrlenW(scn) + 1));
                _Index->Push(&createData);
                _Index->Save();
                _Data->Save();
            }
            saveProcess = (double)p / (double)(lasger);
            p++;
        }
     
        delete _Index;
        delete _Data;
        delete Index;
        delete Data;
     //   saveProcess = 0.0;
        saveProcess = 1;
        DeleteFile(L"Data.ax");
        DeleteFile(L"Index.ax");
        rename("~Data.ax", "Data.ax");
        rename("~Index.ax", "Index.ax");
        Index = new MicroData(L"Index.ax", sizeof(IndexData));
        Data = new MicroBinary(L"Data.ax");
        Index->Load();
        Data->Load();
        return 1;
    }
}
void CreateDataExport(WCHAR data[])
{
    DWORD dwOld;
    HANDLE hTr = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tPid);
    if (!hTr)
        MessageBoxA(0,"","",0);
    LPWSTR PszLibFileRemote = (LPWSTR)VirtualAllocEx(hTr, NULL, 2*(lstrlenW(data)+1), MEM_COMMIT, PAGE_READWRITE);
    if (!PszLibFileRemote)
        MessageBoxA(0, "", "", 0);
    WriteProcessMemory(hTr, PszLibFileRemote, data, 2 * (lstrlenW(data) + 1), &dwOld);
    HANDLE hHookStart = CreateRemoteThread(hTr, NULL, 0, (LPTHREAD_START_ROUTINE)
        ::GetProcAddress(hMod, "CreateDataExportEx"), PszLibFileRemote, 0, NULL);
    if (!hHookStart)
        MessageBoxA(0, "", "", 0);
    WaitForSingleObject(hHookStart, INFINITE);
}
BOOL CreateDataByID(int ID, LPCWSTR jpBuff, int ljp, LPCWSTR cnBuffer, int lcn)
{
    IndexData createData;
    createData.Id = ID;
    createData.JpLength = ljp;
    createData.CnLength = lcn;
    createData.JpBass = Data->Size();
    Data->Push(jpBuff, ljp);
    createData.CnBass = Data->Size();
    Data->Push(cnBuffer, lcn);
    Index->Push(&createData);
    Index->Save();
    Data->Save();
    return 0;
}
BOOL GetDataByJP(int* ID, LPCWSTR jpBuff, LPWSTR cnBuffer)
{
    IndexData index;
    WCHAR njp[3096];
    WCHAR ncn[3096];
    (*Index) = 0;
    do {
        Index->Get(&index);
       ( *Data) = index.JpBass;
        Data->Sub(njp, index.JpLength);
       (* Data) = index.CnBass;
        Data->Sub(ncn, index.CnLength);
        if (lstrcmpW(jpBuff, njp) == 0) {
            *ID = index.Id;
            lstrcpyW(cnBuffer, ncn);
            (*Index) = 0;
            (*Data) = 0;
            return 1;
        }
        if ((*Index)++)
            continue;
        else {
            (*Data) = 0;
            (*Index) = 0;
            return 0;
        }
    } while (1);
}
BOOL GetDataByID(int ID, LPWSTR jpBuff, LPWSTR cnBuffer)
{
   // 
    if (ID < 0)return 0;
    IndexData index;
    (*Index) = ID;
    Index->Get(&index);
    if (index.Id != ID)
    {
        (*Index) = 0;
        do {
            Index->Get(&index);
            if (index.Id == ID)
                break;
            if ((*Index)++)
                continue;
            else {
                (*Index) = 0;
                return 0;
            }
        } while (1);
    }
    (*Data) = index.JpBass;
    Data->Sub(jpBuff, index.JpLength);
    (*Data) = index.CnBass;
    Data->Sub(cnBuffer, index.CnLength);
    (*Data) = 0;
    (*Index) = 0;
    return 1;
}
int GEtLargestID()
{
    IndexData index;
    int result = 0;
    do {
        Index->Get(&index);
        if (index.Id > result)
            result = index.Id;
    } while ((*Index)++);
    (*Index) = 0;
    return result;
}
