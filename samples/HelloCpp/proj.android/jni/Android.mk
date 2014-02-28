LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := helloCpp

LOCAL_MODULE_FILENAME := libHelloCpp
LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Hello3DLayer.cpp \
                   ../../Classes/HelloWorldScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_CFLAGS    := -D__ANDROID__\
                   -I"$(LOCAL_PATH)/../../../../cocos3d" \

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocos3d_static

LOCAL_LDLIBS :=  -landroid -lEGL -lGLESv2
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx) 
$(call import-module,cocos3d) 
