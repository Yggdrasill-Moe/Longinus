/*
��������

made by Yggdrasill��Darkness-TX & Destiny�λ����
2017.01.09
*/
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <detours.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

//�����룬������unicode��......
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
	wchar_t iniPath[MAX_PATH];
	wchar_t exePath[MAX_PATH];
	wchar_t dllPath[MAX_PATH];
	unit8 dllPathA[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, dirPath);
	wsprintfW(iniPath, L"%ls\\%ls", dirPath, L"Longinus.ini");
	if (_waccess(iniPath, 4) == -1)
	{
		MessageBoxW(NULL, L"Longinus.ini�ļ������ڣ�", L"����", MB_OK);
		exit(0);
	}
	if (GetPrivateProfileStringW(L"FileName", L"DLL", L"", dllPath, MAX_PATH, iniPath) == 0)
	{
		MessageBoxW(NULL, L"����ʧ�ܣ�����DLL�����Ƿ���ȷ��", L"����", MB_OK);
		exit(0);
	}
	GetPrivateProfileStringW(L"FileName", L"EXE", L"", exePath, MAX_PATH, iniPath);
	WideCharToMultiByte(CP_ACP, 0, dllPath, -1, dllPathA, MAX_PATH, NULL, NULL);
	if (wcschr(exePath, L'\\') != NULL)
	{
		MessageBoxW(NULL, L"EXE���Բ�֧�ִ�Ŀ¼��ʽ��\n��ȷ��EXE������������ͬһ��Ŀ¼�£�", L"����", MB_OK);
		exit(0);
	}
	if (!DetourCreateProcessWithDllW(NULL, exePath, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, dirPath, &si, &pi, dllPathA, NULL))
		MessageBoxW(NULL, L"����ʧ�ܣ�����EXE�����Ƿ���ȷ��", L"����", MB_OK);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(&si);
	CloseHandle(&pi);
	return 0;
}