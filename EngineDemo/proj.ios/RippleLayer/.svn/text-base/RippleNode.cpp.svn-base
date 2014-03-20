//
//  RippleNode.cpp
//  HelloWorld
//
//  Created by 张 晓龙 on 12-4-7.
//  Copyright (c) 2012年 chukong-inc. All rights reserved.
//

#include <iostream>
#include "RippleNode.h"
#include "support/ccUtils.h"
using namespace cocos2d ;
using namespace std ;
RippleNode::RippleNode(unsigned int factor, CCSize size)
{
    textureSize = size ;
    _meshFactor = factor ;
    radius = 0 ;
    isExplode = false ;
    _ripple = NULL ;
    _texture = NULL ;
    _screenWidth = CCDirector::sharedDirector()->getWinSize().width ;
    _screenHeight = CCDirector::sharedDirector()->getWinSize().height ;
    
    
    CCSize s = CCDirector::sharedDirector()->getWinSizeInPixels() ;
    unsigned long POTWide = ccNextPOT((unsigned long)s.width);
	unsigned long POTHigh = ccNextPOT((unsigned long)s.height);
    CCTexture2DPixelFormat format = kCCTexture2DPixelFormat_RGBA8888 ;
    //setContentSize(CCSizeMake(_screenWidth, _screenHeight)) ;
//    int bpp = 4 ;
//    void *data = calloc((size_t)(POTWide*POTHigh*bpp), 1) ;
//    if ( !data )
//    {
//        CCLOG("not enough memory!") ;
//        this->release() ;
//        return ;
//    }
    RenderTexture = new CCTexture2D() ;
    RenderTexture->initWithData(NULL, format, POTWide, POTHigh, s) ;
//    free(data) ;
    
    grabber = new CCGrabber() ;
    grabber->grab(RenderTexture) ;
}

RippleNode::~RippleNode()
{
    glDeleteBuffers(1, &_positionVBO) ;
    glDeleteBuffers(1, &_texcoordVBO) ;
    glDeleteBuffers(1, &_indexVBO) ;
    grabber->release() ;
    RenderTexture->release() ;

}

bool RippleNode::initWithVertex(std::string vert, std::string frag)
{
#if !defined(USE_GLES10)
    CCGLProgram *_shaderProgram = new CCGLProgram() ;
    _shaderProgram->initWithVertexShaderFilename(vert.c_str(), frag.c_str()) ;
    
    _shaderProgram->addAttribute("a_position", kCCVertexAttrib_Position) ;
    _shaderProgram->addAttribute("a_texCoord", kCCVertexAttrib_TexCoords) ;
    _shaderProgram->link() ;
    _shaderProgram->updateUniforms() ;
    _uniforms[UNIFORM_Y] = glGetUniformLocation(_shaderProgram->getProgram(), "SamplerY") ;
    _uniforms[RADIUS] = glGetUniformLocation(_shaderProgram->getProgram(), "radius") ;
    _uniforms[STRENGTH] = glGetUniformLocation(_shaderProgram->getProgram(), "strength") ;
    _uniforms[CENTER] = glGetUniformLocation(_shaderProgram->getProgram(), "center") ;
    _uniforms[TEXTSIZE] = glGetUniformLocation(_shaderProgram->getProgram(), "texSize") ;
    
    this->setShaderProgram(_shaderProgram) ;
    _shaderProgram->release() ;
#endif
    setupGL() ;
    
    if ( !_ripple )
    {
        _ripple = new RippleModel() ;
        _ripple->initWithScreenWidth((unsigned)_screenWidth, 
                                     (unsigned)_screenHeight, 
                                     _meshFactor,
                                     4,
                                     (unsigned)textureSize.width,
                                     (unsigned)textureSize.height) ;
        _ripple->resetMesh() ;
        _ripple->setRippleStrength(false, 8) ;
    }
    
    setupBuffers() ;
    return true ;
}


void RippleNode::setupGL() 
{
#if !defined(USE_GLES10)
    ccGLUseProgram( getShaderProgram()->getProgram() );
    
    glUniform1i(_uniforms[UNIFORM_Y], 0) ;
    glUniform1f(_uniforms[RADIUS], radius) ;
    glUniform1f(_uniforms[STRENGTH], 0.8f) ;
    glUniform2f(_uniforms[CENTER], 0, 0) ;
    glUniform2f(_uniforms[TEXTSIZE], textureSize.width, textureSize.height) ;
#endif
}

void RippleNode::touchRipple(CCTouch*touch, CCEvent*event) 
{
    CCPoint location = touch->locationInView() ;
    location = CCDirector::sharedDirector()->convertToGL(location) ;
    //location = ccp(_screenWidth - location.x, location.y) ;
    location = ccpSub(location, ccpSub(getPosition(), ccp(getPosition().x*getAnchorPoint().x, getPosition().y*getAnchorPoint().y))) ;
    //_ripple->initiateRippleAtLocation(location) ;
    strength = 1.2f ;
    radius = 0 ;
    isExplode = true ;
}

void RippleNode::touchRippleByLocation(CCPoint location)
{
    location = ccpSub(location, ccpSub(getPosition(), ccp(getPosition().x*getAnchorPoint().x, getPosition().y*getAnchorPoint().y))) ;
    //_ripple->initiateRippleAtLocation(location) ;
    
    //location = ccp(location.x, CCDirector::sharedDirector()->getWinSize().height - location.y+256 );
    _touchLocation = location ;
    strength = 0.5f ;
    radius = 0 ;
    isExplode = true ;
}
void RippleNode::setupBuffers()
{
    glGenBuffers(1, &_indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _ripple->getIndexSize(), _ripple->getIndices(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &_positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _positionVBO);
    glBufferData(GL_ARRAY_BUFFER, _ripple->getVertexSize(), _ripple->getVertices(), GL_STATIC_DRAW);

#if defined(USE_GLES10)
    ccGLEnableVertexAttribs(kCCVertexAttrib_Position);
    ccglVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#else
    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#endif
    
    glGenBuffers(1, &_texcoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _texcoordVBO);
    glBufferData(GL_ARRAY_BUFFER, _ripple->getVertexSize(), _ripple->getTexCoords(), GL_DYNAMIC_DRAW);
#if defined(USE_GLES10)
    ccGLEnableVertexAttribs(kCCVertexAttrib_TexCoords);
    ccglVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#else   
    glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#endif
    
    CHECK_GL_ERROR_DEBUG();
}

void RippleNode::endBuffers()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void RippleNode::updateBuffer()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _positionVBO);

#if defined(USE_GLES10)
    ccglVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#else
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, _texcoordVBO);

#if defined(USE_GLES10)
    ccglVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#else
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#endif

    //_ripple->runSimulation() ;
    
    glBindBuffer(GL_ARRAY_BUFFER, _texcoordVBO);
    glBufferData(GL_ARRAY_BUFFER, _ripple->getVertexSize(), _ripple->getTexCoords(), GL_DYNAMIC_DRAW) ;
    if ( isExplode )
    {
//        if ( strength >= 0.f )
//        {
//            strength -= 0.1f ;
//        }
        if ( radius <= 100 )
        {
            radius += 20 ;
        }
        else {
            isExplode = false ;
            
        }
    }
    else {
        if ( radius > 0 )
            radius -= 15 ;
    }

#if !defined(USE_GLES10)
    glUniform1f(_uniforms[STRENGTH], strength) ;
    glUniform2f(_uniforms[CENTER], _touchLocation.x, _touchLocation.y) ;
    glUniform1f(_uniforms[RADIUS], radius) ;
#endif
}

void RippleNode::beforeDraw()
{
    grabber->beforeRender(RenderTexture) ;
}
void RippleNode::afterDraw()
{
    grabber->afterRender(RenderTexture) ;
    ccGLBindTexture2D(RenderTexture->getName()) ;
    blit() ;
}
void RippleNode::blit()
{
    //glClearColor(0, 0, 0.0, 1) ;
    
    CC_NODE_DRAW_SETUP();
    
    if ( _ripple )
    {
        updateBuffer() ;
        glDrawElements(GL_TRIANGLE_STRIP, _ripple->getIndexCount(), GL_UNSIGNED_SHORT, 0) ;
    }
    //CC_INCREMENT_GL_DRAWS(1);
    endBuffers() ;
}
