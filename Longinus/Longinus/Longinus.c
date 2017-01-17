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
#include <io.h>
#include <detours.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

unit16 *data;

PVOID g_pOldCreateFontIndirectA = NULL;
typedef int (WINAPI *PfuncCreateFontIndirectA)(LOGFONTA *lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA *lplf)
{
	lplf->lfCharSet = GB2312_CHARSET;
	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

PVOID g_pOldCreateFontA = NULL;
typedef int (WINAPI *PfuncCreateFontA)(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName);
int WINAPI NewCreateFontA(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
{
	iCharSet = 0x86;
	return ((PfuncCreateFontA)g_pOldCreateFontA)(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic,
		bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
}

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int (WINAPI *PfuncMultiByteToWideChar)(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);
int WINAPI NewMultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
{
	if (CodePage == 932)
		CodePage = 936;
	return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

PVOID g_pOldGetGlyphOutlineW = NULL;
typedef int (WINAPI *PfuncGetGlyphOutlineW)(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2 *lpmat2);
int WINAPI NewGetGlyphOutlineW(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2)
{
	if (uChar < 0xA000)
		uChar = data[uChar];
	return ((PfuncGetGlyphOutlineW)g_pOldGetGlyphOutlineW)(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}

PVOID g_pOldCreateFileA = NULL;
typedef int (WINAPI *PfuncCreateFileA)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
int WINAPI NewCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{

	if (strncmp(lpFileName, "Update2.pac", 11) == 0)
		strcpy(lpFileName, "UpdateCHS.pac");
	return ((PfuncCreateFileA)g_pOldCreateFileA)(lpFileName, dwDesiredAccess, dwShareMode, 
		lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

PVOID g_pOldCreateFileW = NULL;
typedef int (WINAPI *PfuncCreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
int WINAPI NewCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (wcsncmp(lpFileName, L"Update2.pac", 11) == 0)
		wcscpy(lpFileName, L"UpdateCHS.pac");
	return ((PfuncCreateFileW)g_pOldCreateFileW)(lpFileName, dwDesiredAccess, dwShareMode,
		lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

//边界检测
//cmp al,0x9F
void BorderPatch()
{
	unit8 Border = 0xFE;
	//memcpy((void*)0x40511E, &Border, 1);
	//memcpy((void*)0x405A6B, &Border, 1);
	//memcpy((void*)0x4B5093, &Border, 1);
	//memcpy((void*)0x4EBD2A, &Border, 1);
	//memcpy((void*)0x50397E, &Border, 1);
	//memcpy((void*)0x504E4E, &Border, 1);
	memcpy((void*)0x64DD6D, &Border, 1);
	//memcpy((void*)0x64DE14, &Border, 1);
	//memcpy((void*)0x64DF25, &Border, 1);
	//memcpy((void*)0x64E07A, &Border, 1);
	//memcpy((void*)0x64E16C, &Border, 1);
	//memcpy((void*)0x64E577, &Border, 1);
	//memcpy((void*)0x64E8EE, &Border, 1);
}

//选择中文字体，配合CreateFontIndirectA
void EnumFontFamiliesAPatch()
{
	unit8 CharSet = 0x86;
	memcpy((void*)0x66C937, &CharSet, 1);
}

//安装Hook 
BOOL APIENTRY SetHook()
{
	FILE *tbl = fopen("CHS.TBL", "rb");
	data = malloc(0xF000 * 2);
	fread(data, 1, 0xF000 * 2, tbl);
	fclose(tbl);
	BorderPatch();
	EnumFontFamiliesAPatch();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	g_pOldCreateFontA = DetourFindFunction("GDI32.dll", "CreateFontA");
	DetourAttach(&g_pOldCreateFontA, NewCreateFontA);
	g_pOldCreateFileA = DetourFindFunction("kernel32.dll", "CreateFileA");
	DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	//g_pOldCreateFileW = DetourFindFunction("kernel32.dll", "CreateFileW");
	//DetourAttach(&g_pOldCreateFileW, NewCreateFileW);
	g_pOldGetGlyphOutlineW = DetourFindFunction("GDI32.dll", "GetGlyphOutline");
	DetourAttach(&g_pOldGetGlyphOutlineW, NewGetGlyphOutlineW);
	g_pOldMultiByteToWideChar = DetourFindFunction("kernel32.dll", "MultiByteToWideChar");
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	LONG ret = DetourTransactionCommit();
	return ret == NO_ERROR;
}

//卸载Hook
BOOL APIENTRY DropHook()
{
	free(data);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	DetourDetach(&g_pOldCreateFontA, NewCreateFontA);
	DetourDetach(&g_pOldCreateFileA, NewCreateFileA);
	//DetourDetach(&g_pOldCreateFileW, NewCreateFileW);
	DetourDetach(&g_pOldGetGlyphOutlineW, NewGetGlyphOutlineW);
	DetourDetach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	LONG ret = DetourTransactionCommit();
	return ret == NO_ERROR;
}

static HMODULE s_hDll;
HMODULE WINAPI Detoured()
{
	return s_hDll;
}

__declspec(dllexport)void WINAPI Init()
{

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		s_hDll = hModule;
		DisableThreadLibraryCalls(hModule);
		SetHook();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		DropHook();
		break;
	}
	return TRUE;
}