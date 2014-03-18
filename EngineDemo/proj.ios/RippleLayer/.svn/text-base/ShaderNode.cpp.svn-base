//
//  ShaderNode.cpp
//  HelloWorld
//
//  Created by 张 晓龙 on 12-4-7.
//  Copyright (c) 2012年 chukong-inc. All rights reserved.
//

#include <iostream>
#include "ShaderNode.h"
using namespace cocos2d ;
ShaderNode::ShaderNode() 
{
    
}
ShaderNode::~ShaderNode() 
{
    
    ccGLDeleteProgram(uniformCenter) ;
    ccGLDeleteProgram(uniformTime) ;
    ccGLDeleteProgram(uniformResolution) ;
    shaderProgram_->release() ;
    if ( texture )
        texture->release() ;
}
ShaderNode* ShaderNode::shaderNodeWithVertex(std::string vert, std::string frag) 
{
    ShaderNode*node = new ShaderNode() ;
    node->initWithVertex(vert, frag) ;
    node->autorelease() ;

    return node ;
}
bool ShaderNode::initWithVertex(std::string vert, std::string frag) 
{
    loadShaderVertex(vert, frag) ;
    time_ = 0 ;
    ccVertex2F vertex2f;
    vertex2f.x = SIZE_X;
    vertex2f.y = SIZE_Y;
    resolution_ = vertex2f;
    scheduleUpdate() ;
    setContentSize(CCSizeMake(SIZE_X, SIZE_Y)) ;
    setAnchorPoint(ccp(0.5f, 0.5f)) ;
    texture = NULL ;
    return true ;
}
bool ShaderNode::loadTexture(std::string textureName) 
{
    texture = CCTextureCache::sharedTextureCache()->addImage(textureName.c_str());
    texture->retain() ;
    return true ;
}
void ShaderNode::loadShaderVertex(std::string vert, std::string frag) 
{
    CCGLProgram*shader = new CCGLProgram() ;
    shader->initWithVertexShaderFilename(vert.c_str(), frag.c_str()) ;
    shader->addAttribute("aVertex", kCCVertexAttrib_Position) ;
    shader->link() ;
    shader->updateUniforms() ;
    uniformCenter = glGetUniformLocation(shader->getProgram(), "center") ;
    uniformResolution = glGetUniformLocation(shader->getProgram(), "resolution") ;
    uniformTime = glGetUniformLocation(shader->getProgram(), "time") ;
    
    this->shaderProgram_ = shader ;
    //shader->release() ;
    
}
void ShaderNode::update(float dt) 
{
    time_ += dt ;
}


void ShaderNode::draw() 
{
    float w = SIZE_X, h = SIZE_Y;
	GLfloat vertices[12] = {0,0, w,0, w,h, 0,0, 0,h, w,h};
    
	ccGLUseProgram( shaderProgram_->getProgram() );
    
	//
	// Uniforms
	//
	
	shaderProgram_->setUniformForModelViewProjectionMatrix();
    
    if ( texture )
    {
        glActiveTexture (GL_TEXTURE0);
        GLuint tex = texture->getName() ;
        //glGenTextures(1, &tex) ;
        glBindTexture(GL_TEXTURE_2D, tex) ;
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
	glUniform2fv( uniformCenter, 1, (GLfloat*)&getPosition() );
	glUniform2fv( uniformResolution, 1, (GLfloat*)&resolution_ );
	glUniform1f( uniformTime, time_ );
	
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    
	glDrawArrays(GL_TRIANGLES, 0, 6);	

}