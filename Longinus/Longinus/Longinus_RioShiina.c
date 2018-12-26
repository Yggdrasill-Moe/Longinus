/*
RioShiina���溺����ʱ����
made by Yggdrasill��Darkness-TX & Destiny�λ����
2018.04.05
*/
#include "Longinus_RioShiina.h"
#include "Longinus_dat.h"
/*
__declspec(naked)�ñ��������Ż�������ʵ��������ʵ�֣�����jmp���͵�hook��
�����������������������ʱ�����push����ټ���pop��ret���ֶ�ջƽ�⣬
������Υ������ת�Լ��ĺ���ִ�����ٻص�ԭ������Ŀ�ģ�
���������Ҫ����pop��jmp��������__declspec(naked)�Ͳ��á�
����������Բ��Ӻͼ��Ϻ�������У���OD���������̿���ʵ�ֵ�����
�������������Ļ������ڿ�ͷ�Ӹ�messagebox��ͣ���ٸ��ӣ�
���Ӻ������Ctrl+Gֱ����ת��hook��ַ��һ��������jmp����ѧϰ����������
naked�����Ͳ�Ҫ�������ˣ�Ҫ���Ļ�����д����һ������Ȼ����naked������
��push aaaa��call bbbb
*/
__declspec(naked)void RS_GetFileName()
{
	__asm
	{
		pushad
		mov RS.RS_FileName, edi
		mov RS.RS_rGetFileName, 1
		call RS_CheckFileName
		popad
		//pop edi
		//pop esi
		//pop ebx
		//pop ebp
		jmp RS.RS_pGetFileName
	}
}

__declspec(naked)void RS_GetDataSize()
{
	__asm
	{
		pushad
		mov RS.RS_DataSize, ebx
		mov RS.RS_rGetDataSize, 1
		call RS_CheckFileSize
		popad
		mov ebx, RS.RS_DataSize
		jmp RS.RS_pGetDataSize
	}
}

__declspec(naked)void RS_GetDataOffset()
{
	__asm
	{
		pushad
		mov RS.RS_DataOffset, esi
		mov RS.RS_rGetDataOffset, 1
		popad
		jmp RS.RS_pGetDataOffset
	}
}

__declspec(naked)void RS_CopyFileToMem()
{
	__asm
	{
		pushad
		call RS_CopyFile
		popad
		jmp RS.RS_pCopyFileToMem
	}
}

void WINAPI RS_CheckFileName()
{
	char* FileName_Up = malloc(strlen(RS.RS_FileName) + 1);
	memset(FileName_Up, 0, strlen(RS.RS_FileName) + 1);
	for (unit32 i = 0; i < strlen(RS.RS_FileName); i++)
		FileName_Up[i] = toupper(RS.RS_FileName[i]);
	wchar_t* wFileName_Up = malloc(strlen(RS.RS_FileName) * 2 + 2);
	memset(wFileName_Up, 0, strlen(RS.RS_FileName) * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, FileName_Up, -1, wFileName_Up, strlen(RS.RS_FileName));
	unit32 Hash = BKDRhash(wFileName_Up);
	free(FileName_Up);
	free(wFileName_Up);
	NodeIndex_DataV4 *q = Index_DataV4;
	while (q->next != NULL)
	{
		q = q->next;
		if (Hash == q->hash)
		{
			RS.RS_NameHash = Hash;
			break;
		}
	}
}

void WINAPI RS_CheckFileSize()
{
	if (RS.RS_NameHash)
	{
		NodeIndex_DataV4 *q = Index_DataV4;
		while (q->next != NULL)
		{
			q = q->next;
			if (RS.RS_NameHash == q->hash)
			{
				if (q->size > RS.RS_DataSize)
					RS.RS_DataSize = q->size;
				break;
			}
		}
	}
}

void WINAPI RS_CopyFile()
{
	if (RS.RS_NameHash && RS.RS_rGetFileName && RS.RS_rGetDataSize && RS.RS_rGetDataOffset)
	{
		NodeIndex_DataV4 *q = Index_DataV4;
		while (q->next != NULL)
		{
			q = q->next;
			if (RS.RS_NameHash == q->hash)
			{
				unit8 *data = malloc(RS.RS_DataSize);
				memset(data, 0, RS.RS_DataSize);
				FILE *src = fopen("Longinus.dat", "rb");
				fseek(src, q->offset, SEEK_SET);
				fread(data, q->size, 1, src);
				fclose(src);
				memcpy((LPVOID)RS.RS_DataOffset, data, RS.RS_DataSize);
				free(data);
				break;
			}
		}
		if (RS.LogFile)
		{
			fputs(RS.RS_FileName, RS.LogFile);
			fputs("|", RS.LogFile);
			fprintf(RS.LogFile, "Hash:0x%X", RS.RS_NameHash);
			fputs("|", RS.LogFile);
			fprintf(RS.LogFile, "Size:0x%X", RS.RS_DataSize);
			fputs("|", RS.LogFile);
			fprintf(RS.LogFile, "Offset:0x%X", RS.RS_DataOffset);
			fputs("\n", RS.LogFile);
		}
		RS.RS_rGetFileName = 0;
		RS.RS_rGetDataSize = 0;
		RS.RS_rGetDataOffset = 0;
		RS.RS_NameHash = 0;
	}
}