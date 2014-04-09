LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cocos3d_static

LOCAL_MODULE_FILENAME := libcocos3d

LOCAL_SRC_FILES :=\
AttachNode.cpp \
BonePart.cpp \
BundleData.cpp \
C3DAABB.cpp \
C3DActionListener.cpp \
C3DAnimatedLight.cpp \
C3DAnimation.cpp \
C3DAnimationChannel.cpp \
C3DAnimationClip.cpp \
C3DAnimationCurve.cpp \
C3DBaseMesh.cpp \
C3DBaseModel.cpp \
C3DBatchMesh.cpp \
C3DBatchModel.cpp \
C3DBone.cpp \
C3DCamera.cpp \
C3DCollitionBox.cpp \
C3DCone.cpp \
C3DCylinder.cpp \
C3DDepthStencilTarget.cpp \
C3DEffect.cpp \
C3DEffectManager.cpp \
C3DElementNode.cpp \
C3DForcePSA.cpp \
C3DFrameBuffer.cpp \
C3DFrustum.cpp \
C3DGeoWireRender.cpp \
C3DLayer.cpp \
C3DLight.cpp \
C3DLineRender.cpp \
C3DMaterial.cpp \
C3DMaterialManager.cpp \
C3DMathUtilty.cpp \
C3DMatrix.cpp \
C3DMesh.cpp \
C3DMeshSkin.cpp \
C3DModel.cpp \
C3DModelNode.cpp \
C3Dneon_matrix_impl.c \
C3DNode.cpp \
C3DNoise.cpp \
C3DOBB.cpp \
C3DParticleEmitter.cpp \
C3DParticleRender.cpp \
C3DParticleSystem.cpp \
C3DParticleSystemCommon.cpp \
C3DPass.cpp \
C3DPlane.cpp \
C3DPostEffect.cpp \
C3DPostProcess.cpp \
C3DProfile.cpp \
C3DQuaternion.cpp \
C3DRandom.cpp \
C3DRay.cpp \
C3DRenderBlock.cpp \
C3DRenderChannel.cpp \
C3DRenderNode.cpp \
C3DRenderState.cpp \
C3DRenderSystem.cpp \
C3DRenderTarget.cpp \
C3DResource.cpp \
C3DResourceLoader.cpp \
C3DResourceManager.cpp \
C3DResourcePool.cpp \
C3DSampler.cpp \
C3DSamplerCube.cpp \
C3DScene.cpp \
C3DShadowMap.cpp \
C3DSkeleton.cpp \
C3DSkinlessModel.cpp \
C3DSkinModel.cpp \
C3DSprite.cpp \
C3DRenderNodeManager.cpp \
C3DStat.cpp \
C3DStaticObj.cpp \
C3DStream.cpp \
C3DTechnique.cpp \
C3DTexture.cpp \
C3DTexture3D.cpp \
C3DTintPSA.cpp \
C3DTransform.cpp \
C3DTransformPSA.cpp \
C3DVector2.cpp \
C3DVector3.cpp \
C3DVector4.cpp \
C3DVertexDeclaration.cpp \
C3DVertexFormat.cpp \
C3DViewport.cpp \
FacialAnimation.cpp \
FacialAnimManager.cpp \
Geo.cpp \
MaterialParameter.cpp \
MeshPart.cpp \
C3DMorph.cpp \
C3DMorphMesh.cpp \
C3DMorphModel.cpp \
Rectangle.cpp \
StringTool.cpp \
C3DDeviceAdapter_android.cpp \


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
//LOCAL_WHOLE_STATIC_LIBRARIES += cpufeatures


LOCAL_C_INCLUDES := $(LOCAL_PATH) \
$(LOCAL_PATH)/physics \

                    
//LOCAL_CFLAGS := -D__ANDROID__ -fexceptions 
//LOCAL_LDLIBS := -lGLESv2  -lEGL -llog -lz -landroid
                
                    
include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/cpufeatures)

                           
