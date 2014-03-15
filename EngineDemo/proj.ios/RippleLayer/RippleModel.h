//
//  RippleModel.h
//  HelloWorld
//
//  Created by 张 晓龙 on 12-4-7.
//  Copyright (c) 2012年 chukong-inc. All rights reserved.
//

#ifndef HelloWorld_RippleModel_h
#define HelloWorld_RippleModel_h

#include "cocos2d.h"
//#include "CCGLProgram.h"
class RippleModel : public cocos2d::CCObject{
    
public:
    RippleModel() ;
    virtual ~RippleModel() ;
    void initRippleMap() ;
    void initRippleCoeff() ;
    void initMesh() ;
    void freeBuffers() ;
    void resetMesh();
    bool initWithScreenWidth( unsigned int width, 
                              unsigned int height,
                              unsigned int factor, 
                              unsigned int radius,
                              unsigned int texWidth,
                              unsigned int texHeight) ;
    void runSimulation() ;
    void initiateRippleAtLocation(cocos2d::CCPoint location) ;
    void setRippleStrength(bool repeat, float strength) ;
    GLfloat* getVertices() ;
    GLfloat* getTexCoords() ;
    GLushort* getIndices() ;
    unsigned int getVertexSize() ;
    unsigned int getIndexSize() ;
    unsigned int getIndexCount() ;
private:
    unsigned int _screenWidth ;
    unsigned int _screenHeight ;
    unsigned int poolWidth ;
    unsigned int poolHeight ;
    unsigned int touchRadius ;
    unsigned int textureWidth ;
    unsigned int textureHeight ;
    unsigned int meshFactor ;
    
    float texCoordFactorS ;
    float texCoordOffsetS ;
    float texCoordFactorT ;
    float texCoordOffsetT ;
    
    float *rippleCoeff ;
    float *rippleSource ;
    float *rippleDest ;
    
    bool    isRepeat ;
    float   rippleStrength ;
    
    GLfloat  *rippleVertices ;
    GLfloat  *rippleTexCoords ;
    GLushort *rippleIndicies ;

	bool isNeedSimulation;
	float aspectRatio; // 宽高比 
};

#endif
