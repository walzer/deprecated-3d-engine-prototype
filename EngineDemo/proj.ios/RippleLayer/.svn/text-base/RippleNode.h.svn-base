//
//  RippleNode.h
//  HelloWorld
//
//  Created by 张 晓龙 on 12-4-7.
//  Copyright (c) 2012年 chukong-inc. All rights reserved.
//

#ifndef HelloWorld_RippleNode_h
#define HelloWorld_RippleNode_h
#include "cocos2d.h"
#if !defined(USE_GLES10)
#include "CCGLProgram.h"
#endif
#include "RippleModel.h"
//#include "CCGrabber.h"
enum
{
    RADIUS,
    STRENGTH,
    CENTER,
    UNIFORM_Y,
    TEXTSIZE,
    TEXCOORDVERYING,
    NUM_UNIFORMS
};

namespace cocos2d
{

    class RippleNode : public CCNode{
    public:
        RippleNode(unsigned int factor, CCSize size) ;
        ~RippleNode() ;
        bool initWithVertex(std::string vert, std::string frag) ;
        bool initWithTexture(std::string textureName) ;
        void touchRippleByLocation(CCPoint location) ;
        void setupBuffers() ;
        void setupGL() ;
        void updateBuffer() ;
        void touchRipple(CCTouch*touch, CCEvent*event) ;
        void endBuffers() ;
        void beforeDraw() ;
        void afterDraw() ;
        void calculateVertexPoints() ;
        void blit() ;
    private:
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
        GLint       _uniforms[NUM_UNIFORMS];
        CCTexture2D *RenderTexture ;
        CCGrabber   *grabber ;
        CCSize      textureSize ;
        GLfloat     radius ;
        GLfloat     strength ;
        GLfloat     center ;
        CCPoint     _touchLocation ;
        bool   isExplode ;
    };
}
#endif
