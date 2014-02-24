//
//  SceneNode.h
//  HelloWorld
//
//  Created by 晓龙 张 on 12-4-18.
//  Copyright 2012年 chukong-inc. All rights reserved.
//

#ifndef HelloWorld_SceneNode_h
#define HelloWorld_SceneNode_h
#include "cocos2d.h"
#if !defined(USE_GLES10)
#include "CCGLProgram.h"
#endif
#include "RippleModel.h"
#include "effects/CCGrabber.h"
#include "RippleNode.h"


namespace cocos2d
{
    
    class SceneNode : public CCLayer{
    public:
        SceneNode(unsigned int factor, bool isBulgeNode) ;
        ~SceneNode() ;
        bool initWithVertex(std::string vert, std::string frag) ;
        bool initWithTexture(std::string textureName) ;
        void showOptionButton(bool bIsShow);
        void setRippleStrength(bool repeat, float strength) ;
        void cleanUpTextures() ;
        void touchRippleByLocation(CCPoint location) ;
        void setupBuffers() ;
        void setupGL() ;
        void updateBulgeEffect() ;
        void updateBuffer() ;
        void draw() ;
        void endBuffers() ;
        void visit() ;
        bool initWithBulgeShader(std::string vert, std::string frag) ;
        virtual void listenBackToForeground(CCObject *sender);
        RippleNode*getBulgeNode(){return bulgeNode ;}
    private:
        bool        _bIsBulgeNode ;
        RippleNode    *bulgeNode ;
        GLuint      _positionVBO ;
        GLuint      _texcoordVBO ;
        GLuint      _indexVBO ;
        
        float     _screenWidth ;
        float     _screenHeight ;
        size_t      _textureWidth ;
        size_t      _textureHeight ;
        unsigned int _meshFactor ;
        
        RippleModel *_ripple ;
        CCTexture2D *_texture ;
        GLint _uniforms[NUM_UNIFORMS];
        
        bool        bIsShowButton;
//        CCTexture2D *_textureOption;
        CCTexture2D *_textureMail;
        
//        GLfloat     radius ;
//        GLfloat     strength ;
//        GLfloat     center ;
//        CCPoint     _touchLocation ;
//        bool   isExplode ;
    };
}
#endif
