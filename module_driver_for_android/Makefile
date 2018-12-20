MODNAME=print
#1.Android平台写法
KERNELDIR=out/target/product/msmxxx/obj/kernel/msm-3.18
ARCH=arm64
CROSS_COMPILE=aarch64-linux-android-

#2.Linux平台写法
#KERNELDIR=~/linux/kernel
#ARCH=arm64
#CROSS_COMPILE=~/linux/prebuilts/gcc/linux-x86/aarch64/gcc-xxx_aarch64-linux-gnu/bin/aarch64-linux-gnu-

obj-m += $(MODNAME).o

default:
	$(MAKE) -C $(KERNELDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)  M=$(PWD) modules

clean:
	rm -fr *.mod.* *.o *.ko .$(MODNAME).* .tmp* Module.* modules.*
