TARGET = EBOOT

OBJS = ModuleMgrForKernel.o KUBridge.o main.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall -std=gnu99
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS =

PSP_FW_VERSION = 500
PRX_EXPORTS = exports.exp

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
rename:
	rm -f EBOOT.BIN
	ren EBOOT.prx EBOOT.BIN