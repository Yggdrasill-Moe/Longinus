#include <pspkernel.h>

/* Define the module info section */
//在内核模式(PSP_MODULE_KERNEL)下载入基地址不会是用户模式(PSP_MODULE_USER)的0x08804000，如果以用户模式载入那整个游戏EBOOT基地址都要后移，
//可能造成金手指失效或者之前记录的函数地址都要后移，所以以内核模式载入，这样不会影响游戏EBOOT。
//但是！！！！！！以内核模式载入时实机会直接黑屏，所以还是只能用户模式，如果现在还存在实机玩家的话（比如我），然后需要在prx中自行移动Hook地址。
PSP_MODULE_INFO("Longinus_Start", PSP_MODULE_USER, 1, 0);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

void LonginusThread()
{
	int status;
	SceUID meboot = sceKernelLoadModule("disc0:/PSP_GAME/SYSDIR/EBOOT.DTX", 0, NULL);
	if (meboot >= 0)
	{
		SceUID mprx = sceKernelLoadModule("disc0:/PSP_GAME/SYSDIR/Longinus.prx", 0, NULL);
		if (mprx >= 0)
			sceKernelStartModule(mprx, 0, NULL, &status, NULL);
		sceKernelStartModule(meboot, 0, NULL, &status, NULL);
	}
	
}

/* Sets up the callback thread and returns its thread id */
SceUID SetupCallbacks()
{
	SceUID thid = sceKernelCreateThread("Longinus_Start", (void*) LonginusThread, 0x11, 0x1000, 0, 0);
	if(thid >= 0)
		sceKernelStartThread(thid, 0, 0);
	return thid;
}

int module_start(int argc, char *argv[])
{
	SetupCallbacks();
	sceKernelExitDeleteThread(0);
	return 0;
}