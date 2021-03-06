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
	BOOL OpenLonginusFile;
	BOOL OpenCreateFontIndirect;
	BOOL OpenCreateFont;
	BOOL OpenMultiByteToWideChar;
	BOOL OpenWideCharToMultiByte;
	BOOL OpenGetGlyphOutline;
	BOOL OpenCreateFileA;
	BOOL OpenCreateFileW;
	BOOL OpenSetWindowTextA;
	BOOL OpenSetWindowTextW;
	BOOL OpenMessageBoxA;
	BOOL OpenMessageBoxW;
	BOOL OpenCreateWindowEx;
	BOOL OpenGetProcAddress;
	BOOL OpenEnumFontFamiliesA;
	BOOL OpenEnumFontFamiliesEx;
	BOOL OpenGetDriveType;
	BOOL OpenGetVolumeInformationW;
	BOOL OpenFindFirstFile;
	BOOL OpenBorderPatch;
	BOOL OpenChangeFace;
	BOOL OpenlstrcpyW;
	BOOL OpenlstrcpyA;
	BOOL OpenTextOutA;
	BOOL OpenTextOutW;
	BOOL OpenLonginusLog;
	BOOL OpenLonginusPlus;
}IsOpen;

typedef struct SetWindowTextA_Replace_s
{
	char OldString[255];
	char NewString[255];
	struct SetWindowTextA_Replace_s *next;
}NodeSetWindowTextA_Replace, *LinkSetWindowTextA_Replace;
LinkSetWindowTextA_Replace SetWindowTextA_Replace;

typedef struct SetWindowTextW_Replace_s
{
	wchar_t OldString[255];
	wchar_t NewString[255];
	struct SetWindowTextW_Replace_s *next;
}NodeSetWindowTextW_Replace, *LinkSetWindowTextW_Replace;
LinkSetWindowTextW_Replace SetWindowTextW_Replace;

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

typedef struct MessageBoxW_TextReplace_s
{
	wchar_t OldString[255];
	wchar_t NewString[255];
	struct MessageBoxW_TextReplace_s *next;
}NodeMessageBoxW_TextReplace, *LinkMessageBoxW_TextReplace;
LinkMessageBoxW_TextReplace MessageBoxW_TextReplace;

typedef struct MessageBoxW_CaptionReplace_s
{
	wchar_t OldString[255];
	wchar_t NewString[255];
	struct MessageBoxW_CaptionReplace_s *next;
}NodeMessageBoxW_CaptionReplace, *LinkMessageBoxW_CaptionReplace;
LinkMessageBoxW_CaptionReplace MessageBoxW_CaptionReplace;

typedef struct CreateFileA_Replace_s
{
	char OldFileName[MAX_PATH];
	char NewFileName[MAX_PATH];
	struct CreateFileA_Replace_s *next;
}NodeCreateFileA_Replace, *LinkCreateFileA_Replace;
LinkCreateFileA_Replace CreateFileA_Replace;

typedef struct CreateFileW_Replace_s
{
	wchar_t OldFileName[MAX_PATH];
	wchar_t NewFileName[MAX_PATH];
	struct CreateFileW_Replace_s *next;
}NodeCreateFileW_Replace, *LinkCreateFileW_Replace;
LinkCreateFileW_Replace CreateFileW_Replace;
//汉化的话lpString2的长度应该不会超MAX_PATH吧。。。。
typedef struct lstrcpyW_Replace_s
{
	wchar_t OldlpString2[MAX_PATH];
	wchar_t NewlpString2[MAX_PATH];
	struct lstrcpyW_Replace_s *next;
}NodelstrcpyW_Replace, *LinklstrcpyW_Replace;
LinklstrcpyW_Replace lstrcpyW_Replace;

typedef struct TextOutA_Replace_s
{
	unit8 OldlpString[MAX_PATH];
	unit8 NewlpString[MAX_PATH];
	struct TextOutA_Replace_s *next;
}NodeTextOutA_Replace, *LinkTextOutA_Replace;
LinkTextOutA_Replace TextOutA_Replace;

typedef struct TextOutW_Replace_s
{
	wchar_t OldlpString[MAX_PATH];
	wchar_t NewlpString[MAX_PATH];
	struct TextOutW_Replace_s *next;
}NodeTextOutW_Replace, *LinkTextOutW_Replace;
LinkTextOutW_Replace TextOutW_Replace;

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

BYTE CreateFontIndirect_CharSet;
BYTE CreateFont_CharSet;
DWORD MultiByteToWideChar_CodePage;
DWORD WideCharToMultiByte_CodePage;
BYTE EnumFontFamiliesEx_CharSet;
BYTE EnumFontFamiliesA_CharSet;
DWORD EnumFontFamiliesA_MemOffset;
BOOL First_Face_Patch = FALSE;
BOOL First_Border_Patch = FALSE;
BOOL ChangeFace_All = FALSE;
DWORD oldProtect;
unit16 *tbl_data;
FILE *LogFile_Char = NULL;
FILE *LogFile_Wchar = NULL;

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