/*
用于实现改进后的NeXAS引擎
自制字库与GetGlyphOutline
相兼容的汉化方式

made by Yggdrasill（Darkness-TX & Destinyの火狐）
2017.01.09
*/
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <detours.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,  LPWSTR lpCmdLine,  int nShowCmd)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	STARTUPINFOW si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	si.cb = sizeof(STARTUPINFOW);

	wchar_t dirPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dirPath);

	wchar_t exePath[MAX_PATH];
	wsprintfW(exePath, L"%ls\\BALDR HEART_dump.exe", dirPath);
	DetourCreateProcessWithDllW(NULL, exePath, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, dirPath, &si, &pi, "Longinus.dll", NULL);
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(&si);
	CloseHandle(&pi);

	return 0;
}