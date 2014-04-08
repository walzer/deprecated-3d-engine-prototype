LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := EngineDemo

LOCAL_MODULE_FILENAME := libEngineDemo
LOCAL_SRC_FILES := main.cpp \
                   ../../Classes/AppDelegate.cpp\
                   ../../Classes/C3DActor.cpp \
                   ../../Classes/C3DBaseActor.cpp \
                   ../../Classes/MainLayer.cpp \
                   ../../Classes/VisibleRect.cpp \
                   ../../Classes/AnimLightTest/AnimLightTestLayer.cpp \
                   ../../Classes/ChangeCloth/ChangeClothesTestLayer.cpp \
                   ../../Classes/ChangeCloth/ClothUILayer.cpp \
                   ../../Classes/ColorTest/ColorTestLayer.cpp \
                   ../../Classes/FullDemo/BaseBullet.cpp \
                   ../../Classes/FullDemo/Bullet.cpp \
                   ../../Classes/FullDemo/Lighting.cpp \
                   ../../Classes/FullDemo/Enemy.cpp \
                   ../../Classes/FullDemo/FullDemoLayer.cpp \
                   ../../Classes/FullDemo/FullDemoUILayer.cpp \
                   ../../Classes/FullDemo/MainPlayer.cpp \
                   ../../Classes/FullDemo/Npc.cpp \
                   ../../Classes/FullDemo/UI3DLayer.cpp \
                   ../../Classes/MaterialTest/MaterialTestLayer.cpp \
                   ../../Classes/ParticleTest/ParticleTestLayer.cpp \
                   ../../Classes/PostEffect/PEBloom.cpp \
                   ../../Classes/PostEffect/PEBlur.cpp \
                   ../../Classes/PostEffect/PEColor.cpp \
                   ../../Classes/PostEffect/PEGlow.cpp \
                   ../../Classes/PostEffect/PEOutLine.cpp \
                   ../../Classes/PostEffect/PEPointWarp.cpp \
                   ../../Classes/PostEffect/PESceneChange.cpp \
                   ../../Classes/PerformanceTest/PerformanceTestLayer.cpp \
                   ../../Classes/PostEffect/PESpaceWarp.cpp \
                   ../../Classes/PostEffect/PEVortex.cpp \
                   ../../Classes/PostEffect/PostEffectTestLayer.cpp \
                   ../../Classes/TunnelTest/TunnelTestLayer.cpp \
                   ../../Classes/ShadowTest/ShadowTestLayer.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_CFLAGS   := -D__ANDROID__\
                  -I"$(LOCAL_PATH)/../../../cocos3d"

LOCAL_CPPFLAGS := -std=c++11 -pthread -frtti -fexceptions

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocos3d_static

LOCAL_LDLIBS :=  -landroid -lEGL -lGLESv2
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d) 
$(call import-module,cocos3d)
$(call import-module,chipmunk)
