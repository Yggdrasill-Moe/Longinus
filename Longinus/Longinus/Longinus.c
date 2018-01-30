/*
用于实现自制字库
与GetGlyphOutline
相兼容的汉化方式

made by Yggdrasill（Darkness-TX & Destinyの火狐）
2017.01.09
*/
#include "Longinus_dat.h"
#include "Longinus.h"

PVOID g_pOldCreateFontIndirectA = NULL;
typedef int(WINAPI *PfuncCreateFontIndirectA)(LOGFONTA *lplf);
int WINAPI NewCreateFontIndirectA(LOGFONTA *lplf)
{
	lplf->lfCharSet = CreateFontIndirect_CharSet;
	if (ChangeFace_All)
	{
		wchar_t buff[32];
		GetPrivateProfileStringW(L"ChangeFace", L"Face", NULL, buff, 32, iniPath);
		WideCharToMultiByte(CP_ACP, 0, buff, -1, lplf->lfFaceName , MAX_PATH, NULL, NULL);
	}
	return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

PVOID g_pOldCreateFontIndirectW = NULL;
typedef int(WINAPI *PfuncCreateFontIndirectW)(LOGFONTW *lplf);
int WINAPI NewCreateFontIndirectW(LOGFONTW *lplf)
{
	lplf->lfCharSet = CreateFontIndirect_CharSet;
	if (ChangeFace_All)
		GetPrivateProfileStringW(L"ChangeFace", L"Face", NULL, lplf->lfFaceName, 32, iniPath);
	return ((PfuncCreateFontIndirectW)g_pOldCreateFontIndirectW)(lplf);
}

PVOID g_pOldCreateFontA = NULL;
typedef int(WINAPI *PfuncCreateFontA)(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName);
int WINAPI NewCreateFontA(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
{
	iCharSet = CreateFont_CharSet;
	if (ChangeFace_All)
	{
		wchar_t buff[32];
		GetPrivateProfileStringW(L"ChangeFace", L"Face", NULL, buff, 32, iniPath);
		WideCharToMultiByte(CP_ACP, 0, buff, -1, (char *)pszFaceName, MAX_PATH, NULL, NULL);
	}
	return ((PfuncCreateFontA)g_pOldCreateFontA)(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic,
		bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
}

PVOID g_pOldCreateFontW = NULL;
typedef int(WINAPI *PfuncCreateFontW)(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName);
int WINAPI NewCreateFontW(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName)
{
	iCharSet = CreateFont_CharSet;
	if (ChangeFace_All)
		GetPrivateProfileStringW(L"ChangeFace", L"Face", NULL, (WCHAR *)pszFaceName, 32, iniPath);
	return ((PfuncCreateFontW)g_pOldCreateFontW)(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic,
		bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
}

PVOID g_pOldEnumFontFamiliesExA = NULL;
typedef int(WINAPI *PfuncEnumFontFamiliesExA)(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags);
int WINAPI NewEnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags)
{
	lpLogfont->lfCharSet = EnumFontFamiliesEx_CharSet;
	if (ChangeFace_All)
	{
		wchar_t buff[32];
		GetPrivateProfileStringW(L"ChangeFace", L"Face", NULL, buff, 32, iniPath);
		WideCharToMultiByte(CP_ACP, 0, buff, -1, lpLogfont->lfFaceName, MAX_PATH, NULL, NULL);
	}
	return ((PfuncEnumFontFamiliesExA)g_pOldEnumFontFamiliesExA)(hdc, lpLogfont, lpProc, lParam, dwFlags);
}

PVOID g_pOldEnumFontFamiliesExW = NULL;
typedef int(WINAPI *PfuncEnumFontFamiliesExW)(HDC hdc, LPLOGFONTW lpLogfont, FONTENUMPROCW lpProc, LPARAM lParam, DWORD dwFlags);
int WINAPI NewEnumFontFamiliesExW(HDC hdc, LPLOGFONTW lpLogfont, FONTENUMPROCW lpProc, LPARAM lParam, DWORD dwFlags)
{
	lpLogfont->lfCharSet = EnumFontFamiliesEx_CharSet;
	if (ChangeFace_All)
		GetPrivateProfileStringW(L"ChangeFace", L"Face", NULL, lpLogfont->lfFaceName, 32, iniPath);
	return ((PfuncEnumFontFamiliesExW)g_pOldEnumFontFamiliesExW)(hdc, lpLogfont, lpProc, lParam, dwFlags);
}

PVOID g_pOldMultiByteToWideChar = NULL;
typedef int(WINAPI *PfuncMultiByteToWideChar)(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);
int WINAPI NewMultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
{
	if (CodePage == 932)
		CodePage = MultiByteToWideChar_CodePage;
	return ((PfuncMultiByteToWideChar)g_pOldMultiByteToWideChar)(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

PVOID g_pOldWideCharToMultiByte = NULL;
typedef int(WINAPI *PfuncWideCharToMultiByte)(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar);
int WINAPI NewWideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar)
{
	if (CodePage == 932)
		CodePage = WideCharToMultiByte_CodePage;
	return ((PfuncWideCharToMultiByte)g_pOldWideCharToMultiByte)(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
}

PVOID g_pOldGetGlyphOutlineW = NULL;
typedef int(WINAPI *PfuncGetGlyphOutlineW)(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2 *lpmat2);
int WINAPI NewGetGlyphOutlineW(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2)
{
	if (GetPrivateProfileIntW(L"GetGlyphOutline", L"TBL_MAPPED", 0, iniPath))
		if (tbl_data[uChar] != 0)
			uChar = tbl_data[uChar];
	wchar_t buff[20];
	GetPrivateProfileStringW(L"GetGlyphOutline", L"YF_CODING", NULL, buff, 20, iniPath);
	if (uChar == CheckString(buff))
	{
		uChar = 0x266A;
		if (cjBuffer == 0)
		{
			LOGFONTW newlogfont;
			LOGFONTW oldlogfont;
			HFONT newhFont;
			HFONT oldhFont;
			newlogfont.lfHeight = 30;
			newlogfont.lfWidth = 0;
			newlogfont.lfWeight = 0;
			newlogfont.lfEscapement = 0;
			newlogfont.lfOrientation = 0;
			newlogfont.lfItalic = 0;
			newlogfont.lfUnderline = 0;
			newlogfont.lfStrikeOut = 0;
			newlogfont.lfCharSet = DEFAULT_CHARSET;
			newlogfont.lfOutPrecision = 0;
			newlogfont.lfClipPrecision = 0;
			newlogfont.lfQuality = 0;
			newlogfont.lfPitchAndFamily = 0;
			wsprintf(newlogfont.lfFaceName, L"%ls", L"Arial");
			newhFont = CreateFontIndirectW(&newlogfont);
			oldhFont = (HFONT)SelectObject(hdc, newhFont);
			GetObjectW(oldhFont, sizeof(LOGFONTW), &oldlogfont);
			wsprintf(oldlogfont.lfFaceName, L"%ls", L"Arial");
			oldhFont = CreateFontIndirectW(&oldlogfont);
			newhFont = (HFONT)SelectObject(hdc, oldhFont);
			DeleteObject(newhFont);
		}
	}
	return ((PfuncGetGlyphOutlineW)g_pOldGetGlyphOutlineW)(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}

PVOID g_pOldGetGlyphOutlineA = NULL;
typedef int(WINAPI *PfuncGetGlyphOutlineA)(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2 *lpmat2);
int WINAPI NewGetGlyphOutlineA(HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, MAT2* lpmat2)
{
	if (GetPrivateProfileIntW(L"GetGlyphOutline", L"TBL_MAPPED", 0, iniPath))
		if (uChar < 0xA000 || uChar>0xE000)
			uChar = tbl_data[uChar];
	wchar_t buff[20];
	GetPrivateProfileStringW(L"GetGlyphOutline", L"YF_CODING", NULL, buff, 20, iniPath);
	if (uChar == CheckString(buff))
	{
		uChar = 0x266A;
		if (cjBuffer == 0)
		{
			LOGFONTA newlogfont;
			LOGFONTA oldlogfont;
			HFONT newhFont;
			HFONT oldhFont;
			newlogfont.lfHeight = 30;
			newlogfont.lfWidth = 0;
			newlogfont.lfWeight = 0;
			newlogfont.lfEscapement = 0;
			newlogfont.lfOrientation = 0;
			newlogfont.lfItalic = 0;
			newlogfont.lfUnderline = 0;
			newlogfont.lfStrikeOut = 0;
			newlogfont.lfCharSet = DEFAULT_CHARSET;
			newlogfont.lfOutPrecision = 0;
			newlogfont.lfClipPrecision = 0;
			newlogfont.lfQuality = 0;
			newlogfont.lfPitchAndFamily = 0;
			sprintf(newlogfont.lfFaceName, "%s", "Arial");
			newhFont = CreateFontIndirectA(&newlogfont);
			oldhFont = (HFONT)SelectObject(hdc, newhFont);
			GetObjectA(oldhFont, sizeof(LOGFONTW), &oldlogfont);
			sprintf(oldlogfont.lfFaceName, "%s", "Arial");
			oldhFont = CreateFontIndirectA(&oldlogfont);
			newhFont = (HFONT)SelectObject(hdc, oldhFont);
			DeleteObject(newhFont);
		}
	}
	return ((PfuncGetGlyphOutlineA)g_pOldGetGlyphOutlineA)(hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2);
}

PVOID g_pOldCreateFileA = NULL;
typedef int(WINAPI *PfuncCreateFileA)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
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
			strcpy((char *)filename, q->NewFileName);
			break;
		}
	}
	return ((PfuncCreateFileA)g_pOldCreateFileA)(filename, dwDesiredAccess, dwShareMode,
		lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

PVOID g_pOldCreateFileW = NULL;
typedef int(WINAPI *PfuncCreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
int WINAPI NewCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	LPWSTR filename[MAX_PATH];
	wcscpy((wchar_t *)filename, lpFileName);
	NodeCreateFileW_Replace *q = CreateFileW_Replace;
	while (q->next != NULL)
	{
		q = q->next;
		if (wcscmp((wchar_t *)filename, q->OldFileName) == 0)
		{
			wcscpy((wchar_t *)filename, q->NewFileName);
			break;
		}
	}
	return ((PfuncCreateFileW)g_pOldCreateFileW)((LPCWSTR)filename, dwDesiredAccess, dwShareMode,
		lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

PVOID g_pOldlstrcpyW = NULL;
typedef int(WINAPI *PfunclstrcpyW)(LPWSTR lpString1, LPCWSTR lpString2);
int WINAPI NewlstrcpyW(LPWSTR lpString1,  LPCWSTR lpString2)
{
	LPWSTR lpString[MAX_PATH];
	wcscpy((wchar_t *)lpString, lpString2);
	if (IsOpen.OpenLonginusFile)
	{
		unit32 hash = BKDRhash((wchar_t *)lpString);
		unit8 *p;
		p = (unit8 *)&hash + 3;
		NodeIndex_Data *iq = Index_Data;
		while (iq->next != NULL)
		{
			iq = iq->next;
			if (*p == iq->index)
			{
				NodeHash_Data *hq = iq->hash_data;
				while (hq != NULL)
				{
					if (hq->hash == hash)
						return ((PfunclstrcpyW)g_pOldlstrcpyW)(lpString1, hq->str_data);
					hq = hq->next;
				}
			}
		}
	}
	NodelstrcpyW_Replace *q = lstrcpyW_Replace;
	while (q->next != NULL)
	{
		q = q->next;
		if (wcscmp((wchar_t *)lpString, q->OldlpString2) == 0)
		{
			wcscpy((wchar_t *)lpString, q->NewlpString2);
			break;
		}
	}
	return ((PfunclstrcpyW)g_pOldlstrcpyW)(lpString1, (LPCWSTR)lpString);
}

PVOID g_pOldTextOutW = NULL;
typedef int(WINAPI *PfuncTextOutW)(HDC hdc, int x, int y, LPCWSTR lpString, int c);
int WINAPI NewTextOutW(HDC hdc, int x, int y, LPCWSTR lpString, int c)
{
	LPWSTR String[MAX_PATH];
	wcscpy((wchar_t *)String, lpString);
	/*
	if (IsOpen.OpenLonginusFile)
	{
		unit32 hash = BKDRhash((wchar_t *)String);
		unit8 *p;
		p = (unit8 *)&hash + 3;
		NodeIndex_Data *iq = Index_Data;
		while (iq->next != NULL)
		{
			iq = iq->next;
			if (*p == iq->index)
			{
				NodeHash_Data *hq = iq->hash_data;
				while (hq != NULL)
				{
					if (hq->hash == hash)
						return ((PfuncTextOutW)g_pOldTextOutW)(hdc, x, y, hq->str_data, wcslen(hq->str_data));
					hq = hq->next;
				}
			}
		}
	}*/
	NodeTextOutW_Replace *q = TextOutW_Replace;
	while (q->next != NULL)
	{
		q = q->next;
		if (wcsncmp((wchar_t *)String, q->OldlpString, c) == 0)
		{
			wcscpy((wchar_t *)String, q->NewlpString);
			break;
		}
	}
	return ((PfuncTextOutW)g_pOldTextOutW)(hdc, x, y, String, wcslen(String));
}

PVOID g_pOldCreateWindowExA = NULL;
typedef int(WINAPI *PfuncCreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,
	int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
int WINAPI NewCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y,
	int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	LPCSTR titlename = "Longinus EXE 1.0.0.2|Longinus DLL 1.3.0.0";
	return ((PfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, titlename, dwStyle, X, Y,
		nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

PVOID g_pOldCreateWindowExW = NULL;
typedef int(WINAPI *PfuncCreateWindowExW)(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y,
 int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
int WINAPI NewCreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y,
	int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	LPCWSTR titlename = L"Longinus EXE 1.0.0.2|Longinus DLL 1.3.0.0";
	return ((PfuncCreateWindowExW)g_pOldCreateWindowExW)(dwExStyle, lpClassName, titlename, dwStyle, X, Y,
		nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

PVOID g_pOldSetWindowTextA = NULL;
typedef int(WINAPI *PfuncSetWindowTextA)(HWND hWnd, LPCSTR lpString);
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
typedef int(WINAPI *PfuncMessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
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

PVOID g_pOldGetDriveTypeW = NULL;
typedef UINT(WINAPI *PfuncGetDriveTypeW)(LPCWSTR lpRootPathName);
UINT WINAPI NewGetDriveTypeW(LPCWSTR lpRootPathName)
{
	return DRIVE_CDROM;
}

PVOID g_pOldGetVolumeInformationW = NULL;
typedef BOOL(WINAPI *PfuncGetVolumeInformationW)(LPCWSTR lpRootPathName, LPWSTR lpVolumeNameBuffer, DWORD nVolumeNameSize,
 LPDWORD lpVolumeSerialNumber, LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags, LPWSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize);
BOOL WINAPI NewGetVolumeInformationW(LPCWSTR lpRootPathName, LPWSTR lpVolumeNameBuffer, DWORD nVolumeNameSize,
	LPDWORD lpVolumeSerialNumber, LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags, LPWSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize)
{
	GetPrivateProfileStringW(L"GetVolumeInformationW", L"VolumeName", L"", lpVolumeNameBuffer, nVolumeNameSize, iniPath);
	return TRUE;
}

PVOID g_pOldGetProcAddress = NULL;
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
	return ((PfuncGetProcAddress)g_pOldGetProcAddress)(hModule, lpProcName);
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
	IsOpen.OpenCreateFontIndirect = GetPrivateProfileIntW(L"Settings", L"CreateFontIndirect", 0, iniPath);
	IsOpen.OpenCreateFont = GetPrivateProfileIntW(L"Settings", L"CreateFont", 0, iniPath);
	IsOpen.OpenMultiByteToWideChar = GetPrivateProfileIntW(L"Settings", L"MultiByteToWideChar", 0, iniPath);
	IsOpen.OpenWideCharToMultiByte = GetPrivateProfileIntW(L"Settings", L"WideCharToMultiByte", 0, iniPath);
	IsOpen.OpenGetGlyphOutline = GetPrivateProfileIntW(L"Settings", L"GetGlyphOutline", 0, iniPath);
	IsOpen.OpenCreateWindowEx = GetPrivateProfileIntW(L"Settings", L"CreateWindowEx", 1, iniPath);
	IsOpen.OpenCreateFileA = GetPrivateProfileIntW(L"Settings", L"CreateFileA", 0, iniPath);
	IsOpen.OpenCreateFileW = GetPrivateProfileIntW(L"Settings", L"CreateFileW", 0, iniPath);
	IsOpen.OpenSetWindowTextA = GetPrivateProfileIntW(L"Settings", L"SetWindowTextA", 0, iniPath);
	IsOpen.OpenMessageBoxA = GetPrivateProfileIntW(L"Settings", L"MessageBoxA", 0, iniPath);
	IsOpen.OpenGetProcAddress = GetPrivateProfileIntW(L"Settings", L"GetProcAddress", 0, iniPath);
	IsOpen.OpenEnumFontFamiliesA = GetPrivateProfileIntW(L"Settings", L"EnumFontFamiliesA", 0, iniPath);
	IsOpen.OpenEnumFontFamiliesEx = GetPrivateProfileIntW(L"Settings", L"EnumFontFamiliesEx", 0, iniPath);
	IsOpen.OpenBorderPatch = GetPrivateProfileIntW(L"Settings", L"BorderPatch", 0, iniPath);
	IsOpen.OpenChangeFace = GetPrivateProfileIntW(L"Settings", L"ChangeFace", 0, iniPath);
	IsOpen.OpenlstrcpyW = GetPrivateProfileIntW(L"Settings", L"lstrcpyW", 0, iniPath);
	IsOpen.OpenTextOutW = GetPrivateProfileIntW(L"Settings", L"TextOutW", 0, iniPath);
	IsOpen.OpenGetDriveTypeW = GetPrivateProfileIntW(L"Settings", L"GetDriveTypeW", 0, iniPath);
	IsOpen.OpenGetVolumeInformationW = GetPrivateProfileIntW(L"Settings", L"GetVolumeInformationW", 0, iniPath);
	IsOpen.OpenLonginusFile = GetPrivateProfileIntW(L"Settings", L"Longinus_File", 0, iniPath);
	if (IsOpen.OpenLonginusFile)
	{
		if (!Read_Dat(L"Longinus.dat"))
			exit(0);
	}
	if (IsOpen.OpenCreateFontIndirect)
	{
		GetPrivateProfileStringW(L"CreateFontIndirect", L"CharSet", L"0x86", buff, 50, iniPath);
		CreateFontIndirect_CharSet = (BYTE)CheckString(buff);
	}
	if (IsOpen.OpenCreateFont)
	{
		GetPrivateProfileStringW(L"CreateFont", L"CharSet", L"0x86", buff, 50, iniPath);
		CreateFont_CharSet = (BYTE)CheckString(buff);
	}
	if (IsOpen.OpenMultiByteToWideChar)
	{
		GetPrivateProfileStringW(L"MultiByteToWideChar", L"CodePage", L"936", buff, 50, iniPath);
		MultiByteToWideChar_CodePage = CheckString(buff);
	}
	if (IsOpen.OpenWideCharToMultiByte)
	{
		GetPrivateProfileStringW(L"WideCharToMultiByte", L"CodePage", L"936", buff, 50, iniPath);
		WideCharToMultiByte_CodePage = CheckString(buff);
	}
	if (IsOpen.OpenEnumFontFamiliesEx)
	{
		GetPrivateProfileStringW(L"EnumFontFamiliesEx", L"CharSet", L"0x86", buff, 50, iniPath);
		EnumFontFamiliesEx_CharSet = (BYTE)CheckString(buff);
	}
	if (IsOpen.OpenEnumFontFamiliesA)
	{
		GetPrivateProfileStringW(L"EnumFontFamiliesA", L"CharSet", L"0x86", buff, 50, iniPath);
		EnumFontFamiliesA_CharSet = (BYTE)CheckString(buff);
		GetPrivateProfileStringW(L"EnumFontFamiliesA", L"MemOffset", NULL, buff, 50, iniPath);
		EnumFontFamiliesA_MemOffset = CheckString(buff);
	}
	if (IsOpen.OpenGetGlyphOutline)
	{
		if (GetPrivateProfileIntW(L"GetGlyphOutline", L"TBL_MAPPED", 0, iniPath))
		{
			GetPrivateProfileStringW(L"GetGlyphOutline", L"TBL", L"", buff, MAX_PATH, iniPath);
			if (buff == L"")
			{
				MessageBoxW(NULL, L"未指定TBL文件名，请于GetGlyphOutline下添加TBL配置！", L"错误", MB_OK);
				exit(0);
			}
			FILE *tbl = _wfopen(buff, L"rb");
			tbl_data = malloc(0xFF00 * 2);
			fread(tbl_data, 1, 0xFF00 * 2, tbl);
			fclose(tbl);
		}
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
		for (DWORD i = 1; i <= MessageBoxA_CaptionCount; i++)
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
			if (GetPrivateProfileIntW(L"CreateFileA", L"Use_Directory", 0, iniPath))
			{
				wchar_t *buff2 = malloc(MAX_PATH * 2);
				wsprintfW(buff2, L"%ls\\%ls", dirPath, buff);
				wsprintfW(buff, buff2);
				free(buff2);
			}
			WideCharToMultiByte(CP_ACP, 0, buff, -1, row->OldFileName, MAX_PATH, NULL, NULL);
			wsprintfW(buff, L"NewFileName%d", i);
			GetPrivateProfileStringW(L"CreateFileA", buff, NULL, buff, MAX_PATH, iniPath);
			if (GetPrivateProfileIntW(L"CreateFileA", L"Use_Directory", 0, iniPath))
			{
				wchar_t *buff2 = malloc(MAX_PATH * 2);
				wsprintfW(buff2, L"%ls\\%ls", dirPath, buff);
				wsprintfW(buff, buff2);
				free(buff2);
			}
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
			if (GetPrivateProfileIntW(L"CreateFileW", L"Use_Directory", 0, iniPath))
			{
				GetPrivateProfileStringW(L"CreateFileW", buff, NULL, buff, MAX_PATH, iniPath);
				wsprintfW(row->OldFileName, L"%ls\\%ls", dirPath, buff);
			}
			else
				GetPrivateProfileStringW(L"CreateFileW", buff, NULL, row->OldFileName, MAX_PATH, iniPath);
			wsprintfW(buff, L"NewFileName%d", i);
			if (GetPrivateProfileIntW(L"CreateFileW", L"Use_Directory", 0, iniPath))
			{
				GetPrivateProfileStringW(L"CreateFileW", buff, NULL, buff, MAX_PATH, iniPath);
				wsprintfW(row->NewFileName, L"%ls\\%ls", dirPath, buff);
			}
			else
				GetPrivateProfileStringW(L"CreateFileW", buff, NULL, row->NewFileName, MAX_PATH, iniPath);
			q->next = row;
			q = row;
		}
	}
	if (IsOpen.OpenlstrcpyW)
	{
		DWORD lstrcpyW_Count = GetPrivateProfileIntW(L"lstrcpyW", L"Count", 0, iniPath);
		NodelstrcpyW_Replace *q;
		q = malloc(sizeof(NodelstrcpyW_Replace));
		lstrcpyW_Replace = q;
		for (DWORD i = 1; i <= lstrcpyW_Count; i++)
		{
			NodelstrcpyW_Replace *row;
			row = malloc(sizeof(NodelstrcpyW_Replace));
			row->next = NULL;
			wsprintfW(buff, L"OldlpString%d", i);
			GetPrivateProfileStringW(L"lstrcpyW", buff, NULL, row->OldlpString2, MAX_PATH, iniPath);
			wsprintfW(buff, L"NewlpString%d", i);
			GetPrivateProfileStringW(L"lstrcpyW", buff, NULL, row->NewlpString2, MAX_PATH, iniPath);
			q->next = row;
			q = row;
		}
	}
	if (IsOpen.OpenTextOutW)
	{
		DWORD TextOutW_Count = GetPrivateProfileIntW(L"TextOutW", L"Count", 0, iniPath);
		NodeTextOutW_Replace *q;
		q = malloc(sizeof(NodeTextOutW_Replace));
		TextOutW_Replace = q;
		for (DWORD i = 1; i <= TextOutW_Count; i++)
		{
			NodeTextOutW_Replace *row;
			row = malloc(sizeof(NodeTextOutW_Replace));
			row->next = NULL;
			wsprintfW(buff, L"OldlpString%d", i);
			GetPrivateProfileStringW(L"TextOutW", buff, NULL, row->OldlpString, MAX_PATH, iniPath);
			wsprintfW(buff, L"NewlpString%d", i);
			GetPrivateProfileStringW(L"TextOutW", buff, NULL, row->NewlpString, MAX_PATH, iniPath);
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
	GetSettings();
	if (IsOpen.OpenEnumFontFamiliesA)
		EnumFontFamiliesAPatch();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	if (IsOpen.OpenCreateFontIndirect)
	{
		if (GetPrivateProfileIntW(L"CreateFontIndirect", L"Mode", 0, iniPath) == 0)
		{
			g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
			DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
		}
		if (GetPrivateProfileIntW(L"CreateFontIndirect", L"Mode", 0, iniPath) == 1)
		{
			g_pOldCreateFontIndirectW = DetourFindFunction("GDI32.dll", "CreateFontIndirectW");
			DetourAttach(&g_pOldCreateFontIndirectW, NewCreateFontIndirectW);
		}
		if (GetPrivateProfileIntW(L"CreateFontIndirect", L"Mode", 0, iniPath) == 2)
		{
			g_pOldCreateFontIndirectA = DetourFindFunction("GDI32.dll", "CreateFontIndirectA");
			DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
			g_pOldCreateFontIndirectW = DetourFindFunction("GDI32.dll", "CreateFontIndirectW");
			DetourAttach(&g_pOldCreateFontIndirectW, NewCreateFontIndirectW);
		}
	}
	if (IsOpen.OpenCreateFont)
	{
		if (GetPrivateProfileIntW(L"CreateFont", L"Mode", 0, iniPath) == 0)
		{
			g_pOldCreateFontA = DetourFindFunction("GDI32.dll", "CreateFontA");
			DetourAttach(&g_pOldCreateFontA, NewCreateFontA);
		}
		if (GetPrivateProfileIntW(L"CreateFont", L"Mode", 0, iniPath) == 1)
		{
			g_pOldCreateFontW = DetourFindFunction("GDI32.dll", "CreateFontW");
			DetourAttach(&g_pOldCreateFontW, NewCreateFontW);
		}
		if (GetPrivateProfileIntW(L"CreateFont", L"Mode", 0, iniPath) == 2)
		{
			g_pOldCreateFontA = DetourFindFunction("GDI32.dll", "CreateFontA");
			DetourAttach(&g_pOldCreateFontA, NewCreateFontA);
			g_pOldCreateFontW = DetourFindFunction("GDI32.dll", "CreateFontW");
			DetourAttach(&g_pOldCreateFontW, NewCreateFontW);
		}
	}
	if (IsOpen.OpenEnumFontFamiliesEx)
	{
		if (GetPrivateProfileIntW(L"EnumFontFamiliesEx", L"Mode", 0, iniPath) == 0)
		{
			g_pOldEnumFontFamiliesExA = DetourFindFunction("GDI32.dll", "EnumFontFamiliesExA");
			DetourAttach(&g_pOldEnumFontFamiliesExA, NewEnumFontFamiliesExA);
		}
		if (GetPrivateProfileIntW(L"EnumFontFamiliesEx", L"Mode", 0, iniPath) == 1)
		{
			g_pOldEnumFontFamiliesExW = DetourFindFunction("GDI32.dll", "EnumFontFamiliesExW");
			DetourAttach(&g_pOldEnumFontFamiliesExW, NewEnumFontFamiliesExW);
		}
		if (GetPrivateProfileIntW(L"EnumFontFamiliesEx", L"Mode", 0, iniPath) == 2)
		{
			g_pOldEnumFontFamiliesExA = DetourFindFunction("GDI32.dll", "EnumFontFamiliesExA");
			DetourAttach(&g_pOldEnumFontFamiliesExA, NewEnumFontFamiliesExA);
			g_pOldEnumFontFamiliesExW = DetourFindFunction("GDI32.dll", "EnumFontFamiliesExW");
			DetourAttach(&g_pOldEnumFontFamiliesExW, NewEnumFontFamiliesExW);
		}
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
	if (IsOpen.OpenlstrcpyW)
	{
		g_pOldlstrcpyW = DetourFindFunction("kernel32.dll", "lstrcpyW");
		DetourAttach(&g_pOldlstrcpyW, NewlstrcpyW);
	}
	if (IsOpen.OpenTextOutW)
	{
		g_pOldTextOutW = DetourFindFunction("GDI32.dll", "TextOutW");
		DetourAttach(&g_pOldTextOutW, NewTextOutW);
	}
	if (IsOpen.OpenGetDriveTypeW)
	{
		g_pOldGetDriveTypeW = DetourFindFunction("kernel32.dll", "GetDriveTypeW");
		DetourAttach(&g_pOldGetDriveTypeW, NewGetDriveTypeW);
	}
	if (IsOpen.OpenGetVolumeInformationW)
	{
		g_pOldGetVolumeInformationW = DetourFindFunction("kernel32.dll", "GetVolumeInformationW");
		DetourAttach(&g_pOldGetVolumeInformationW, NewGetVolumeInformationW);
	}
	if (IsOpen.OpenGetGlyphOutline)
	{
		if (GetPrivateProfileIntW(L"GetGlyphOutline", L"Type", 0, iniPath) == 0)
		{
			g_pOldGetGlyphOutlineA = DetourFindFunction("GDI32.dll", "GetGlyphOutline");
			DetourAttach(&g_pOldGetGlyphOutlineA, NewGetGlyphOutlineA);
		}
		else if (GetPrivateProfileIntW(L"GetGlyphOutline", L"Type", 0, iniPath) == 1)
		{
			g_pOldGetGlyphOutlineA = DetourFindFunction("GDI32.dll", "GetGlyphOutlineA");
			DetourAttach(&g_pOldGetGlyphOutlineA, NewGetGlyphOutlineA);
		}
		else if (GetPrivateProfileIntW(L"GetGlyphOutline", L"Type", 0, iniPath) == 2)
		{
			g_pOldGetGlyphOutlineW = DetourFindFunction("GDI32.dll", "GetGlyphOutlineW");
			DetourAttach(&g_pOldGetGlyphOutlineW, NewGetGlyphOutlineW);
		}
	}
	if (IsOpen.OpenMultiByteToWideChar)
	{
		g_pOldMultiByteToWideChar = DetourFindFunction("kernel32.dll", "MultiByteToWideChar");
		DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	}
	if (IsOpen.OpenWideCharToMultiByte)
	{
		g_pOldWideCharToMultiByte = DetourFindFunction("kernel32.dll", "WideCharToMultiByte");
		DetourAttach(&g_pOldWideCharToMultiByte, NewWideCharToMultiByte);
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
		g_pOldGetProcAddress = DetourFindFunction("kernel32.dll", "GetProcAddress");
		DetourAttach(&g_pOldGetProcAddress, NewGetProcAddress);
	}
	if (IsOpen.OpenCreateWindowEx)
	{
		if (GetPrivateProfileIntW(L"CreateWindowEx", L"Mode", 0, iniPath) == 0)
		{
			g_pOldCreateWindowExA = DetourFindFunction("USER32.dll", "CreateWindowExA");
			DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
		}
		else if (GetPrivateProfileIntW(L"CreateWindowEx", L"Mode", 0, iniPath) == 1)
		{
			g_pOldCreateWindowExW = DetourFindFunction("USER32.dll", "CreateWindowExW");
			DetourAttach(&g_pOldCreateWindowExW, NewCreateWindowExW);
		}
	}
	LONG ret = DetourTransactionCommit();
	return ret == NO_ERROR;
}

//卸载Hook
BOOL APIENTRY DropHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	if (IsOpen.OpenCreateFontIndirect)
	{
		if (GetPrivateProfileIntW(L"CreateFontIndirect", L"Mode", 0, iniPath) == 0)
			DetourDetach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
		if (GetPrivateProfileIntW(L"CreateFontIndirect", L"Mode", 0, iniPath) == 1)
			DetourDetach(&g_pOldCreateFontIndirectW, NewCreateFontIndirectW);
		if (GetPrivateProfileIntW(L"CreateFontIndirect", L"Mode", 0, iniPath) == 2)
		{
			DetourDetach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);
			DetourDetach(&g_pOldCreateFontIndirectW, NewCreateFontIndirectW);
		}
	}
	if (IsOpen.OpenCreateFont)
	{
		if (GetPrivateProfileIntW(L"CreateFont", L"Mode", 0, iniPath) == 0)
			DetourDetach(&g_pOldCreateFontA, NewCreateFontA);
		if (GetPrivateProfileIntW(L"CreateFont", L"Mode", 0, iniPath) == 1)
			DetourDetach(&g_pOldCreateFontW, NewCreateFontW);
		if (GetPrivateProfileIntW(L"CreateFont", L"Mode", 0, iniPath) == 2)
		{
			DetourDetach(&g_pOldCreateFontA, NewCreateFontA);
			DetourDetach(&g_pOldCreateFontW, NewCreateFontW);
		}
	}
	if (IsOpen.OpenEnumFontFamiliesEx)
	{
		if (GetPrivateProfileIntW(L"EnumFontFamiliesEx", L"Mode", 0, iniPath) == 0)
			DetourDetach(&g_pOldEnumFontFamiliesExA, NewEnumFontFamiliesExA);
		if (GetPrivateProfileIntW(L"EnumFontFamiliesEx", L"Mode", 0, iniPath) == 1)
			DetourDetach(&g_pOldEnumFontFamiliesExW, NewEnumFontFamiliesExW);
		if (GetPrivateProfileIntW(L"EnumFontFamiliesEx", L"Mode", 0, iniPath) == 2)
		{
			DetourDetach(&g_pOldEnumFontFamiliesExA, NewEnumFontFamiliesExA);
			DetourDetach(&g_pOldEnumFontFamiliesExW, NewEnumFontFamiliesExW);
		}
	}
	if (IsOpen.OpenCreateFileA)
		DetourDetach(&g_pOldCreateFileA, NewCreateFileA);
	if (IsOpen.OpenCreateFileW)
		DetourDetach(&g_pOldCreateFileW, NewCreateFileW);
	if (IsOpen.OpenlstrcpyW)
		DetourDetach(&g_pOldlstrcpyW, NewlstrcpyW);
	if (IsOpen.OpenTextOutW)
		DetourDetach(&g_pOldTextOutW, NewTextOutW);
	if (IsOpen.OpenGetDriveTypeW)
		DetourDetach(&g_pOldGetDriveTypeW, NewGetDriveTypeW);
	if(IsOpen.OpenGetVolumeInformationW)
		DetourDetach(&g_pOldGetVolumeInformationW, NewGetVolumeInformationW);
	if (IsOpen.OpenGetGlyphOutline)
	{
		free(tbl_data);
		if (GetPrivateProfileIntW(L"GetGlyphOutline", L"Type", 0, iniPath) == 0)
			DetourDetach(&g_pOldGetGlyphOutlineA, NewGetGlyphOutlineA);
		else if (GetPrivateProfileIntW(L"GetGlyphOutline", L"Type", 0, iniPath) == 1)
			DetourDetach(&g_pOldGetGlyphOutlineA, NewGetGlyphOutlineA);
		else if (GetPrivateProfileIntW(L"GetGlyphOutline", L"Type", 0, iniPath) == 2)
			DetourDetach(&g_pOldGetGlyphOutlineW, NewGetGlyphOutlineW);
	}
	if (IsOpen.OpenMultiByteToWideChar)
		DetourDetach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	if (IsOpen.OpenWideCharToMultiByte)
		DetourDetach(&g_pOldWideCharToMultiByte, NewWideCharToMultiByte);
	if (IsOpen.OpenSetWindowTextA)
		DetourDetach(&g_pOldSetWindowTextA, NewSetWindowTextA);
	if (IsOpen.OpenMessageBoxA)
		DetourDetach(&g_pOldMessageBoxA, NewMessageBoxA);
	if (IsOpen.OpenGetProcAddress)
		DetourDetach(&g_pOldGetProcAddress, NewGetProcAddress);
	if (IsOpen.OpenCreateWindowEx)
	{
		if (GetPrivateProfileIntW(L"CreateWindowEx", L"Mode", 0, iniPath) == 0)
			DetourDetach(&g_pOldCreateWindowExA, NewCreateWindowExA);
		if (GetPrivateProfileIntW(L"CreateWindowEx", L"Mode", 0, iniPath) == 1)
			DetourDetach(&g_pOldCreateWindowExW, NewCreateWindowExW);
	}
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