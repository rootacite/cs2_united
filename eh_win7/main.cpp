#include <Windows.h>

#include "BuildIn.h"
#include "Data.h"

#pragma comment (lib,"cs2_patch.lib")

int  WINAPI wWinMain(HINSTANCE hinst, HINSTANCE prevHin, LPWSTR lpw, int nCmdShow)
{
	InjectSelfTo("ExHIBIT.exe");
}