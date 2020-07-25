编译v4l2-ctl for android
1.将stlport和v4l-utils拷贝到android的external目录
# cp -rf stlport v4l-utils ~/android/external

2.编译
# cd ~/android/external/stlport
# mm -j16
# cd /android/external/v4l-utils/utils/v4l2-ctl
# mm -j16

3.将libstlport.so与v4l2-ctl push到android
# adb push libstlport.so /system/lib
# adb push v4l2-ctl /system/bin

# v4l2-ctl -D
Driver Info (not using libv4l2):
	Driver name   : comip
	Card type     : comip
	Bus info      : isp_output0
	Driver version: 2.0.0
	Capabilities  : 0x04000001
		Video Capture
		Streaming
 
注意:
v4l2-bin目录为编译好的arm 32　for android.
