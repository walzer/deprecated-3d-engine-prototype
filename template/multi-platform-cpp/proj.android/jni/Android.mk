LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := HelloCocos3D

LOCAL_MODULE_FILENAME := libHelloCocos3D
LOCAL_SRC_FILES := src/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Hello3DLayer.cpp \
                   ../../Classes/HelloWorldScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_CFLAGS    := -D__ANDROID__\
                   -I"$(LOCAL_PATH)/../../../../cocos3d" \

LOCAL_CPPFLAGS := -std=c++11 -pthread -frtti -fexceptions

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocos3d_static

LOCAL_LDLIBS :=  -landroid -lEGL -lGLESv2
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d) 
$(call import-module,cocos3d) 
