ifneq ($(KERNELRELEASE),)    
	XY-objs:=
	obj-m := XY.o
else
	KDIR := $GITHUB_WORKSPACE/android-kernel/out/*/common
all:
	make -C $(KDIR) M=$(PWD) O=$(KDIR) ARCH=arm64 CLANG_TRIPLE=aarch64-linux-gnu- CROSS_COMPILE=aarch64-linux-gnu- CROSS_COMPILE_ARM32=arm-linux-gnueabi- CROSS_COMPILE_COMPAT=arm-linux-gnueabi- CC=clang LD=ld.lld LLVM_IAS=1 SUBARCH=arm64 qaq.ko
clean:    
	rm -f *.o *.mod.o *.mod.c *.order *.cmd
endif
