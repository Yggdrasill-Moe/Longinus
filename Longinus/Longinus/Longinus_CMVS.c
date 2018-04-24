/*
CMVS引擎汉化临时方案
made by Yggdrasill（Darkness-TX & Destinyの火狐）
2018.04.24
*/
#include "Longinus_CMVS.h"
#include "Longinus_dat.h"

__declspec(naked)void CMVS_GetFileName()
{
	__asm
	{
		pushad
		mov CMVS.CMVS_FileName, edi
		mov CMVS.CMVS_rGetFileName, 1
		call CMVS_CheckFileName
		popad
		jmp CMVS.CMVS_pGetFileName
	}
}

__declspec(naked)void CMVS_GetDataSize()
{
	__asm
	{
		pushad
		mov CMVS.CMVS_FileSize, eax
		mov CMVS.CMVS_rGetDataSize, 1
		call CMVS_CheckFileSize
		popad
		mov eax, CMVS.CMVS_FileSize
		jmp CMVS.CMVS_pGetDataSize
	}
}

__declspec(naked)void CMVS_CopyFileToMem()
{
	__asm
	{
		pushad
		mov CMVS.CMVS_DataOffset, ebx
		call CMVS_CopyFile
		popad
		jmp CMVS.CMVS_pCopyFileToMem
	}
}

void WINAPI CMVS_CheckFileName()
{
	char* FileName_Up = malloc(strlen(CMVS.CMVS_FileName) + 1);
	memset(FileName_Up, 0, strlen(CMVS.CMVS_FileName) + 1);
	for (unit32 i = 0; i < strlen(CMVS.CMVS_FileName); i++)
		FileName_Up[i] = toupper(CMVS.CMVS_FileName[i]);
	wchar_t* wFileName_Up = malloc(strlen(CMVS.CMVS_FileName) * 2 + 2);
	memset(wFileName_Up, 0, strlen(CMVS.CMVS_FileName) * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, FileName_Up, -1, wFileName_Up, strlen(CMVS.CMVS_FileName));
	unit32 Hash = BKDRhash(wFileName_Up);
	free(FileName_Up);
	free(wFileName_Up);
	NodeIndex_DataV4 *q = Index_DataV4;
	while (q->next != NULL)
	{
		q = q->next;
		if (Hash == q->hash)
		{
			CMVS.CMVS_NameHash = Hash;
			break;
		}
	}
}

void WINAPI CMVS_CheckFileSize()
{
	if (CMVS.CMVS_NameHash)
	{
		NodeIndex_DataV4 *q = Index_DataV4;
		while (q->next != NULL)
		{
			q = q->next;
			if (CMVS.CMVS_NameHash == q->hash)
			{
				if (q->size > CMVS.CMVS_FileSize)
					CMVS.CMVS_FileSize = q->size;
				break;
			}
		}
	}
}

void WINAPI CMVS_CopyFile()
{
	if (CMVS.CMVS_NameHash && CMVS.CMVS_rGetDataSize && CMVS.CMVS_pGetFileName)
	{
		NodeIndex_DataV4 *q = Index_DataV4;
		while (q->next != NULL)
		{
			q = q->next;
			if (CMVS.CMVS_NameHash == q->hash)
			{
				unit8 *data = malloc(CMVS.CMVS_FileSize);
				memset(data, 0, CMVS.CMVS_FileSize);
				FILE *src = fopen("Longinus.dat", "rb");
				fseek(src, q->offset, SEEK_SET);
				fread(data, q->size, 1, src);
				fclose(src);
				memcpy((LPVOID)CMVS.CMVS_DataOffset, data, CMVS.CMVS_FileSize);
				free(data);
				break;
			}
		}
		if (CMVS.LogFile)
		{
			fputs(CMVS.CMVS_FileName, CMVS.LogFile);
			fputs("|", CMVS.LogFile);
			fprintf(CMVS.LogFile, "Hash:0x%X", CMVS.CMVS_NameHash);
			fputs("|", CMVS.LogFile);
			fprintf(CMVS.LogFile, "Size:0x%X", CMVS.CMVS_FileSize);
			fputs("|", CMVS.LogFile);
			fprintf(CMVS.LogFile, "Offset:0x%X", CMVS.CMVS_DataOffset);
			fputs("\n", CMVS.LogFile);
		}
		CMVS.CMVS_rGetDataSize = 0;
		CMVS.CMVS_rGetFileName = 0;
		CMVS.CMVS_NameHash = 0;
	}
}