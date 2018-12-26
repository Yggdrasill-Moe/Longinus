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
	char *CMVS_PicName;
	unit32 CMVS_FileSize;
	unit32 CMVS_NameHash;
	unit32 CMVS_PicHash;
	unit32 CMVS_DataOffset;
	unit32 CMVS_PicOffset;
	BOOL CMVS_rGetFileName;
	BOOL CMVS_rGetDataSize;
	PVOID CMVS_pGetFileName;
	PVOID CMVS_pGetDataSize;
	PVOID CMVS_pCopyFileToMem;
	PVOID CMVS_pCopyPicToMem;
	FILE *LogFile;
} CMVS;

void CMVS_GetFileName();
void CMVS_GetDataSize();
void CMVS_CopyFileToMem();
void CMVS_CopyPicToMem();
void WINAPI CMVS_CheckFileName();
void WINAPI CMVS_CheckFileSize();
void WINAPI CMVS_CopyFile();
void WINAPI CMVS_CopyPicFile();