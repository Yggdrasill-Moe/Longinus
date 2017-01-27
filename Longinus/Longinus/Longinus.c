/*
用于实现改进后的NeXAS引擎
自制字库与GetGlyphOutline
相兼容的汉化方式

made by Yggdrasill（Darkness-TX & Destinyの火狐）
2017.01.09
*/
#include "Longinus.h"

PVOID g_pOldCreateFontIndirectA = NULL;
typedef int (WINAPI *PfuncCreateFontIndirectA)(LOGFONTA *lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA *lplf)
{
	lplf->lfCharSet = CreateFontIndirectA_CharSet;
	if (ChangeFace_All)
	{
		wchar_t buff[20];
		GetPrivateProfileStringW(L"ChangeFace", L"Face", NULL, buff, 20, iniPath);
		WideCharToMultiByte(CP_ACP, 0, buff, -1, lplf->lfFaceName , MAX_PATH, NULL, NULL);
	}
	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

PVOID g_pOldCreateFontA = NULL;
typedef int (WINAPI *PfuncCreateFontA)(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName);
int WINAPI NewCreateFontA(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
{
	iCharSet = CreateFontA_CharSet;
	if (ChangeFace_All)
	{
		wchar_t buff[20];
		GetPrivateProfileStringW(L"ChangeFace", L"Face", NULL, buff, 20, iniPath);
		WideCharToMultiByte(CP_ACP, 0, buff, -1, (char *)pszFaceName, MAX_PATH, NULL, NULL);
	}
	return ((PfuncCreateFontA)g_pOldCreateFontA)(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic,
		bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
}

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int (WINAPI *PfuncMultiByteToWideChar)(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);
int WINAPI NewMultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
{
	if (CodePage == 932)
		CodePage = MultiByteToWideChar_CodePage;
	return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

PVOID g_pOldGetGlyphOutlineW = NULL;
typedef int (WINAPI *PfuncGetGlyphOutlineW)(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2 *lpmat2);
int WINAPI NewGetGlyphOutlineW(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2)
{
	if (uChar < 0xA000)
		uChar = tbl_data[uChar];
	return ((PfuncGetGlyphOutlineW)g_pOldGetGlyphOutlineW)(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}

PVOID g_pOldGetGlyphOutlineA = NULL;
typedef int (WINAPI *PfuncGetGlyphOutlineA)(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2 *lpmat2);
int WINAPI NewGetGlyphOutlineA(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2)
{
	if (uChar < 0xA000)
		uChar = tbl_data[uChar];
	return ((PfuncGetGlyphOutlineA)g_pOldGetGlyphOutlineA)(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}

PVOID g_pOldCreateFileA = NULL;
typedef int (WINAPI *PfuncCreateFileA)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
int WINAPI NewCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{

	LPCSTR filename = lpFileName;
	NodeCreateFileA_Replace *q = CreateFileA_Replace;
	while (q->next != NULL)
	{
		q = q->next;
		if (strcmp(lpFileName, q->OldFileName) == 0)
		{
			filename = q->NewFileName;
			break;
		}
	}
	return ((PfuncCreateFileA)g_pOldCreateFileA)(filename, dwDesiredAccess, dwShareMode,
		lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

PVOID g_pOldCreateFileW = NULL;
typedef int (WINAPI *PfuncCreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
int WINAPI NewCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	LPCWSTR filename = lpFileName;
	NodeCreateFileW_Replace *q = CreateFileW_Replace;
	while (q->next != NULL)
	{
		q = q->next;
		if (wcscmp(lpFileName, q->OldFileName) == 0)
		{
			filename = q->NewFileName;
			break;
		}
	}
	return ((PfuncCreateFileW)g_pOldCreateFileW)(filename, dwDesiredAccess, dwShareMode,
		lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

PVOID g_pOldCreateWindowExA = NULL;
typedef int (WINAPI *PfuncCreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,
	int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
int WINAPI NewCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,
	int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	LPCSTR titlename = "魔女恋爱日记|爱丽丝后宫协会（请勿网络直播本补丁内容）|Longinus 1.0.0.2";
	return ((PfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, titlename, dwStyle, X, Y,
		nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

PVOID g_pOldSetWindowTextA = NULL;
typedef int (WINAPI *PfuncSetWindowTextA)(HWND hWnd, LPCSTR lpString);
int WINAPI NewSetWindowTextA(HWND hWnd, LPCSTR lpString)
{
	LPCSTR textname = lpString;
	NodeSetWindowTextA_Replace *q = SetWindowTextA_Replace;
	while (q->next != NULL)
	{
		q = q->next;
		if (strcmp(lpString, q->OldString) == 0)
		{
			textname = q->NewString;
			break;
		}
	}
	return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hWnd, textname);
}

PVOID g_pOldMessageBoxA = NULL;
typedef int (WINAPI *PfuncMessageBoxA)(HWND hWnd, LPCSTR lpString);
int WINAPI NewMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	LPCSTR titlename = lpCaption;
	NodeMessageBoxA_CaptionReplace *cq = MessageBoxA_CaptionReplace;
	while (cq->next != NULL)
	{
		cq = cq->next;
		if (strcmp(lpCaption, cq->OldString) == 0)
		{
			titlename = cq->NewString;
			break;
		}
	}
	LPCSTR textname = lpText;
	NodeMessageBoxA_TextReplace *tq = MessageBoxA_TextReplace;
	while (tq->next != NULL)
	{
		tq = tq->next;
		if (strcmp(lpText, tq->OldString) == 0)
		{
			textname = tq->NewString;
			break;
		}
	}
	return ((PfuncMessageBoxA)g_pOldMessageBoxA)(hWnd, textname, titlename, uType);
}

PVOID g_OldGetProcAddress = NULL;
typedef FARPROC(WINAPI *PfuncGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
FARPROC WINAPI NewGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	if (!First_Face_Patch)
	{
		if (GetPrivateProfileIntW(L"GetProcAddress", L"ChangeFace_Module", 1, iniPath) == 0)
		{
			NodeChangeFace *q = Change_Face;
			while (q->next != NULL)
			{
				q = q->next;
				ChangeFace(GetModuleHandleW(exePath), q->Offset, q->Size, q->Face);
			}
			First_Face_Patch = TRUE;
		}
		else
		{
			wchar_t ModuleName[30];
			GetPrivateProfileStringW(L"GetProcAddress", L"ChangeFace_ModuleName", NULL, ModuleName, 30, iniPath);
			if (GetModuleHandleW(ModuleName) == hModule)
			{
				NodeChangeFace *q = Change_Face;
				while (q->next != NULL)
				{
					q = q->next;
					ChangeFace(hModule, q->Offset, q->Size, q->Face);
				}
				First_Face_Patch = TRUE;
			}
		}
	}
	if (!First_Border_Patch)
	{
		if (GetPrivateProfileIntW(L"GetProcAddress", L"BorderPatch_Module", 1, iniPath) == 0)
		{
			NodeBorder_Patch *q = Border_Patch;
			while (q->next != NULL)
			{
				q = q->next;
				BorderPatch(GetModuleHandleW(exePath), q->Offset, q->Border);
			}
			First_Border_Patch = TRUE;
		}
		else
		{
			wchar_t ModuleName[30];
			GetPrivateProfileStringW(L"GetProcAddress", L"BorderPatch_ModuleName", NULL, ModuleName, 30, iniPath);
			if (GetModuleHandleW(ModuleName) == hModule)
			{
				NodeBorder_Patch *q = Border_Patch;
				while (q->next != NULL)
				{
					q = q->next;
					BorderPatch(hModule, q->Offset, q->Border);
				}
				First_Border_Patch = TRUE;
			}
		}
	}
	return ((PfuncGetProcAddress)g_OldGetProcAddress)(hModule, lpProcName);
}

//改变字体
void ChangeFace(HMODULE hModule, unit32 Offset, unit32 Size, char *Face)
{
	if (hModule == NULL)
	{
		VirtualProtect((void*)Offset, Size, PAGE_EXECUTE_READWRITE, &oldProtect);
		memset((void*)Offset, 0, Size);
		strcpy((void*)Offset, Face);
	}
	else
	{
		VirtualProtect((PBYTE)hModule + Offset, Size, PAGE_EXECUTE_READWRITE, &oldProtect);
		memset((PBYTE)hModule + Offset, 0, Size);
		strcpy((PBYTE)hModule + Offset, Face);
	}
}

//边界检测
//BH:cmp al,0x9F,魔女:搜FC
void BorderPatch(HMODULE hModule, unit32 Offset, unit8 Border)
{
	//memcpy((void*)0x40511E, &Border, 1);
	//memcpy((void*)0x405A6B, &Border, 1);
	//memcpy((void*)0x4B5093, &Border, 1);
	//memcpy((void*)0x4EBD2A, &Border, 1);
	//memcpy((void*)0x50397E, &Border, 1);
	//memcpy((void*)0x504E4E, &Border, 1);
	//memcpy((void*)0x64DE14, &Border, 1);
	//memcpy((void*)0x64DF25, &Border, 1);
	//memcpy((void*)0x64E07A, &Border, 1);
	//memcpy((void*)0x64E16C, &Border, 1);
	//memcpy((void*)0x64E577, &Border, 1);
	//memcpy((void*)0x64E8EE, &Border, 1);
	if (hModule == NULL)
	{
		VirtualProtect((PBYTE)Offset, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
		memset((void*)Offset, Border, 1);
	}
	else
	{
		VirtualProtect((PBYTE)hModule + Offset, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
		memset((PBYTE)hModule + Offset, Border, 1);
	}
}

//选择中文字体，配合CreateFontIndirectA
void EnumFontFamiliesAPatch()
{
	VirtualProtect((void *)EnumFontFamiliesA_MemOffset, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void *)EnumFontFamiliesA_MemOffset, &EnumFontFamiliesA_CharSet, 1);
}

void GetSettings()
{
	wchar_t *buff = malloc(MAX_PATH * 2);
	GetCurrentDirectoryW(MAX_PATH, dirPath);
	wsprintfW(iniPath, L"%ls\\%ls", dirPath, L"Longinus.ini");
	GetPrivateProfileStringW(L"FileName", L"EXE", L"", exePath, MAX_PATH, iniPath);
	IsOpen.OpenCreateFontIndirectA = GetPrivateProfileIntW(L"Settings", L"CreateFontIndirectA", 0, iniPath);
	IsOpen.OpenCreateFontA = GetPrivateProfileIntW(L"Settings", L"CreateFontA", 0, iniPath);
	IsOpen.OpenMultiByteToWideChar = GetPrivateProfileIntW(L"Settings", L"MultiByteToWideChar", 0, iniPath);
	IsOpen.OpenGetGlyphOutlineW = GetPrivateProfileIntW(L"Settings", L"GetGlyphOutlineW", 0, iniPath);
	IsOpen.OpenGetGlyphOutlineA = GetPrivateProfileIntW(L"Settings", L"GetGlyphOutlineA", 0, iniPath);
	IsOpen.OpenCreateFileA = GetPrivateProfileIntW(L"Settings", L"CreateFileA", 0, iniPath);
	IsOpen.OpenCreateFileW = GetPrivateProfileIntW(L"Settings", L"CreateFileW", 0, iniPath);
	IsOpen.OpenSetWindowTextA = GetPrivateProfileIntW(L"Settings", L"SetWindowTextA", 0, iniPath);
	IsOpen.OpenMessageBoxA = GetPrivateProfileIntW(L"Settings", L"MessageBoxA", 0, iniPath);
	IsOpen.OpenGetProcAddress = GetPrivateProfileIntW(L"Settings", L"GetProcAddress", 0, iniPath);
	IsOpen.OpenEnumFontFamiliesA = GetPrivateProfileIntW(L"Settings", L"EnumFontFamiliesA", 0, iniPath);
	IsOpen.OpenBorderPatch = GetPrivateProfileIntW(L"Settings", L"BorderPatch", 0, iniPath);
	IsOpen.OpenChangeFace = GetPrivateProfileIntW(L"Settings", L"ChangeFace", 0, iniPath);
	if (IsOpen.OpenCreateFontIndirectA)
	{
		GetPrivateProfileStringW(L"CreateFontIndirectA", L"CharSet", L"0x86", buff, 50, iniPath);
		CreateFontIndirectA_CharSet = (BYTE)CheckString(buff);
	}
	if (IsOpen.OpenCreateFontA)
	{
		GetPrivateProfileStringW(L"CreateFontA", L"CharSet", L"0x86", buff, 50, iniPath);
		CreateFontA_CharSet = (BYTE)CheckString(buff);
	}
	if (IsOpen.OpenMultiByteToWideChar)
	{
		GetPrivateProfileStringW(L"MultiByteToWideChar", L"CodePage", L"936", buff, 50, iniPath);
		MultiByteToWideChar_CodePage = CheckString(buff);
	}
	if (IsOpen.OpenEnumFontFamiliesA)
	{
		GetPrivateProfileStringW(L"EnumFontFamiliesA", L"CharSet", L"0x86", buff, 50, iniPath);
		EnumFontFamiliesA_CharSet = (BYTE)CheckString(buff);
		GetPrivateProfileStringW(L"EnumFontFamiliesA", L"MemOffset", NULL, buff, 50, iniPath);
		EnumFontFamiliesA_MemOffset = CheckString(buff);
	}
	if (IsOpen.OpenGetGlyphOutlineA)
	{
		GetPrivateProfileStringW(L"GetGlyphOutlineA", L"TBL", L"", buff, MAX_PATH, iniPath);
		FILE *tbl = _wfopen(buff, L"rb");
		tbl_data = malloc(0xF000 * 2);
		fread(tbl_data, 1, 0xF000 * 2, tbl);
		fclose(tbl);
	}
	if (IsOpen.OpenGetGlyphOutlineW)
	{
		GetPrivateProfileStringW(L"GetGlyphOutlineW", L"TBL", L"", buff, MAX_PATH, iniPath);
		FILE *tbl = _wfopen(buff, L"rb");
		tbl_data = malloc(0xF000 * 2);
		fread(tbl_data, 1, 0xF000 * 2, tbl);
		fclose(tbl);
	}
	if (IsOpen.OpenSetWindowTextA)
	{
		DWORD SetWindowTextA_Count = GetPrivateProfileIntW(L"SetWindowTextA", L"Count", 0, iniPath);
		NodeSetWindowTextA_Replace *q;
		q = malloc(sizeof(NodeSetWindowTextA_Replace));
		SetWindowTextA_Replace = q;
		for (DWORD i = 1; i <= SetWindowTextA_Count; i++)
		{
			NodeSetWindowTextA_Replace *row;
			row = malloc(sizeof(NodeSetWindowTextA_Replace));
			row->next = NULL;
			wsprintfW(buff, L"OldString%d", i);
			GetPrivateProfileStringW(L"SetWindowTextA", buff, NULL, buff, MAX_PATH, iniPath);
			WideCharToMultiByte(CP_ACP, 0, buff, -1, row->OldString, MAX_PATH, NULL, NULL);
			wsprintfW(buff, L"NewString%d", i);
			GetPrivateProfileStringW(L"SetWindowTextA", buff, NULL, buff, MAX_PATH, iniPath);
			WideCharToMultiByte(CP_ACP, 0, buff, -1, row->NewString, MAX_PATH, NULL, NULL);
			q->next = row;
			q = row;
		}
	}
	if (IsOpen.OpenMessageBoxA)
	{
		DWORD MessageBoxA_TextCount = GetPrivateProfileIntW(L"MessageBoxA", L"TextCount", 0, iniPath);
		NodeMessageBoxA_TextReplace *tq;
		tq = malloc(sizeof(NodeMessageBoxA_TextReplace));
		MessageBoxA_TextReplace = tq;
		for (DWORD i = 1; i <= MessageBoxA_TextCount; i++)
		{
			NodeMessageBoxA_TextReplace *trow;
			trow = malloc(sizeof(NodeMessageBoxA_TextReplace));
			trow->next = NULL;
			wsprintfW(buff, L"TextOldString%d", i);
			GetPrivateProfileStringW(L"MessageBoxA", buff, NULL, buff, MAX_PATH, iniPath);
			WideCharToMultiByte(CP_ACP, 0, buff, -1, trow->OldString, MAX_PATH, NULL, NULL);
			wsprintfW(buff, L"TextNewString%d", i);
			GetPrivateProfileStringW(L"MessageBoxA", buff, NULL, buff, MAX_PATH, iniPath);
			WideCharToMultiByte(CP_ACP, 0, buff, -1, trow->NewString, MAX_PATH, NULL, NULL);
			tq->next = trow;
			tq = trow;
		}
		DWORD MessageBoxA_CaptionCount = GetPrivateProfileIntW(L"MessageBoxA", L"CaptionCount", 0, iniPath);
		NodeMessageBoxA_CaptionReplace *cq;
		cq = malloc(sizeof(NodeMessageBoxA_CaptionReplace));
		MessageBoxA_CaptionReplace = cq;
		for (DWORD i = 1; i <= MessageBoxA_TextCount; i++)
		{
			NodeMessageBoxA_CaptionReplace *crow;
			crow = malloc(sizeof(NodeMessageBoxA_CaptionReplace));
			crow->next = NULL;
			wsprintfW(buff, L"CaptionOldString%d", i);
			GetPrivateProfileStringW(L"MessageBoxA", buff, NULL, buff, MAX_PATH, iniPath);
			WideCharToMultiByte(CP_ACP, 0, buff, -1, crow->OldString, MAX_PATH, NULL, NULL);
			wsprintfW(buff, L"CaptionNewString%d", i);
			GetPrivateProfileStringW(L"MessageBoxA", buff, NULL, buff, MAX_PATH, iniPath);
			WideCharToMultiByte(CP_ACP, 0, buff, -1, crow->NewString, MAX_PATH, NULL, NULL);
			cq->next = crow;
			cq = crow;
		}
	}
	if (IsOpen.OpenCreateFileA)
	{
		DWORD CreateFileA_Count = GetPrivateProfileIntW(L"CreateFileA", L"Count", 0, iniPath);
		NodeCreateFileA_Replace *q;
		q = malloc(sizeof(NodeCreateFileA_Replace));
		CreateFileA_Replace = q;
		for (DWORD i = 1; i <= CreateFileA_Count; i++)
		{
			NodeCreateFileA_Replace *row;
			row = malloc(sizeof(NodeCreateFileA_Replace));
			row->next = NULL;
			wsprintfW(buff, L"OldFileName%d", i);
			GetPrivateProfileStringW(L"CreateFileA", buff, NULL, buff, MAX_PATH, iniPath);
			WideCharToMultiByte(CP_ACP, 0, buff, -1, row->OldFileName, MAX_PATH, NULL, NULL);
			wsprintfW(buff, L"NewFileName%d", i);
			GetPrivateProfileStringW(L"CreateFileA", buff, NULL, buff, MAX_PATH, iniPath);
			WideCharToMultiByte(CP_ACP, 0, buff, -1, row->NewFileName, MAX_PATH, NULL, NULL);
			q->next = row;
			q = row;
		}
	}
	if (IsOpen.OpenCreateFileW)
	{
		DWORD CreateFileW_Count = GetPrivateProfileIntW(L"CreateFileW", L"Count", 0, iniPath);
		NodeCreateFileW_Replace *q;
		q = malloc(sizeof(NodeCreateFileW_Replace));
		CreateFileW_Replace = q;
		for (DWORD i = 1; i <= CreateFileW_Count; i++)
		{
			NodeCreateFileW_Replace *row;
			row = malloc(sizeof(NodeCreateFileW_Replace));
			row->next = NULL;
			wsprintfW(buff, L"OldFileName%d", i);
			GetPrivateProfileStringW(L"CreateFileW", buff, NULL, row->OldFileName, MAX_PATH, iniPath);
			wsprintfW(buff, L"NewFileName%d", i);
			GetPrivateProfileStringW(L"CreateFileW", buff, NULL, row->NewFileName, MAX_PATH, iniPath);
			q->next = row;
			q = row;
		}
	}
	if (IsOpen.OpenBorderPatch)
	{
		DWORD BorderPatch_Count = GetPrivateProfileIntW(L"BorderPatch", L"Count", 0, iniPath);
		if (GetPrivateProfileIntW(L"BorderPatch", L"Border_Address_Static", 1, iniPath))
		{
			GetPrivateProfileStringW(L"BorderPatch", L"Border_Mode", NULL, buff, 10, iniPath);
			if (wcsncmp(buff, L"ALL", 3) == 0)
			{
				GetPrivateProfileStringW(L"BorderPatch", L"Border", NULL, buff, 10, iniPath);
				BYTE Border = (BYTE)CheckString(buff);
				for (DWORD i = 1; i <= BorderPatch_Count; i++)
				{
					wsprintfW(buff, L"Offset%d", i);
					GetPrivateProfileStringW(L"BorderPatch", buff, NULL, buff, 50, iniPath);
					BorderPatch(NULL, CheckString(buff), Border);
				}
			}
			else if (wcsncmp(buff, L"ANY", 3) == 0)
			{
				for (DWORD i = 1; i <= BorderPatch_Count; i++)
				{
					wsprintfW(buff, L"Border%d", i);
					GetPrivateProfileStringW(L"BorderPatch", buff, NULL, buff, 10, iniPath);
					BYTE Border = (BYTE)CheckString(buff);
					wsprintfW(buff, L"Offset%d", i);
					GetPrivateProfileStringW(L"BorderPatch", buff, NULL, buff, 50, iniPath);
					BorderPatch(NULL, CheckString(buff), Border);
				}
			}
		}
		else
		{
			GetPrivateProfileStringW(L"BorderPatch", L"Border_Mode", NULL, buff, 10, iniPath);
			if (wcsncmp(buff, L"ALL", 3) == 0)
			{
				NodeBorder_Patch *q;
				q = malloc(sizeof(NodeBorder_Patch));
				Border_Patch = q;
				GetPrivateProfileStringW(L"BorderPatch", L"Border", NULL, buff, 10, iniPath);
				BYTE Border = (BYTE)CheckString(buff);
				for (DWORD i = 1; i <= BorderPatch_Count; i++)
				{
					NodeBorder_Patch *row;
					row = malloc(sizeof(NodeBorder_Patch));
					row->next = NULL;
					wsprintfW(buff, L"Offset%d", i);
					GetPrivateProfileStringW(L"BorderPatch", buff, NULL, buff, 50, iniPath);
					row->Border = Border;
					row->Offset = CheckString(buff);
					q->next = row;
					q = row;
				}
			}
			else if (wcsncmp(buff, L"ANY", 3) == 0)
			{
				NodeBorder_Patch *q;
				q = malloc(sizeof(NodeBorder_Patch));
				Border_Patch = q;
				for (DWORD i = 1; i <= BorderPatch_Count; i++)
				{
					NodeBorder_Patch *row;
					row = malloc(sizeof(NodeBorder_Patch));
					row->next = NULL;
					wsprintfW(buff, L"Border%d", i);
					GetPrivateProfileStringW(L"BorderPatch", buff, NULL, buff, 10, iniPath);
					BYTE Border = (BYTE)CheckString(buff);
					wsprintfW(buff, L"Offset%d", i);
					GetPrivateProfileStringW(L"BorderPatch", buff, NULL, buff, 50, iniPath);
					row->Border = Border;
					row->Offset = CheckString(buff);
					q->next = row;
					q = row;
				}
			}
		}
	}
	if (IsOpen.OpenChangeFace)
	{
		GetPrivateProfileStringW(L"ChangeFace", L"ChangeFace_Mode", NULL, buff, 10, iniPath);
		if (wcsncmp(buff, L"ALL", 3) == 0)
			ChangeFace_All = TRUE;
		else if (wcsncmp(buff, L"ANY", 3) == 0)
		{
			DWORD ChangeFace_Count = GetPrivateProfileIntW(L"ChangeFace", L"Count", 0, iniPath);
			if (GetPrivateProfileIntW(L"ChangeFace", L"Face_Address_Static", 1, iniPath))
			{
				for (DWORD i = 1; i <= ChangeFace_Count; i++)
				{		
					wsprintfW(buff, L"Offset%d", i);
					GetPrivateProfileStringW(L"ChangeFace", buff, NULL, buff, 20, iniPath);
					DWORD Offset = CheckString(buff);
					wsprintfW(buff, L"Size%d", i);
					GetPrivateProfileStringW(L"ChangeFace", buff, NULL, buff, 20, iniPath);
					DWORD Size = CheckString(buff);
					wsprintfW(buff, L"Face%d", i);
					GetPrivateProfileStringW(L"ChangeFace", buff, NULL, buff, 20, iniPath);
					char FaceA[20];
					WideCharToMultiByte(CP_ACP, 0, buff, -1, FaceA, MAX_PATH, NULL, NULL);
					ChangeFace(NULL, Offset, Size, FaceA);
				}
			}
			else
			{
				NodeChangeFace *q;
				q = malloc(sizeof(NodeChangeFace));
				Change_Face = q;
				for (DWORD i = 1; i <= ChangeFace_Count; i++)
				{
					NodeChangeFace *row;
					row = malloc(sizeof(NodeChangeFace));
					row->next = NULL;
					wsprintfW(buff, L"Offset%d", i);
					GetPrivateProfileStringW(L"ChangeFace", buff, NULL, buff, 20, iniPath);
					DWORD Offset = CheckString(buff);
					wsprintfW(buff, L"Size%d", i);
					GetPrivateProfileStringW(L"ChangeFace", buff, NULL, buff, 20, iniPath);
					DWORD Size = CheckString(buff);
					wsprintfW(buff, L"Face%d", i);
					GetPrivateProfileStringW(L"ChangeFace", buff, NULL, buff, 20, iniPath);
					char FaceA[20];
					WideCharToMultiByte(CP_ACP, 0, buff, -1, FaceA, MAX_PATH, NULL, NULL);
					row->Offset = Offset;
					row->Size = Size;
					strcpy(row->Face, FaceA);
					q->next = row;
					q = row;
				}
			}
		}
	}
	free(buff);
}

//安装Hook 
BOOL APIENTRY SetHook()
{
	//ChangeFace();

	GetSettings();
	if (IsOpen.OpenEnumFontFamiliesA)
		EnumFontFamiliesAPatch();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	if (IsOpen.OpenCreateFontIndirectA)
	{
		g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
		DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	}
	if (IsOpen.OpenCreateFontA)
	{
		g_pOldCreateFontA = DetourFindFunction("GDI32.dll", "CreateFontA");
		DetourAttach(&g_pOldCreateFontA, NewCreateFontA);
	}
	if (IsOpen.OpenCreateFileA)
	{
		g_pOldCreateFileA = DetourFindFunction("kernel32.dll", "CreateFileA");
		DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	}
	if (IsOpen.OpenCreateFileW)
	{
		g_pOldCreateFileW = DetourFindFunction("kernel32.dll", "CreateFileW");
		DetourAttach(&g_pOldCreateFileW, NewCreateFileW);
	}
	if (IsOpen.OpenGetGlyphOutlineW)
	{
		g_pOldGetGlyphOutlineW = DetourFindFunction("GDI32.dll", "GetGlyphOutline");
		DetourAttach(&g_pOldGetGlyphOutlineW, NewGetGlyphOutlineW);
	}
	if (IsOpen.OpenGetGlyphOutlineA)
	{
		g_pOldGetGlyphOutlineA = DetourFindFunction("GDI32.dll", "GetGlyphOutlineA");
		DetourAttach(&g_pOldGetGlyphOutlineA, NewGetGlyphOutlineA);
	}
	if (IsOpen.OpenMultiByteToWideChar)
	{
		g_pOldMultiByteToWideChar = DetourFindFunction("kernel32.dll", "MultiByteToWideChar");
		DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	}
	if (IsOpen.OpenSetWindowTextA)
	{
		g_pOldSetWindowTextA = DetourFindFunction("USER32.dll", "SetWindowTextA");
		DetourAttach(&g_pOldSetWindowTextA, NewSetWindowTextA);
	}
	if (IsOpen.OpenMessageBoxA)
	{
		g_pOldMessageBoxA = DetourFindFunction("USER32.dll", "MessageBoxA");
		DetourAttach(&g_pOldMessageBoxA, NewMessageBoxA);
	}
	if (IsOpen.OpenGetProcAddress)
	{
		g_OldGetProcAddress = DetourFindFunction("kernel32.dll", "GetProcAddress");
		DetourAttach(&g_OldGetProcAddress, NewGetProcAddress);
	}
	g_pOldCreateWindowExA = DetourFindFunction("USER32.dll", "CreateWindowExA");
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	LONG ret = DetourTransactionCommit();
	return ret == NO_ERROR;
}

//卸载Hook
BOOL APIENTRY DropHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	if (IsOpen.OpenCreateFontIndirectA)
		DetourDetach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
	if (IsOpen.OpenCreateFontA)
		DetourDetach(&g_pOldCreateFontA, NewCreateFontA);
	if (IsOpen.OpenCreateFileA)
		DetourDetach(&g_pOldCreateFileA, NewCreateFileA);
	if (IsOpen.OpenCreateFileW)
		DetourDetach(&g_pOldCreateFileW, NewCreateFileW);
	if (IsOpen.OpenGetGlyphOutlineW)
	{
		free(tbl_data);
		DetourDetach(&g_pOldGetGlyphOutlineW, NewGetGlyphOutlineW);
	}
	if (IsOpen.OpenGetGlyphOutlineA)
	{
		free(tbl_data);
		DetourDetach(&g_pOldGetGlyphOutlineA, NewGetGlyphOutlineA);
	}
	if (IsOpen.OpenMultiByteToWideChar)
		DetourDetach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	if (IsOpen.OpenSetWindowTextA)
		DetourDetach(&g_pOldSetWindowTextA, NewSetWindowTextA);
	if (IsOpen.OpenMessageBoxA)
		DetourDetach(&g_pOldMessageBoxA, NewMessageBoxA);
	if (IsOpen.OpenGetProcAddress)
		DetourDetach(&g_OldGetProcAddress, NewGetProcAddress);
	DetourDetach(&g_pOldCreateWindowExA, NewCreateWindowExA);
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