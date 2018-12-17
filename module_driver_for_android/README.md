[CSDN链接](https://blog.csdn.net/u010164190/article/details/83900420)
module_driver_for_android是一个简单的android驱动,可以模块编译ko文件，还包含对ko签名文件具体步骤.

1.Android7.0步骤
注意：执行make命令之前，在android源码中执行以下命令(不然Makefile找不到aarch64-linux-android-交叉编译命令)      
#source build/envsetup.sh     
#lunch xxx-userdebug

2.注意：Android9.0      
需要改的位置：android9.0/build/envsetup.sh是软连接     
真正的位置：android9.0/build/make/envsetup.sh中修改: function make(){...}    
把Android7.0/build/envsetup.sh中的make()函数拷贝过来，替换了ndroid9.0/build/make/envsetup.sh中的make()函数即可。        
#source build/envsetup.sh     
#lunch xxx-userdebug
