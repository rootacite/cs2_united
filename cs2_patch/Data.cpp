#include "pch.h"
#include "Data.h"
extern MicroData* Index;
extern MicroBinary* Data;
extern "C" extern DLLAPI double saveProcess;
extern HMODULE hMod;

extern "C" extern DLLAPI wchar_t ms_str[3096];

extern "C" extern DLLAPI int nID;
extern "C" extern DLLAPI DWORD tPid;
extern char IpfData[16];
#define PutInt(a) _itoa_s(a,IpfData,10);MessageBoxA(0,IpfData,"num",0);
struct ExportParam {
    WCHAR data[1024];
    WCHAR src[1024];
};
DWORD CreateDataExportEx(LPVOID data)
{

    ExportParam* bData = (ExportParam*)data;
    WCHAR sjp[1024];
    WCHAR scn[1024];
    int lasger = GEtLargestID();
    if (nID == -1) {
        MessageBox(0, L"the ID value seems not available,therefore this action has been refused", L"error", MB_ICONERROR | MB_MODEMASK);
        saveProcess = 1.0;
        return 1;
    }
    if (nID - lasger > 1) {
        MessageBox(0, L"the ID value seems not available,therefore this action has been refused", L"error", MB_ICONERROR | MB_MODEMASK);
        saveProcess = 1.0;
        return 1;
    }
    if (!GetDataByID(nID, sjp, scn)) {
     //   PutInt(nID);
      //  PutInt(2 * (lstrlenW(bData->src) + 1));
      //  PutInt(2 * (lstrlenW(bData->data) + 1));

        CreateDataByID(nID, bData->src, 2 * (lstrlenW(bData->src) + 1), bData->data, 2 * (lstrlenW(bData->data) + 1));
        WCHAR abv[16];
        _itow_s(nID, abv, 10);
        wstring str;
        str += L"Apply ID:";
        str += abv;
        str += L"\n";
        str += bData->src;
        str += L"->";
        str += bData->data;
        //  MessageBoxW(NULL, str.c_str(), L"successed to add rule", MB_ICONINFORMATION | MB_MODEMASK);
        saveProcess = 1.0;
        return 1;
    }
    else {
        wstring nString = L"This rule is already exist,do you want still to replace it?(according to your PC,it may take you a short time)\n";
        WCHAR abv[16];
        _itow_s(nID, abv, 10);
        nString += L"ID:";
        nString += abv;
        nString += L"\n";
        nString += sjp;
        saveProcess = 0.0;
        int result = MessageBoxW(NULL, nString.c_str(), L"information", MB_ICONINFORMATION | MB_OKCANCEL | MB_MODEMASK);
        if (result != IDOK) {
            saveProcess = 1;
            return 1;
        }
        int p = 0;
        MicroData* _Index = new MicroData(L"~Index.ax", sizeof(IndexData));
        MicroBinary* _Data = new MicroBinary(L"~Data.ax");
        IndexData createData;
        while (GetDataByID(p, sjp, scn)) {
            if (p == nID) {
                createData.Id = p;
                createData.JpLength = 2 * (lstrlenW(sjp) + 1);
                createData.CnLength = 2 * (lstrlenW(bData->data) + 1);
                createData.JpBass = _Data->Size();
                _Data->Push(sjp, 2 * (lstrlenW(sjp) + 1));
                createData.CnBass = _Data->Size();
                _Data->Push(bData->data, 2 * (lstrlenW(bData->data) + 1));
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
              
            }
            saveProcess = (double)p / (double)(lasger);
            p++;
        }
        _Index->Save();
        _Data->Save();
        delete _Index;
        delete _Data;
        delete Index;
        delete ::Data;
        //   saveProcess = 0.0;
       
        DeleteFile(L"Data.ax");
        DeleteFile(L"Index.ax");
        rename("~Data.ax", "Data.ax");
        rename("~Index.ax", "Index.ax");
        Index = new MicroData(L"Index.ax", sizeof(IndexData));
        ::Data = new MicroBinary(L"Data.ax");
        Index->Load();
        ::Data->Load();

        saveProcess = 1;
        return 1;
    }
    saveProcess = 1;
    return 0;
}
void CreateDataExport(WCHAR src[],WCHAR data[])
{
    ExportParam alc;
    lstrcpyW(alc.data, data);
    lstrcpyW(alc.src, src);

    DWORD dwOld;
    HANDLE hTr = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tPid);
    if (!hTr) {
        MessageBoxA(0, "", "", 0);
        return;
    }
  

    BYTE* PszLibFileRemote = (PBYTE)VirtualAllocEx(hTr, NULL, sizeof(ExportParam), MEM_COMMIT, PAGE_READWRITE);
    if (!PszLibFileRemote) {
        MessageBoxA(0, "", "", 0);
        return;
    }

    WriteProcessMemory(hTr, PszLibFileRemote, &alc, sizeof(ExportParam), &dwOld);

    HANDLE hHookStart = CreateRemoteThread(hTr, NULL, 0, (LPTHREAD_START_ROUTINE)
        ::GetProcAddress(hMod, "CreateDataExportEx"), PszLibFileRemote, 0, NULL);
    if (!hHookStart) {
        MessageBoxA(0, "", "", 0);
        return;
    }
    WaitForSingleObject(hHookStart, INFINITE);
    VirtualFreeEx(hTr, PszLibFileRemote, 0, MEM_RELEASE);
    CloseHandle(hTr);
}
BOOL CreateDataByID(int ID, LPCWSTR jpBuff, int ljp, LPCWSTR cnBuffer, int lcn)
{
   // MessageBoxA(0,"","",0);
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
    if (Index->Size() == 0) {
        return 0;
    }
    IndexData index;
    WCHAR njp[3096];
    WCHAR ncn[3096];
    (*Index) = 0;
    do {
        Index->Get(&index);
        if (index.CnLength > 1024 || index.JpLength > 1024)
            MessageBox(0, L"error(12)", L"", 0);
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
            *ID = index.Id;
            (*Data) = 0;
            (*Index) = 0;
            return 0;
        }
    } while (1);
}
BOOL GetDataByID(int ID, LPWSTR jpBuff, LPWSTR cnBuffer)
{
   // 
    if (ID == -1) {
        return 0;
    }
    if (Index->Size() == 0) {
        return 0;
    }
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
    if (index.CnLength > 1024 || index.JpLength > 1024)
        MessageBox(0,L"error",L"",0);


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
    if (Index->Size() == 0)
        return -1;
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
