#pragma once
#include <Windows.h>

#include <string>
using namespace std;

#define DLLAPI __declspec(dllexport)

#ifndef ALGORITHM_EXPORTS
#define ALGORITHM_CLASS __declspec(dllimport)
#define ALGORITHM_TEMPLATE
#else //EXPORT
#define ALGORITHM_CLASS __declspec(dllexport)
#define ALGORITHM_TEMPLATE __declspec(dllexport)
#endif


#define ENCODE_BYTE 1
#define ENCODE_WORD 2
#define ENCODE_DWORD 4

 class DLLAPI MicroFile
{
public:
	//打开文件句柄，保存文件名，不能操作4G以上文件
	MicroFile(LPCWSTR filename);
	//关闭文件句柄，清理内存
	~MicroFile();
	//将文件完整装入内存
	virtual BOOL Load();
	//清空文件
	void Clear();
    //保存修改
	virtual BOOL Save();
	//获取文件大小
	DWORD Size();
	///将一段数据推入文件末尾
	void Push(LPCVOID sour, ULONG size);
	///将一段数据从文件末尾弹出
	void Pop(LPVOID sour, ULONG size);
	//从当前读取位置截取数据
	void Sub(LPVOID tart,int size);

	//复制整个文件数据
	BOOL Gate(LPVOID tart);

	virtual BOOL Get(LPBYTE tart) = 0;
	virtual BOOL Set(BYTE sour) = 0;

	MicroFile& operator=(int sour);

	BYTE &operator*();
	BOOL operator++(int);
	BOOL operator--(int);
	BOOL operator-=(DWORD count);
	BOOL operator+=(DWORD count);



protected:
	HANDLE m_file = NULL;
	BYTE* fileData = NULL;
	DWORD size;
	wstring* name = new wstring;
	BYTE* nPoint = NULL;
};

class DLLAPI  MicroBinary :public MicroFile
{
public:
	MicroBinary(LPCWSTR filename);
	~MicroBinary();

	
	BOOL Get(LPWORD tart);
	BOOL Get(LPDWORD tart);
    BOOL Get(LPBYTE tart);
	BOOL Set(BYTE sour);
	BOOL Set(WORD sour);
	BOOL Set(DWORD sour);

	MicroBinary& operator=(int sour);
private:

};


class DLLAPI MicroText:public MicroFile
{
public:
	MicroText(LPCWSTR filename,UINT code);
	~MicroText();


	BOOL Get(LPWSTR tart);
	BOOL Get(LPSTR tart);

	BOOL Get(LPBYTE tart);
	BOOL Set(BYTE sour);

	BOOL Set(LPCWSTR tart);
	BOOL Set(LPCSTR tart);

	BOOL Save();
	BOOL Load();
	//MicroText& operator=(int sour);
	void Push(LPCWSTR sour);
	void Push(LPCSTR sour);


	void Pop(LPWSTR tart,int snbize);
	void Pop(LPSTR tart, int snbize);
	void Clear();

	DWORD Size();

	MicroText& operator=(int sour);

	char& operator*();
	WCHAR& operator&();

	BOOL operator++(int);
	BOOL operator--(int);

	//void Sub(LPBYTE tart, int size);
private:
	int m_code;
	wstring* wData = new wstring;
	string* Data = new string;
};


class DLLAPI MicroData :public MicroFile
{
public:
	 MicroData(LPCWSTR filename, DWORD nsize);
	 ~MicroData();


	 void operator=(int sour);

	 BOOL operator++(int);
	 BOOL operator--(int);
	 BOOL operator-=(DWORD count);
	 	BOOL operator+=(DWORD count);
	 void Push(LPCVOID sour);
	///将一段数据从文件末尾弹出
	 void Pop(LPVOID tart);
	 DWORD Size();
	 BOOL Get(LPBYTE tart);
	 BOOL Set(BYTE sour);

	 BOOL Get(LPVOID tart);
	 BOOL Set(LPCVOID sour);
private:
	int structure;
};
