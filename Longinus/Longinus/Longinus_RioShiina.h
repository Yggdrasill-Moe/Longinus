#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <ctype.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

struct RS_s
{
	char *RS_FileName;
	unit32 RS_DataOffset;
	unit32 RS_DataSize;
	unit32 RS_NameHash;
	BOOL RS_rGetFileName;
	BOOL RS_rGetDataOffset;
	BOOL RS_rGetDataSize;
	PVOID RS_pGetFileName;
	PVOID RS_pGetDataOffset;
	PVOID RS_pGetDataSize;
	PVOID RS_pCopyFileToMem;
	FILE *LogFile;
} RS;


void RS_GetFileName();
void RS_GetDataOffset();
void RS_GetDataSize();
void RS_CopyFileToMem();
void WINAPI RS_CheckFileName();
void WINAPI RS_CheckFileSize();
void WINAPI RS_CopyFile();