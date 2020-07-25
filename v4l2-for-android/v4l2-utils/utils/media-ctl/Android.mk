LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MULTILIB := 32
LOCAL_CFLAGS += -g
LOCAL_CPPFLAGS += -g
LOCAL_STRIP_MODULE :=false

LOCAL_MODULE := media-ctl
LOCAL_MODULE_TAGS := optional

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../.. \
    $(LOCAL_PATH)/../../include \
    bionic \
    external/stlport/stlport

LOCAL_SHARED_LIBRARIES := libstlport

LOCAL_SRC_FILES := media-ctl.c libmediactl.c libv4l2subdev.c options.c

include $(BUILD_EXECUTABLE)
