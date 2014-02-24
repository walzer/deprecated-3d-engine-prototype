//
//  SceneNode.cpp
//  HelloWorld
//
//  Created by 晓龙 张 on 12-4-18.
//  Copyright 2012年 chukong-inc. All rights reserved.
//

#include "SceneNode.h"
//#include "GameConfig.h"
#include <iostream>
#include "support/ccUtils.h"

using namespace cocos2d ;
using namespace std ;

SceneNode::SceneNode(unsigned int factor, bool isBulgeNode)
{
    bulgeNode = NULL;
    _positionVBO = 0;
    _texcoordVBO  = 0;
    _indexVBO  = 0;

    _textureWidth = 0;
    _textureHeight  = 0;
    memset(_uniforms, 0, sizeof(_uniforms));

    _bIsBulgeNode = isBulgeNode ;
    _meshFactor = factor ;
    _ripple = NULL ;
    _texture = NULL ;
//    center = 0 ;
//    isExplode = false ;
//    radius = 0 ;
//    strength = 0 ;
    
    _screenWidth = CCDirector::sharedDirector()->getWinSize().width ;
    _screenHeight = CCDirector::sharedDirector()->getWinSize().height ;
    
    // zhangxm: should listen the event that app come to foreground to reload shader
//    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
//                                                                             callfuncO_selector(SceneNode::listenBackToForeground),
//                                                                             EVNET_COME_TO_FOREGROUND,
//                                                                             NULL);
    
}
SceneNode::~SceneNode()
{
    _ripple->release() ;
    _texture->release() ;
    if (_bIsBulgeNode) {
        bulgeNode->release() ;
    }
    
    glDeleteBuffers(1, &_positionVBO) ;
    glDeleteBuffers(1, &_texcoordVBO) ;
    glDeleteBuffers(1, &_indexVBO) ;
    
//    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVNET_COME_TO_FOREGROUND);
}

void SceneNode::listenBackToForeground(CCObject *sender)
{
    /*
    // reset shader program
    CCGLProgram *_shaderProgram = getShaderProgram();
    _shaderProgram->reset();
    _shaderProgram->initWithVertexShaderFilename("Shader.fsh", "Shader.vsh") ;
    
    _shaderProgram->addAttribute("a_position", kCCVertexAttrib_Position) ;
    _shaderProgram->addAttribute("a_texCoord", kCCVertexAttrib_TexCoords) ;
    _shaderProgram->link() ;
    _shaderProgram->updateUniforms() ;
    _uniforms[UNIFORM_Y] = glGetUniformLocation(_shaderProgram->getProgram(), "SamplerY") ;
    
    setupGL();
     */
    setupBuffers();
}

bool SceneNode::initWithBulgeShader(std::string vert, std::string frag)
{
    if ( _bIsBulgeNode )
    {
        bulgeNode = new RippleNode(512, CCSizeMake(_textureWidth, _textureWidth)) ;
        bulgeNode->initWithVertex(vert, frag) ;
    }
    return true ;
}

bool SceneNode::initWithVertex(std::string vert, std::string frag)
{
    /*
    CCGLProgram *_shaderProgram = new CCGLProgram() ;
    _shaderProgram->initWithVertexShaderFilename(vert.c_str(), frag.c_str()) ;
    
    _shaderProgram->addAttribute("a_position", kCCVertexAttrib_Position) ;
    _shaderProgram->addAttribute("a_texCoord", kCCVertexAttrib_TexCoords) ;
    _shaderProgram->link() ;
    _shaderProgram->updateUniforms() ;
     
    _uniforms[UNIFORM_Y] = glGetUniformLocation(_shaderProgram->getProgram(), "SamplerY") ;
//    _uniforms[RADIUS] = glGetUniformLocation(_shaderProgram->getProgram(), "radius") ;
//    _uniforms[STRENGTH] = glGetUniformLocation(_shaderProgram->getProgram(), "strength") ;
//    _uniforms[CENTER] = glGetUniformLocation(_shaderProgram->getProgram(), "center") ;
//    _uniforms[TEXTSIZE] = glGetUniformLocation(_shaderProgram->getProgram(), "texSize") ;
    
    this->setShaderProgram(_shaderProgram) ;
    _shaderProgram->release() ;
     */
    
    // zhangxm
    // use shader offered by engine
#if !defined(USE_GLES10)
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
#endif

    return true ;
}

bool SceneNode::initWithTexture(std::string textureName) 
{
    //if ( !_texture )
    {
//        CCTexture2D* temptex;
        
        _texture = CCTextureCache::sharedTextureCache()->addImage(textureName.c_str()) ;
        _texture->retain() ;
        _textureWidth = (size_t)_texture->getContentSize().width ;
        _textureHeight = (size_t)_texture->getContentSize().height ;
//        _textureWidth = _screenWidth ;
//        _textureHeight = _screenHeight ;
    }
    if ( !_texture )
        return false ;
    //if ( !_ripple )
    {
        _ripple = new RippleModel() ;
        _ripple->initWithScreenWidth((unsigned)_screenWidth, 
                                     (unsigned)_screenHeight, 
                                     _meshFactor, 
                                     2,
                                     (unsigned)_screenWidth,
                                     (unsigned)_screenHeight
                                     ) ;
        
    }
    if ( !_ripple )
        return false ;
    //setupGL() ;
    setupBuffers() ;
    return true ;
}

void SceneNode::showOptionButton(bool bIsShow){
    if (bIsShow) {
//        _textureOption = CCTextureCache::sharedTextureCache()->addImage("button_shouping_02_1.png");
//        _textureOption->retain();
        
//        CCSprite *spriteOption = CCSprite::create(ImageNameLang("button_shouping_02_1.png"));
//        this->addChild(spriteOption);
//        spriteOption->setPosition(ccp(512, 386));
        
    }
    bIsShowButton = bIsShow;
}
void SceneNode::setRippleStrength(bool repeat, float strength) 
{
    _ripple->setRippleStrength(repeat, strength) ;
}
void SceneNode::cleanUpTextures() 
{
    CC_SAFE_RELEASE_NULL(_texture);
}


void SceneNode::setupGL() 
{
#if !defined(USE_GLES10)
    ccGLUseProgram( getShaderProgram()->getProgram() );
    
    glUniform1i(_uniforms[UNIFORM_Y], 0) ;
#endif
//    glUniform1f(_uniforms[RADIUS], radius) ;
//    glUniform1f(_uniforms[STRENGTH], 0.8f) ;
//    glUniform2f(_uniforms[CENTER], 0, 0) ;
//    glUniform2f(_uniforms[TEXTSIZE], _textureWidth, _textureWidth) ;
}

void SceneNode::touchRippleByLocation(CCPoint location)
{
    // Because SceneNode move up a little in MainMenuSceneBase::initBg(),
    // touch point move down a little here.
    location.y -= 100.f/*VisibleRect::bottom().y*/;

//    location = ccpSub(location, ccpSub(getPosition(), ccp(getPosition().x*getAnchorPoint().x, getPosition().y*getAnchorPoint().y))) ;
    if ( _bIsBulgeNode )
    {
        bulgeNode->touchRippleByLocation(location) ;
    }
    _ripple->initiateRippleAtLocation(location) ;
    
//    _touchLocation = ccp(location.x, _screenHeight - location.y) ; ;
//    strength = 1.0f ;
//    radius = 0 ;
//    isExplode = true ;
}
void SceneNode::setupBuffers()
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
    // zhangxm
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    CHECK_GL_ERROR_DEBUG();
}

void SceneNode::endBuffers()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}
void SceneNode::updateBulgeEffect()
{
//    int radiusMax = 150 ;
//    if ( isExplode )
//    {
//        if ( radius <= radiusMax )
//        {
//            radius += radiusMax/10 ;
//        }
//        else {
//            isExplode = false ;
//            
//        }
//    }
//    else {
//        if ( radius > 0 )
//            radius -= radiusMax ;
//    }
//    glUniform1f(_uniforms[STRENGTH], strength) ;
//    glUniform2f(_uniforms[CENTER], _touchLocation.x, _touchLocation.y) ;
//    glUniform1f(_uniforms[RADIUS], radius) ;
}
void SceneNode::updateBuffer()
{
#if defined(USE_GLES10)
    ccGLEnableVertexAttribs(kCCVertexAttrib_Position);
    ccGLEnableVertexAttribs(kCCVertexAttrib_TexCoords);
#else
    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
#endif    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _positionVBO);
   
#if defined(USE_GLES10)
    ccglVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);    
    ccglVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);  
#else
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#endif

    _ripple->runSimulation() ;
    
    glBindBuffer(GL_ARRAY_BUFFER, _texcoordVBO);
    glBufferData(GL_ARRAY_BUFFER, _ripple->getVertexSize(), _ripple->getTexCoords(), GL_DYNAMIC_DRAW) ;
#if defined(USE_GLES10)
    ccglVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#else
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
#endif

    //updateBulgeEffect() ;
}
void SceneNode::visit()
{
    
    if (!m_bIsVisible)
	{
		return;
	}
	kmGLPushMatrix();
    if ( _bIsBulgeNode )
    {
        bulgeNode->beforeDraw() ;
    }
    
	this->transform();
    
    CCNode* pNode = NULL;
    unsigned int i = 0;
    
	if(m_pChildren && m_pChildren->count() > 0)
	{
		sortAllChildren();
		// draw children zOrder < 0
        ccArray *arrayData = m_pChildren->data;
        for( ; i < arrayData->num; i++ )
        {
            pNode = (CCNode*) arrayData->arr[i];
            
			if ( pNode && pNode->getZOrder() < 0 )
			{
				pNode->visit();
			}
			else
			{
				break;
			}
		}
		// self draw
		this->draw();
        
		for( ; i < arrayData->num; i++ )
		{
			pNode = (CCNode*) arrayData->arr[i];
			if (pNode)
			{
				pNode->visit();
			}
		}		
    }
	else
	{
		this->draw();
	}
    
	// reset for next frame
	m_nOrderOfArrival = 0;
    if ( _bIsBulgeNode )
    {
        bulgeNode->afterDraw() ;
    }
	kmGLPopMatrix();
}
void SceneNode::draw()
{
//    glClearColor(0.1, 0, 0.2, 1) ;
    
    CC_NODE_DRAW_SETUP();
    
    // set texture
    if ( _texture )
    {
        glActiveTexture(GL_TEXTURE0) ;
        ccGLBindTexture2D(_texture->getName()) ;
                
        // npot doesn't support GL_MIRRORED_REPEAT, changed it to GL_CLAMP_TO_EDGE, commented by James Chen.
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    if (bIsShowButton) {
//        if (_textureOption) {
//            glActiveTexture(GL_TEXTURE0) ;
//            ccGLBindTexture2D(_textureOption->getName()) ;
//            
//            // npot doesn't support GL_MIRRORED_REPEAT, changed it to GL_CLAMP_TO_EDGE, commented by James Chen.
//            //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//            //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//            
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        }
    }

    if ( _ripple )
    {
        updateBuffer() ;
        CHECK_GL_ERROR_DEBUG();
        glDrawElements(GL_TRIANGLE_STRIP, _ripple->getIndexCount(), GL_UNSIGNED_SHORT, 0) ;
        CHECK_GL_ERROR_DEBUG();
    }
    //CC_INCREMENT_GL_DRAWS(1);
    endBuffers() ;    
}
