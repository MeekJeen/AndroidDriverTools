module_driver_for_android是一个简单的android驱动,可以模块编译ko文件，还包含对ko签名文件具体步骤.

[CSDN链接](https://blog.csdn.net/u010164190/article/details/83900420)

注意：执行make命令之前，在android源码中执行以下命令(不然Makefile找不到aarch64-linux-android-交叉编译命令)
#source build/envsetup.sh
#lunch xxx
