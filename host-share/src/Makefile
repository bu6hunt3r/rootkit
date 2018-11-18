# Module name
ROOTKIT		:= rk

# Build
MODULEDIR	:= /lib/modules/$(shell uname -r)
BUILDDIR	:= $(MODULEDIR)/build
KERNELDIR 	:= $(MODULEDIR)/kernel

# Header files
SRCS_H		:= $(PWD)/$(SRCS_S)/headers

# Module
obj-m 		:= $(ROOTKIT).o

ccflags-y	:= -I$(SRCS_H)

# Recipes
all:
	$(MAKE) -C $(BUILDDIR) M=$(PWD) modules

load:
	insmod rk.ko

clean:
	$(MAKE) -C $(BUILDDIR) M=$(PWD) clean