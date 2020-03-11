#include "pch.h"
#include "Data.h"


extern MicroData* Index;
extern MicroBinary* Data;

extern HMODULE hMod;
extern "C" extern DLLAPI wchar_t ms_str[3096];
extern "C" extern DLLAPI int nID;

void CreateDataExport(WCHAR data[])
{
    WCHAR sjp[3096];
    WCHAR scn[3096];

    int lasger = GEtLargestID();

    if (nID - lasger > 2) {
        MessageBox(0, L"ID数值似乎不太对劲，\n程序因此取消了这次的行动。", L"错误", MB_ICONERROR);
        return;
    }
    if (!GetDataByID(nID - 1, sjp, scn)) {

        CreateDataByID(nID - 1, ms_str, 2 * (lstrlenW(ms_str) + 1), data, 2 * (lstrlenW(data) + 1));
        WCHAR abv[16];
        _itow_s(nID - 1, abv, 10);
        wstring str;
        str += L"应用：ID:";
        str += abv;
        str += L"\n";
        str += ms_str;
        str += L"->";
        str += data;
        MessageBoxW(NULL, str.c_str(), L"成功添加翻译", MB_ICONINFORMATION);
        return;
    }
    else {
        wstring nString = L"该翻译条目已经存在，是否决定要替换他？\n(这会重构数据文件，根据您的电脑性能，可能会占用一些时间)\n";
        WCHAR abv[16];
        _itow_s(nID - 1, abv, 10);
        nString += L"ID:";
        nString += abv;
        nString += L"\n";
        nString += sjp;
        int result = MessageBoxW(NULL, nString.c_str(), L"信息", MB_ICONINFORMATION | MB_OKCANCEL);


        if (result != IDOK)
            return;


        int p = 0;
        while (GetDataByID(p, sjp, scn)) {
            if (p == (nID - 1)) {
                CreateDataByIDEx(L"~Index.ax", L"~Data.ax", p, sjp, 2 * (lstrlenW(sjp) + 1), data, 2 * (lstrlenW(data) + 1));
            }
            else
            {
                CreateDataByIDEx(L"~Index.ax", L"~Data.ax", p, sjp, 2 * (lstrlenW(sjp) + 1), scn, 2 * (lstrlenW(scn) + 1));
            }
            p++;
        }
        DeleteFile(L"Data.ax");
        DeleteFile(L"Index.ax");

        rename("~Data.ax", "Data.ax");
        rename("~Index.ax", "Index.ax");
        return;
    }
}

BOOL CreateDataByIDEx(LPCWSTR name, LPCWSTR name2, int ID, LPCWSTR jpBuff, int ljp, LPCWSTR cnBuffer, int lcn)
{
    MicroData _Index(name, sizeof(IndexData));
    MicroBinary _Data(name2);

    _Index.Load();
    _Data.Load();

    IndexData createData;
    createData.Id = ID;
    createData.JpLength = ljp;
    createData.CnLength = lcn;

    createData.JpBass = Data->Size();
    _Data.Push(jpBuff, ljp);
    createData.CnBass = Data->Size();
    _Data.Push(cnBuffer, lcn);

    _Index.Push(&createData);
    _Index.Save();
    _Data.Save();
    return 0;
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
