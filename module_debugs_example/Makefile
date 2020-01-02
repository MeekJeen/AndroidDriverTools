MODNAME=debugfs_test

KERNELDIR=/home/test/android/out/target/product/msm89xx_64/obj/kernel/msm-3.18
ARCH=arm64
CROSS_COMPILE=aarch64-linux-android-

obj-m += $(MODNAME).o
default:
	$(MAKE) -C $(KERNELDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)  M=$(PWD) modules

clean:
	rm -fr *.mod.* *.o *.ko .$(MODNAME).* .tmp* Module.* modules.*
