LOCAL_PATH := $(call my-dir)/..
IRRLICHT_PROJECT_PATH := $(LOCAL_PATH)

##
# libuEngine.a
##
include $(CLEAR_VARS)
LOCAL_MODULE := uEngine
LOCAL_SRC_FILES := $(IRRLICHT_PROJECT_PATH)/../../source/Irrlicht/Android/obj/local/armeabi-v7a/lib$(LOCAL_MODULE).a
include $(PREBUILT_STATIC_LIBRARY)

##
# libHelloWorldMobile.so
##
include $(CLEAR_VARS)

LOCAL_MODULE := HelloWorldMobile

LOCAL_CFLAGS := -D_IRR_ANDROID_PLATFORM_ -pipe -fno-rtti -fstrict-aliasing

ifndef NDEBUG
LOCAL_CFLAGS += -g -D_DEBUG
else
LOCAL_CFLAGS += -fexpensive-optimizations -O3
endif

ifeq ($(TARGET_ARCH_ABI),x86)
LOCAL_CFLAGS += -fno-stack-protector
endif

LOCAL_C_INCLUDES := ../../include

LOCAL_SRC_FILES := main.cpp android_tools.cpp

LOCAL_LDLIBS := -lEGL -llog -lGLESv2 -lz -landroid

LOCAL_STATIC_LIBRARIES := uEngine android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

##
# libandroid_native_app_glue.a
##
$(call import-module,android/native_app_glue)

# copy Irrlicht data to assets

# $(shell mkdir $(IRRLICHT_PROJECT_PATH)/assets)
# $(shell mkdir $(IRRLICHT_PROJECT_PATH)/assets/media)
# $(shell mkdir -p $(IRRLICHT_PROJECT_PATH)/assets/media/Shaders)
# $(shell mkdir -p $(IRRLICHT_PROJECT_PATH)/src)
# $(shell cp $(IRRLICHT_PROJECT_PATH)/../media/Shaders/*.* $(IRRLICHT_PROJECT_PATH)/assets/media/Shaders/)
# $(shell cp $(IRRLICHT_PROJECT_PATH)/../media/irrlichtlogo3.png $(IRRLICHT_PROJECT_PATH)/assets/media/)
# $(shell cp $(IRRLICHT_PROJECT_PATH)/../media/dwarf.x $(IRRLICHT_PROJECT_PATH)/assets/media/)
# $(shell cp $(IRRLICHT_PROJECT_PATH)/../media/dwarf.jpg $(IRRLICHT_PROJECT_PATH)/assets/media/)
# $(shell cp $(IRRLICHT_PROJECT_PATH)/../media/axe.jpg $(IRRLICHT_PROJECT_PATH)/assets/media/)
# $(shell cp $(IRRLICHT_PROJECT_PATH)/../media/fonthaettenschweiler.bmp $(IRRLICHT_PROJECT_PATH)/assets/media/)
# $(shell cp $(IRRLICHT_PROJECT_PATH)/../media/bigfont.png $(IRRLICHT_PROJECT_PATH)/assets/media/)

