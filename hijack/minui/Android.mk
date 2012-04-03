
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := graphics.c events.c resources.c

LOCAL_C_INCLUDES +=\
    external/libpng\
    external/zlib

LOCAL_MODULE := libminui_bm
LOCAL_MODULE_TAGS := eng debug

# Defy use this :
ifeq ($(TARGET_RECOVERY_PIXEL_FORMAT),"BGRA_8888")
    LOCAL_CFLAGS += -DPIXELS_BGRA
endif

ifeq ($(TARGET_RECOVERY_PIXEL_FORMAT),"RGBA_8888")
    LOCAL_CFLAGS += -DPIXELS_RGBA
endif

ifeq ($(TARGET_RECOVERY_PIXEL_FORMAT),"RGBX_8888")
    LOCAL_CFLAGS += -DPIXELS_RGBX
endif

# Reversed 16bits RGB (ics software gralloc)
#LOCAL_CFLAGS += -DPIXELS_BGR_16BPP

include $(BUILD_STATIC_LIBRARY)

