//
//  ShaderNode.h
//  HelloWorld
//
//  Created by 张 晓龙 on 12-4-7.
//  Copyright (c) 2012年 chukong-inc. All rights reserved.
//

#ifndef HelloWorld_ShaderNode_h
#define HelloWorld_ShaderNode_h
#include "cocos2d.h"
#include "CCGLProgram.h"

namespace cocos2d
{
enum 
{
    SIZE_X = 256,
    SIZE_Y = 256
} ;
class ShaderNode : public CCNode {
protected:
    ccVertex2F resolution_ ;
    float      time_ ;
    GLuint     uniformCenter ;
    GLuint     uniformResolution ;
    GLuint     uniformTime ;
    CCGLProgram	*shaderProgram_;
    CCTexture2D *texture ;
public:
    ShaderNode() ;
    virtual ~ShaderNode() ;
    static ShaderNode* shaderNodeWithVertex(std::string vert, std::string frag) ;
    bool loadTexture(std::string textureName) ;
    bool initWithVertex(std::string vert, std::string frag) ;
    void loadShaderVertex(std::string vert, std::string frag) ;
    void update(float dt) ;
    void draw() ;
};
}
#endif
