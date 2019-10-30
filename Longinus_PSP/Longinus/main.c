#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspkernel.h>
//#include <png.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

/* Define the module info section */
PSP_MODULE_INFO("Longinus", PSP_MODULE_USER, 1, 0);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

unit32 GetFileName_add = 0;
unit32 GetStruct_add = 0;
unit32 GetFile_add = 0;
//unit32 Isfirst = 1;

unit16 Width = 0;
unit16 Height = 0;
unit32 Frame = 0;
unit32 FrameNum = 0;
unit32 BitAddress = 0;
unit8 ITVName[32] = { 0 };
unit8 dstName[128] = { 0 };
//unit32 pal[256] = { 0 };

/*//不知道是不是libpng库太老了只要png_write_info就崩溃，所以注释掉
void WritePng(FILE *fp,unit32 width,unit32 height,unit32 *pal,unit8 *data)
{
	unit32 i = 0;
	png_colorp pcolor = NULL;
	unit8 *png_alpha = NULL, *paldata = (unit8 *)pal;
	pcolor = (png_colorp)malloc(0x100 * sizeof(png_color));
	png_alpha = (unit8 *)malloc(0x100 * sizeof(unit8));
	for (i = 0; i < 0x100; i++)
	{
		pcolor[i].red = paldata[i * 4 + 0];
		pcolor[i].green = paldata[i * 4 + 1];
		pcolor[i].blue = paldata[i * 4 + 2];
		png_alpha[i] = paldata[i * 4 + 3];
	}
	png_structp png_ptr;
	png_infop info_ptr;
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_set_PLTE(png_ptr, info_ptr, pcolor, 0x100);
	png_set_tRNS(png_ptr, info_ptr, (png_bytep)png_alpha, 0x100, (png_color_16p)0);
	png_write_info(png_ptr, info_ptr);
	for (i = 0; i < height; i++)
		png_write_row(png_ptr, data + i * width);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	free(pcolor);
	free(png_alpha);
}
*/
/*
void CheckITV()
{
	if (Isfirst)
	{
		if (GetFileName_add != 0)
		{
			strcpy(ITVName,(unit8 *)GetFileName_add);
			GetFileName_add = 0;
			sprintf(dstName,"ms0:/%s",ITVName);
			sceIoMkdir(dstName,0777);
			FrameNum = *(unit32 *)(GetStruct_add + 0x8);
			Width = *(unit16 *)(GetStruct_add + 0x16);
			Height = *(unit16 *)(GetStruct_add + 0x18);
			BitAddress = *(unit32 *)(GetStruct_add + 0x1C);
			GetFile_add = *(unit32 *)(GetStruct_add + 0x3C);
			Frame = *(unit32 *)(GetStruct_add + 0x48);
			memcpy(pal,(unit8 *)(GetFile_add + 0x1E),0x100 * 4);
			sprintf(dstName,"ms0:/%s/%03d.bin",ITVName,Frame);
			SceUID fd = sceIoOpen(dstName, PSP_O_WRONLY|PSP_O_CREAT, 0777);
			//FILE *fp = fopen(dstName,"wb");
			//WritePng(fp,Width,Height,pal,(unit8 *)BitAddress);
			//fclose(fp);
			sceIoWrite(fd,&Width,2);
			sceIoWrite(fd,&Height,2);
			sceIoWrite(fd,(unit8 *)pal,0x400);
			sceIoWrite(fd,(unit8 *)BitAddress,Width * Height);
			sceIoClose(fd);
		}
		Isfirst = 0;
	}
	else
	{
		Frame = *(unit32 *)(GetStruct_add + 0x48);
		sprintf(dstName,"ms0:/%s/%03d.bin",ITVName,Frame);
		SceUID fd = sceIoOpen(dstName, PSP_O_WRONLY|PSP_O_CREAT, 0777);
		//FILE *fp = fopen(dstName,"wb");
		//WritePng(fp,Width,Height,pal,(unit8 *)BitAddress);
		//fclose(fp);
		sceIoWrite(fd,&Width,2);
		sceIoWrite(fd,&Height,2);
		sceIoWrite(fd,(unit8 *)pal,0x400);
		sceIoWrite(fd,(unit8 *)BitAddress,Width * Height);
		sceIoClose(fd);
		if (Frame == FrameNum - 1)
			Isfirst = 1;
	}
}
*/
void BuildInfo()
{
	if (GetFileName_add != 0)
	{
		strcpy(ITVName,(unit8 *)GetFileName_add);
		GetFileName_add = 0;
		FrameNum = *(unit32 *)(GetStruct_add + 0x8);
		Width = *(unit16 *)(GetStruct_add + 0x16);
		Height = *(unit16 *)(GetStruct_add + 0x18);
		BitAddress = *(unit32 *)(GetStruct_add + 0x1C);
		GetFile_add = *(unit32 *)(GetStruct_add + 0x3C);
		sprintf(dstName,"disc0:/PSP_GAME/USRDIR/mmv3/%s/pal.bin",ITVName);
		SceUID fd = sceIoOpen(dstName, PSP_O_RDONLY, 0777);
		sceIoRead(fd,(unit8 *)(GetFile_add + 0x1E),0x100 * 4);
		sceIoClose(fd);
	}
}

void ImportFrame()
{
	Frame = *(unit32 *)(GetStruct_add + 0x48);
	sprintf(dstName,"disc0:/PSP_GAME/USRDIR/mmv3/%s/%03d.bin",ITVName,Frame);
	SceUID fd = sceIoOpen(dstName, PSP_O_RDONLY, 0777);
	sceIoRead(fd,(unit8 *)BitAddress,Width * Height);
	sceIoClose(fd);
}
/*
void GetFileName()
{
	//这里只是sp加0x10后正好是文件名所在地址而已，没有特别的含义
	asm("addiu $sp,$sp,0x10");
	//将寄存器sp的值存储到变量中
	asm("sw $sp,GetFileName_add");
	//因为延迟指令的关系其实这条最好是放在jr ra后面
	asm("addiu $sp,$sp,-0x10");
	//返回上上个函数，对，上上个，等于帮上个函数做了返回操作
	asm("jr $ra");
}

void GetStruct()
{
	asm("sw $s0,GetStruct_add");
	//由于这里使用了jal，所以要开个栈保存ra跳转的地址
	asm("addiu $sp,$sp,-0x4");
	asm("sw $ra,0x0($sp)");
	asm("jal CheckITV");
	//还原ra
	asm("lw $ra,0x0($sp)");
	asm("addiu $sp,$sp,0x4");
	asm("jr $ra");
}
*/
void GetInfo()
{
	asm("addiu $sp,$sp,0x10");
	asm("sw $sp,GetFileName_add");
	asm("sw $s0,GetStruct_add");
	asm("addiu $sp,$sp,-0x10");
	//由于这里使用了jal，所以要开个栈保存ra跳转的地址
	asm("addiu $sp,$sp,-0x4");
	asm("sw $ra,0x0($sp)");
	asm("jal BuildInfo");
	asm("lw $ra,0x0($sp)");
	asm("addiu $sp,$sp,0x4");
	asm("jr $ra");
}

void ImportITV()
{
	//由于这里使用了jal，所以要开个栈保存ra跳转的地址
	asm("addiu $sp,$sp,-0x4");
	asm("sw $ra,0x0($sp)");
	asm("jal ImportFrame");
	asm("lw $ra,0x0($sp)");
	asm("addiu $sp,$sp,0x4");
	asm("jr $ra");
}

int module_start(int argc, char *argv[])
{
	//内存中的hook地址，为了方便选择了函数最后的jr ra返回指令覆盖成j xxxxxxxx，跳转后一般已经还原了堆栈
	unit32 hook_GetFileName = 0x0897A1DC;
	// j xxxxxxxx，MIPS指令是4字节对齐，所以j xxxxxxxx为内存地址右移两位后或j的opcode
	//*(unit32 *)(hook_GetFileName) = 0x08000000 | (((unit32)GetFileName) & 0x0FFFFFFC) >> 2;
	*(unit32 *)(hook_GetFileName) = 0x08000000 | (((unsigned int)GetInfo) & 0x0FFFFFFC) >> 2;
	//最后需要刷新下内存
	sceKernelDcacheWritebackInvalidateRange((const void *)hook_GetFileName, 4);
	sceKernelIcacheInvalidateRange((const void *)hook_GetFileName, 4);
	unit32 hook_GetStruct = 0x0897A020;
	//*(unit32 *)(hook_GetStruct) = 0x08000000 | (((unit32)GetStruct) & 0x0FFFFFFC) >> 2;
	*(unit32 *)(hook_GetStruct) = 0x08000000 | (((unsigned int)ImportITV) & 0x0FFFFFFC) >> 2;
	sceKernelDcacheWritebackInvalidateRange((const void *)hook_GetStruct, 4);
	sceKernelIcacheInvalidateRange((const void *)hook_GetStruct, 4);
	return 0;
}

