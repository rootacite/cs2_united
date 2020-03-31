#include <Windows.h>

#include "BuildIn.h"
#include "Data.h"

#pragma comment (lib,"cs2_patch.lib")

int  WINAPI WinMain(HINSTANCE hinst, HINSTANCE prevHin, LPSTR lpw, int nCmdShow)
{
	InjectSelfTo("ExHIBIT.exe");
}