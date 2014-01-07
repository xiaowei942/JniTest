LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS := -D__STDC_CONSTANT_MACROS -Wno-sign-compare -Wno-switch -DHAVE_NEON=1 \
                -mfpu=neon -mfloat-abi=softfp -fPIC -DANDROID

LOCAL_MODULE := jnitest

LOCAL_C_INCLUDES += \
                    $(LOCAL_PATH)/../ffmpeg \

LOCAL_SRC_FILES := \
                    Test.cpp

#LOCAL_STATIC_LIBRARIES := libavformat libavcodec libavdevice libavfilter libavutil libswscale libswresample

LOCAL_LDLIBS :=-L$(NDK_PLATFORMS_ROOT)/$(TARGET_PLATFORM)/arch-arm/usr/lib \
                -L$(LOCAL_PATH)/.. -llog -lz -ldl -lgcc

include $(BUILD_SHARED_LIBRARY)