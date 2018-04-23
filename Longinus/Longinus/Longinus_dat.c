/*
用于读取Longinus.dat
made by Yggdrasill（Darkness-TX & Destinyの火狐）
2018.01.25
*/
#include "Longinus_dat.h"

unit32 BKDRhash(wchar_t* key)
{
	unit32 seed = 131;
	unit32 hash = 0;
	unit8 *p;
	while (*key != L'\n' && *key != L'\0')
	{
		p = (unit8 *)key++;
		hash = hash * seed + *p++;
		hash = hash * seed + *p;
	}
	return hash & 0x7fffffff;
}

BOOL Read_Dat(LPWSTR fname)
{
	FILE *src = _wfopen(fname, L"rb");
	if (src == NULL)
	{
		MessageBoxW(NULL, L"Longinus.dat不存在！\n请检查文件名是否正确\n或于配置文件中关闭Longinus_File。", L"错误", MB_OK);
		exit(0);
	}
	fread(&Dat_Index, sizeof(Dat_Index), 1, src);
	if (strncmp(Dat_Index.magic, "Longinus", 8))
	{
		fclose(src);
		MessageBoxW(NULL, L"Longinus_File的文件头不对！", L"错误", MB_OK);
		exit(0);
	}
	if (Dat_Index.version[0] != 1 || Dat_Index.version[1] != 3 || Dat_Index.version[2] != 0)
	{
		fclose(src);
		MessageBoxW(NULL, L"Longinus_File的版本不对！", L"错误", MB_OK);
		exit(0);
	}
	fseek(src, 0x10, SEEK_SET);
	if (Hash_Table_Build(src))
	{
		fclose(src);
		return TRUE;
	}
	fclose(src);
	MessageBoxW(NULL, L"Longinus_File读取出现异常！", L"错误", MB_OK);
	return FALSE;
}

BOOL Read_DatV4(LPWSTR fname)
{
	FILE *src = _wfopen(fname, L"rb");
	if (src == NULL)
	{
		MessageBoxW(NULL, L"Longinus.dat不存在！\n请检查文件名是否正确\n或于配置文件中关闭Longinus_File。", L"错误", MB_OK);
		exit(0);
	}
	fread(&Dat_IndexV4, sizeof(Dat_IndexV4), 1, src);
	if (strncmp(Dat_IndexV4.magic, "Longinus", 8))
	{
		fclose(src);
		MessageBoxW(NULL, L"Longinus_File的文件头不对！", L"错误", MB_OK);
		exit(0);
	}
	if (Dat_IndexV4.version[0] != 1 || Dat_IndexV4.version[1] != 4 || Dat_IndexV4.version[2] != 0)
	{
		fclose(src);
		MessageBoxW(NULL, L"Longinus_File的版本不对！", L"错误", MB_OK);
		exit(0);
	}
	if (Dat_IndexV4.mode != 1)
	{
		fclose(src);
		MessageBoxW(NULL, L"Longinus_File的模式不对！", L"错误", MB_OK);
		exit(0);
	}
	fseek(src, 0x10, SEEK_SET);
	if (Hash_Table_BuildV4(src))
	{
		fclose(src);
		return TRUE;
	}
	fclose(src);
	MessageBoxW(NULL, L"Longinus_File读取出现异常！", L"错误", MB_OK);
	return FALSE;
}

BOOL Hash_Table_Build(FILE *src)
{
	unit8 i = 0;
	unit32 k = 0, index_count = 0;
	unit16 str_count = 0;
	unit32 point = ftell(src);
	fseek(src, 0, SEEK_END);
	unit32 filesize = ftell(src);
	fseek(src, point, SEEK_SET);
	LinkIndex_Data q;
	q = malloc(sizeof(NodeIndex_Data));
	Index_Data = q;
	for (i = 0; i <= Dat_Index.count; i++)
	{
		NodeIndex_Data *index_node;
		index_node = malloc(sizeof(NodeIndex_Data));
		fread(&index_node->index, 1, 1, src);
		fread(&index_count, 3, 1, src);
		index_node->next = NULL;
		NodeHash_Data *hash_node;
		hash_node = malloc(sizeof(NodeHash_Data));
		hash_node->next = NULL;
		index_node->hash_data = hash_node;
		for (k = 0; k < index_count; k++)
		{
			fread(&str_count, 2, 1, src);
			fread(&hash_node->hash, 4, 1, src);
			LPWSTR str = malloc(str_count + 2);
			memset(str, 0, str_count + 2);
			fread(str, str_count, 1, src);
			hash_node->str_data = str;
			if (k != index_count - 1)
			{
				NodeHash_Data *hq;
				hq = malloc(sizeof(NodeHash_Data));
				hq->next = NULL;
				hash_node->next = hq;
				hash_node = hq;
			}
		}
		q->next = index_node;
		q = index_node;
	}
	if (ftell(src) == filesize)
		return TRUE;
	else
		return FALSE;
}

BOOL Hash_Table_BuildV4(FILE *src)
{
	unit32 i = 0;
	LinkIndex_DataV4 q;
	q = malloc(sizeof(NodeIndex_DataV4));
	Index_DataV4 = q;
	for (i = 0; i < Dat_IndexV4.count; i++)
	{
		NodeIndex_DataV4 *index_node;
		index_node = malloc(sizeof(NodeIndex_DataV4));
		fread(&index_node->hash, 4, 1, src);
		fread(&index_node->size, 4, 1, src);
		fread(&index_node->offset, 4, 1, src);
		index_node->next = NULL;
		q->next = index_node;
		q = index_node;
	}
	if (ftell(src) == (Dat_IndexV4.count * 0x0C + 0x10))
		return TRUE;
	else
		return FALSE;
}