#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <detours.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

struct IsOpen_s
{
	BOOL OpenCreateFontIndirectA;
	BOOL OpenCreateFontA;
	BOOL OpenMultiByteToWideChar;
	BOOL OpenGetGlyphOutlineW;
	BOOL OpenGetGlyphOutlineA;
	BOOL OpenCreateFileA;
	BOOL OpenCreateFileW;
	BOOL OpenSetWindowTextA;
	BOOL OpenMessageBoxA;
	BOOL OpenGetProcAddress;
	BOOL OpenEnumFontFamiliesA;
	BOOL OpenBorderPatch;
	BOOL OpenChangeFace;
}IsOpen;

typedef struct SetWindowTextA_Replace_s
{
	char OldString[255];
	char NewString[255];
	struct SetWindowTextA_Replace_s *next;
}NodeSetWindowTextA_Replace, *LinkSetWindowTextA_Replace;
LinkSetWindowTextA_Replace SetWindowTextA_Replace;

typedef struct MessageBoxA_TextReplace_s
{
	char OldString[255];
	char NewString[255];
	struct MessageBoxA_TextReplace_s *next;
}NodeMessageBoxA_TextReplace, *LinkMessageBoxA_TextReplace;
LinkMessageBoxA_TextReplace MessageBoxA_TextReplace;

typedef struct MessageBoxA_CaptionReplace_s
{
	char OldString[255];
	char NewString[255];
	struct MessageBoxA_CaptionReplace_s *next;
}NodeMessageBoxA_CaptionReplace, *LinkMessageBoxA_CaptionReplace;
LinkMessageBoxA_CaptionReplace MessageBoxA_CaptionReplace;

typedef struct CreateFileA_Replace_s
{
	char OldFileName[255];
	char NewFileName[255];
	struct CreateFileA_Replace_s *next;
}NodeCreateFileA_Replace, *LinkCreateFileA_Replace;
LinkCreateFileA_Replace CreateFileA_Replace;

typedef struct CreateFileW_Replace_s
{
	wchar_t OldFileName[255];
	wchar_t NewFileName[255];
	struct CreateFileW_Replace_s *next;
}NodeCreateFileW_Replace, *LinkCreateFileW_Replace;
LinkCreateFileW_Replace CreateFileW_Replace;

typedef struct Border_Patch_s
{
	DWORD Offset;
	BYTE Border;
	struct Border_Patch_s *next;
}NodeBorder_Patch, *LinkBorder_Patch;
LinkBorder_Patch Border_Patch;

typedef struct ChangeFace_s
{
	DWORD Offset;
	DWORD Size;
	char Face[20];
	struct ChangeFace_s *next;
}NodeChangeFace, *LinkChangeFace;
LinkChangeFace Change_Face;

wchar_t exePath[MAX_PATH];
wchar_t iniPath[MAX_PATH];
wchar_t dirPath[MAX_PATH];

BYTE CreateFontIndirectA_CharSet;
BYTE CreateFontA_CharSet;
DWORD MultiByteToWideChar_CodePage;
BYTE EnumFontFamiliesA_CharSet;
DWORD EnumFontFamiliesA_MemOffset;
BOOL First_Face_Patch = FALSE;
BOOL First_Border_Patch = FALSE;
BOOL ChangeFace_All = FALSE;
DWORD oldProtect;
unit16 *tbl_data;

void ChangeFace(HMODULE hModule, unit32 Offset, unit32 Size, char *Face);
void BorderPatch(HMODULE hModule, unit32 Offset, unit8 Border);
void GetSettings();
DWORD CheckString(wchar_t *buff)
{
	if (wcsncmp(buff, L"0x", 2) == 0 || wcsncmp(buff, L"0X", 2) == 0)
		return wcstol(buff + 2, NULL, 16);
	else
		return wcstol(buff, NULL, 10);
}