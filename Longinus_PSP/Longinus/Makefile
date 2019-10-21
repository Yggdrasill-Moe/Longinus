TARGET = Longinus
OBJS = main.o

# Define to build this as a prx (instead of a static elf)
BUILD_PRX = 1
# Define the name of our custom exports (minus the .exp extension)
PRX_EXPORTS = exports.exp

CFLAGS = -O2 -G0 -Wall -fno-strict-aliasing
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS = 

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
