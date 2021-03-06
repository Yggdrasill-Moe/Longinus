#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

typedef struct Hash_Data_s
{
	unit32 hash;
	LPWSTR str_data;
	struct Hash_Data_s *next;
}NodeHash_Data, *LinkHash_Data;

typedef struct Index_Data_s
{
	unit8 index;
	LinkHash_Data hash_data;
	struct Index_Data_s *next;
}NodeIndex_Data, *LinkIndex_Data;
LinkIndex_Data Index_Data;

typedef struct Index_DataV4_s
{
	unit32 hash;
	unit32 size;
	unit32 offset;
	struct Index_DataV4_s *next;
}NodeIndex_DataV4, *LinkIndex_DataV4;
LinkIndex_DataV4 Index_DataV4;

struct Dat_Index_s
{
	unit8 magic[8];
	unit8 version[3];
	unit8 count;
}Dat_Index;

struct Dat_IndexV4_s
{
	unit8 magic[8];
	unit8 version[3];
	unit8 mode;
	unit32 count;
}Dat_IndexV4;

BOOL Hash_Table_Build(FILE *src);
BOOL Hash_Table_BuildV4(FILE *src);
BOOL Read_Dat(LPWSTR fname);
BOOL Read_DatV4(LPWSTR fname);
unit32 BKDRhash(wchar_t* key);