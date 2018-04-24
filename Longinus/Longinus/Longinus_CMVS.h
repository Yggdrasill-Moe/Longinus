#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <ctype.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

struct CMVS_s
{
	char *CMVS_FileName;
	unit32 CMVS_FileSize;
	unit32 CMVS_NameHash;
	unit32 CMVS_DataOffset;
	BOOL CMVS_rGetFileName;
	BOOL CMVS_rGetDataSize;
	PVOID CMVS_pGetFileName;
	PVOID CMVS_pGetDataSize;
	PVOID CMVS_pCopyFileToMem;
	FILE *LogFile;
} CMVS;

void CMVS_GetFileName();
void CMVS_GetDataSize();
void CMVS_CopyFileToMem();
void WINAPI CMVS_CheckFileName();
void WINAPI CMVS_CheckFileSize();
void WINAPI CMVS_CopyFile();