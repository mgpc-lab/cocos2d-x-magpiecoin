LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ext_event
LOCAL_MODULE_FILENAME := event
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libevent.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)
