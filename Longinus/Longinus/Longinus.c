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
BOOL First_Face_Patch = FALSE;
BOOL First_Border_Patch = FALSE;

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

PVOID g_pOldGetGlyphOutlineA = NULL;
typedef int (WINAPI *PfuncGetGlyphOutlineA)(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2 *lpmat2);
int WINAPI NewGetGlyphOutlineA(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2)
{
	if (uChar < 0xA000)
		uChar = data[uChar];
	return ((PfuncGetGlyphOutlineA)g_pOldGetGlyphOutlineA)(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
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

PVOID g_pOldCreateWindowExA = NULL;
typedef int (WINAPI *PfuncCreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,
	int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
int WINAPI NewCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,
	int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	LPCSTR titlename = "魔女恋爱日记|爱丽丝后宫协会（请勿网络直播本补丁内容）|Longinus 1.0.0.1";
	return ((PfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, titlename, dwStyle, X, Y,
		nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

PVOID g_pOldSetWindowTextA = NULL;
typedef int (WINAPI *PfuncSetWindowTextA)(HWND hWnd, LPCSTR lpString);
int WINAPI NewSetWindowTextA(HWND hWnd, LPCSTR lpString)
{
	LPCSTR textname;
	if (strcmp(lpString, "弶婲摦帪偺僂僀儞僪僂儌乕僪傪愝掕偟傑偡丅") == 0)
		textname = "第一次启动请设定窗口模式";
	else if (strcmp(lpString, "僼儖僗僋儕乕儞偱僎乕儉傪奐巒偡傞丅") == 0)
		textname = "全屏幕开始游戏";
	else
		textname = lpString;
	return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hWnd, textname);
}

PVOID g_pOldMessageBoxA = NULL;
typedef int (WINAPI *PfuncMessageBoxA)(HWND hWnd, LPCSTR lpString);
int WINAPI NewMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	LPCSTR titlename;
	LPCSTR textname;
	if (strcmp(lpCaption, "魔女こいにっき") == 0)
		titlename = "魔女恋爱日记";
	else
		titlename = lpCaption;
	if (strcmp(lpText, "杮摉偵廔椆偟傑偡偐丠") == 0)
		textname = "真的要结束游戏吗？";
	else
		textname = lpText;
	return ((PfuncMessageBoxA)g_pOldMessageBoxA)(hWnd, textname, titlename, uType);
}

//改变字体
void ChangeFace(HMODULE hModule)
{
	DWORD oldProtect;
	PBYTE pal_face_add = (PBYTE)hModule + 0xFBDDC;
	VirtualProtect((void*)pal_face_add, 0x38, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(pal_face_add, 0, 0x38);
	strcpy(pal_face_add, "黑体");
	strcpy(pal_face_add + 0x10, "宋体");
	strcpy(pal_face_add + 0x1C, "黑体");
	strcpy(pal_face_add + 0x2C, "宋体");
}

//边界检测
//cmp al,0x9F
void BorderPatch(HMODULE hModule)
{
	//unit8 Border = 0xFE;
	unit32 oldProtect;
	//memcpy((void*)0x40511E, &Border, 1);
	//memcpy((void*)0x405A6B, &Border, 1);
	//memcpy((void*)0x4B5093, &Border, 1);
	//memcpy((void*)0x4EBD2A, &Border, 1);
	//memcpy((void*)0x50397E, &Border, 1);
	//memcpy((void*)0x504E4E, &Border, 1);

	//memcpy((void*)0x64DD6D, &Border, 1);

	//memcpy((void*)0x64DE14, &Border, 1);
	//memcpy((void*)0x64DF25, &Border, 1);
	//memcpy((void*)0x64E07A, &Border, 1);
	//memcpy((void*)0x64E16C, &Border, 1);
	//memcpy((void*)0x64E577, &Border, 1);
	//memcpy((void*)0x64E8EE, &Border, 1);
	VirtualProtect((PBYTE)hModule+0x262BB, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset((PBYTE)hModule + 0x262BB, 0x80, 1);
	VirtualProtect((PBYTE)hModule + 0x262C8, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset((PBYTE)hModule + 0x262C8, 0xFE, 1);
	VirtualProtect((PBYTE)hModule + 0x262E4, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset((PBYTE)hModule + 0x262E4, 0xFF, 1);
	VirtualProtect((PBYTE)hModule + 0x262FF, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset((PBYTE)hModule + 0x262FF, 0xFF, 1);
}

PVOID g_OldGetProcAddress = NULL;
typedef FARPROC (WINAPI *PfuncGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
FARPROC WINAPI NewGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	wchar_t exePath[MAX_PATH];
	wchar_t iniPath[MAX_PATH];
	wchar_t dirPath[MAX_PATH];
	if (!First_Face_Patch && GetModuleHandleA("pal.dll") == hModule)
	{
		ChangeFace(hModule);
		First_Face_Patch = TRUE;
	}
	if (!First_Border_Patch)
	{
		GetCurrentDirectoryW(MAX_PATH, dirPath);
		wsprintfW(iniPath, L"%ls\\%ls", dirPath, L"Longinus.ini");
		GetPrivateProfileStringW(L"FileName", L"EXE", L"", exePath, MAX_PATH, iniPath);
		wsprintfW(iniPath, L"%ls\\%ls", dirPath, exePath);
		BorderPatch(GetModuleHandleW(exePath));
		First_Border_Patch = TRUE;
	}
	return ((PfuncGetProcAddress)g_OldGetProcAddress)(hModule, lpProcName);
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
	//BorderPatch();
	//ChangeFace();
	//EnumFontFamiliesAPatch();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
	DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	g_pOldCreateFontA = DetourFindFunction("GDI32.dll", "CreateFontA");
	DetourAttach(&g_pOldCreateFontA, NewCreateFontA);
	//g_pOldCreateFileA = DetourFindFunction("kernel32.dll", "CreateFileA");
	//DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	//g_pOldCreateFileW = DetourFindFunction("kernel32.dll", "CreateFileW");
	//DetourAttach(&g_pOldCreateFileW, NewCreateFileW);
	//g_pOldGetGlyphOutlineW = DetourFindFunction("GDI32.dll", "GetGlyphOutline");
	//DetourAttach(&g_pOldGetGlyphOutlineW, NewGetGlyphOutlineW);
	g_pOldGetGlyphOutlineA = DetourFindFunction("GDI32.dll", "GetGlyphOutlineA");
	DetourAttach(&g_pOldGetGlyphOutlineA, NewGetGlyphOutlineA);
	g_pOldMultiByteToWideChar = DetourFindFunction("kernel32.dll", "MultiByteToWideChar");
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	g_pOldCreateWindowExA = DetourFindFunction("USER32.dll", "CreateWindowExA");
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	g_pOldSetWindowTextA = DetourFindFunction("USER32.dll", "SetWindowTextA");
	DetourAttach(&g_pOldSetWindowTextA, NewSetWindowTextA);
	g_pOldMessageBoxA = DetourFindFunction("USER32.dll", "MessageBoxA");
	DetourAttach(&g_pOldMessageBoxA, NewMessageBoxA);
	g_OldGetProcAddress = DetourFindFunction("kernel32.dll", "GetProcAddress");
	DetourAttach(&g_OldGetProcAddress, NewGetProcAddress);
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
	//DetourDetach(&g_pOldCreateFileA, NewCreateFileA);
	//DetourDetach(&g_pOldCreateFileW, NewCreateFileW);
	//DetourDetach(&g_pOldGetGlyphOutlineW, NewGetGlyphOutlineW);
	DetourDetach(&g_pOldGetGlyphOutlineA, NewGetGlyphOutlineA);
	DetourDetach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourDetach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	DetourDetach(&g_pOldSetWindowTextA, NewSetWindowTextA);
	DetourDetach(&g_pOldMessageBoxA, NewMessageBoxA);
	DetourDetach(&g_OldGetProcAddress, NewGetProcAddress);
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