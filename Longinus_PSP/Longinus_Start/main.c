/*
PSP版启动程序 v1.1
made by Yggdrasill（Darkness-TX & Destinyの火狐）
2023.04.15
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <pspge.h>
#include <pspdisplay.h>
#include <psploadexec_kernel.h>
/*
在内核模式(PSP_MODULE_KERNEL)下载入基地址不会是用户模式(PSP_MODULE_USER)的0x08804000，如果以用户模式载入那整个游戏EBOOT基地址都要后移，
可能造成金手指失效或者之前记录的函数地址都要后移，所以以内核模式载入，这样不会影响游戏EBOOT。
但是！！！！！！以内核模式载入时实机会直接黑屏，所以还是只能用户模式，如果现在还存在实机玩家的话（比如我），然后需要在prx中自行移动Hook地址。

2023.04.16更新：
似乎内核模式实机也可以运行，上面说的黑屏是另外的原因，这……判断失误了，误人子弟了啊QAQ，少有的翻车时刻，当初在自制程序这方面还是太新手了。
比起结论，思考的过程也是同样重要，黑历史就不删掉了。

总的来说模拟器能运行不代表实机就没问题，模拟器是以模拟运行过程为目的而开发的，而实机还有些权限的限制，模拟器不一定会还原这些限制，
这就造成了差异，上面遇到的黑屏问题，现在看来应该是内核模式使用sceKernelLoadModule载入EBOOT.BIN引起的。
没找到权限这方面的资料，个人猜测EBOOT.BIN必然是用户模式程序，本启动器如果是内核模式运行，
当使用sceKernelLoadModule加载EBOOT.BIN时会以内核模式加载，这越权了所以就崩溃了，
而用户模式启动本程序使用sceKernelLoadModule就会以用户模式加载EBOOT.BIN，没有越权所以执行成功。
但有些游戏的EBOOT.BIN使用用户模式加载为何也会崩溃？这种情况应该跟游戏本身的EBOOT.BIN设计有关系，开发者直接将内存地址的指针硬编码在EBOOT.BIN，
比如某游戏的EBOOT.BIN中有大量指向静态存储区域中文本地址的指针，而这些地址是基于EBOOT.BIN在基地址为0x08804000载入时的地址，
启动器以用户模式执行时基地址会占用0x08804000，导致后续加载EBOOT.BIN时造成EBOOT.BIN基地址后移，从而破坏了硬编码的指针地址造成了崩溃。

综上，使用内核模式启动本程序，以用户模式启动EBOOT.BIN，或者以不会占用EBOOT.BIN载入后的内存空间的方式以用户模式启动本程序，
再以用户模式启动EBOOT.BIN，即可规避上面的两个问题，但还是第一种方式更容易实现。
问题来了，怎么使用用户模式启动EBOOT.BIN？内核模式用sceKernelLoadModule加载EBOOT不是会出问题吗？
其实已经自带了有API了，上次认识不够没发现……QAQ
即sceKernelLoadExecVSHDisc，或sceKernelLoadModuleDisc，
sceKernelLoadModuleDisc在PSPSDK中没有，需要自行导入NID，建议使用sceKernelLoadModuleDisc，会返回SceUID，
有了SceUID然后可以sceKernelStartModule启动，而sceKernelLoadExecVSHDisc要先配置一波参数，非常麻烦，反正我没成功过。
*/
PSP_MODULE_INFO("Longinus_Start", PSP_MODULE_KERNEL, 1, 1);

PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

//3.XX时为0x1B97BDB3，660系统为0xD8320A28，自制系统自带NID转换，初始版本为0x1B97BDB3，所以这里导入NID就用老的
//int sceKernelLoadExecVSHDisc(const char *file, struct SceKernelLoadExecVSHParam *param);
//3.XX时为0xA1A78C58，660系统为0xCE0A74A5，自制系统自带NID转换，初始版本为0xA1A78C58，所以这里导入NID就用老的
//在M33系统和ppsspp模拟器中又被称为sceKernelLoadModuleForLoadExecVSHDisc，两者NID是一样的，只是名字不同
SceUID sceKernelLoadModuleDisc(const char *path, int flags, SceKernelLMOption *option);
/*
自制系统中的KUBridge插件使用的自制API，作用是可以在用户模式下使用内核API，比如加载模块，可以避免0x80020149(illegal permission call)错误，
而sceKernelLoadModule正好就有权限限制，启动器又是内核模式，插件又必须是用户模式，直接sceKernelLoadModule那就会遇到跟加载EBOOT一样的问题，
这里只用到了这一个KUBridge的API，内部实现上流程大概如下，pro系统在这API中顺便自带了NID转换，
但我不用kuKernelLoadModule而是去直接使用下面的代码实现却还是报0x80020149，唔，也许必须使用对应系统的正确NID才行？
u32 k1 = pspSdkSetK1(0);
sctrlKernelxxxxxx();//为对应sceKernelxxxx的自带NID转换的版本，自制系统实现
pspSdkSetK1(k1);
理论上，使用kuKernelLoadModule加载EBOOT应该也是没问题的？但都有纯正的原生系统API了，肯定用原生的啊。
*/
SceUID kuKernelLoadModule(const char *path, int flags, SceKernelLMOption *option);
/*
既然决定是内核态了，那有些东西就要注意一下了，比如malloc，经典的申请内存函数，嗯，是用户态的，
那到内核态怎么申请内核空间？就变成了kmalloc，但是PSPSDK没封装kmalloc的样子，要自己先创建堆，然后再申请空间，都有对应的系统API，
那内核态不能申请用户空间吗？这里请自行查询Linux内核态申请用户态空间的步骤做参考。

那该怎么做更方便？常用内存分配方式有3种，动态分配因为现在是内核态步骤变多了很麻烦，栈的空间又有限，那可以尝试直接划出块静态存储空间，
比如下面的就是划出的空间，但是内核空间只有8mb，除去系统必须占用的，剩下的很少，在实机上测试基本超1m的静态存储空间就报0x800200D9错。
但内核态可以访问所有内存空间，也许不用申请直接指示个用户内存空间的指针就能用？
如此错误的编程方式，不愧是灵活的C和指针，方式是否错误由我灵活定义，试了下好像还真的可以……。
更极端的情况下，vram也可以拿来做存储空间，保底4M呢。
u8 data[480*272*4];
*/
/*
void LonginusStartThreadsrwz2()
{
	int status, i = 0;
	u8 check = 0;
	u32 fontaddr = 0x89EAA20;
	u32* vram32;
	SceCtrlData pad;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	vram32 = sceGeEdramGetAddr();
	sceDisplaySetMode(0, 480, 272);
	sceDisplaySetFrameBuf(vram32, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
	SceUID fd = sceIoOpen("disc0:/PSP_GAME/SYSDIR/intro.bin", PSP_O_RDONLY, 0777);
	for (i = 0;i < 272*3;i++)
		sceIoRead(fd,(u8*)vram32 + i * 512 * 4,480 * 4);
	sceIoClose(fd);
	while(1)
	{
		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons != 0)
		{
			if (pad.Buttons & PSP_CTRL_LEFT)
			{
				sceDisplaySetFrameBuf((u8*)vram32+512*272*4, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
				sceDisplayWaitVblankStart();
				check = 0;
			}
			else if (pad.Buttons & PSP_CTRL_RIGHT)
			{
				sceDisplaySetFrameBuf((u8*)vram32+512*272*4*2, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
				sceDisplayWaitVblankStart();
				check = 1;
			}
			else if (pad.Buttons & PSP_CTRL_CIRCLE)
			{
				memset((u8*)vram32,0,512 * 272 * 4*3);
				sceDisplaySetFrameBuf(vram32, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
				sceDisplayWaitVblankStart();
				SceUID meboot = sceKernelLoadModuleDisc("disc0:/PSP_GAME/SYSDIR/EBOOT.DTX", 0, NULL);
				if (meboot >= 0)
				{
					if (check == 1)
					{
						fd = sceIoOpen("disc0:/PSP_GAME/SYSDIR/FONT.BIN", PSP_O_RDONLY, 0777);
						sceIoRead(fd,(u8 *)fontaddr,0x144800);
						sceIoClose(fd);
					}
					sceKernelStartModule(meboot, 0, NULL, &status, NULL);
				}
				sceKernelExitDeleteThread(0);
			}
		}
	}
}
*/
void LonginusStartThread()
{
	int status;
	SceUID meboot = sceKernelLoadModuleDisc("disc0:/PSP_GAME/SYSDIR/EBOOT.DTX", 0, NULL);
	if (meboot >= 0)
	{
		SceUID mprx = kuKernelLoadModule("disc0:/PSP_GAME/SYSDIR/Longinus.prx", 0, NULL);
		if (mprx >= 0)
			sceKernelStartModule(mprx, 0, NULL, &status, NULL);
		sceKernelStartModule(meboot, 0, NULL, &status, NULL);
		sceKernelExitDeleteThread(0);
	}
}

int module_start(int argc, char *argv[])
{
	//LonginusStartThread为基础版本，带插件加载，
	//LonginusStartThreadsrwz2用于机战z2汉化整合版，不带插件加载但有其他有意思的功能，所以保留下来，
	//平常情况直接用LonginusStartThread，LonginusStartThreadsrwz2也要注释掉不然会编译进去
	//SceUID thid = sceKernelCreateThread("Longinus_Start", (void*)LonginusStartThreadsrwz2, 0x11, 0x1000, 0, 0);
	SceUID thid = sceKernelCreateThread("Longinus_Start", (void*)LonginusStartThread, 0x11, 0x1000, 0, 0);
	if(thid >= 0)
		sceKernelStartThread(thid, 0, 0);
	sceKernelExitDeleteThread(0);
	return 0;
}