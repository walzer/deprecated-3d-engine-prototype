//
//  RippleModel.cpp
//  HelloWorld
//
//  Created by 张 晓龙 on 12-4-7.
//  Copyright (c) 2012年 chukong-inc. All rights reserved.
//

#include <iostream>
#include "RippleModel.h"
//#include <dispatch/dispatch.h>
//#include <dispatch/base.h> 
//#include "GameConfig.h"

using namespace cocos2d ;
RippleModel::RippleModel() 
: isNeedSimulation(true)
, aspectRatio(1.0f)
{
    
}

RippleModel::~RippleModel() 
{
    freeBuffers() ;
}
void RippleModel::initRippleMap() 
{
    memset(rippleSource, 0, (poolWidth+2)*(poolHeight+2)*sizeof(float)) ;
    memset(rippleDest, 0, (poolWidth+2)*(poolHeight+2)*sizeof(float)) ;
}

void RippleModel::initRippleCoeff() 
{
    for (unsigned y=0; y<=2*touchRadius; y++)
    {
        for (unsigned x=0; x<=2*touchRadius; x++)
        {        
            float distance = sqrt(float((x-touchRadius)*(x-touchRadius)+(y-touchRadius)*(y-touchRadius)));
            
            if (distance <= touchRadius)
            {
                float factor = (distance/touchRadius);
                
                // goes from -512 -> 0
                rippleCoeff[y*(touchRadius*2+1)+x] = -(float)(cos(factor*M_PI)+1.f) * 256.f;
            }
            else 
            {
                rippleCoeff[y*(touchRadius*2+1)+x] = 0.f;   
            }
        }
    }
}
void RippleModel::resetMesh()
{
    for (unsigned i=0; i<poolHeight; i++)
    {
        for (unsigned j=0; j<poolWidth; j++)
        {
            rippleVertices[(i*poolWidth+j)*2+0] = (j*(1.f/(poolWidth-1)))*textureWidth;
            rippleVertices[(i*poolWidth+j)*2+1] = (i*(1.f/(poolHeight-1)))*textureHeight;
            
            rippleTexCoords[(i*poolWidth+j)*2+0] = (float)j/(poolWidth-1);
            rippleTexCoords[(i*poolWidth+j)*2+1] = ((float)i/(poolHeight-1));
            
//            CCLOG("vertices[%d] = %f", (i*poolWidth+j)*2+0, (j*(1.f/(poolWidth-1)))*textureWidth) ;
//            CCLOG("vertices[%d] = %f", (i*poolWidth+j)*2+1, (i*(1.f/(poolHeight-1)))*textureHeight) ;
//            
//            CCLOG("rippleTexCoords[%d] = %f", (i*poolWidth+j)*2+0, (float)j/(poolWidth-1)) ;
//            CCLOG("rippleTexCoords[%d] = %f", (i*poolWidth+j)*2+1, ((float)i/(poolHeight-1))) ;
        }
    }
    
    unsigned int index = 0;
    for (unsigned i=0; i<poolHeight-1; i++)
    {
        for (unsigned j=0; j<poolWidth; j++)
        {
            if (i%2 == 0)
            {
                // emit extra index to create degenerate triangle
                if (j == 0)
                {
                    rippleIndicies[index] = i*poolWidth+j;
                    index++;                    
                }
                
                rippleIndicies[index] = i*poolWidth+j;
                index++;
                rippleIndicies[index] = (i+1)*poolWidth+j;
                index++;
                
                // emit extra index to create degenerate triangle
                if (j == (poolWidth-1))
                {
                    rippleIndicies[index] = (i+1)*poolWidth+j;
                    index++;                    
                }
            }
            else
            {
                // emit extra index to create degenerate triangle
                if (j == 0)
                {
                    rippleIndicies[index] = (i+1)*poolWidth+j;
                    index++;
                }
                
                rippleIndicies[index] = (i+1)*poolWidth+j;
                index++;
                rippleIndicies[index] = i*poolWidth+j;
                index++;
                
                // emit extra index to create degenerate triangle
                if (j == (poolWidth-1))
                {
                    rippleIndicies[index] = i*poolWidth+j;
                    index++;
                }
            }
        }
    }
}
void RippleModel::initMesh() 
{
    for (unsigned i=0; i<poolHeight; i++)
    {
        for (unsigned j=0; j<poolWidth; j++)
        {
            rippleVertices[(i*poolWidth+j)*2+0] = (j*(1.f/(poolWidth-1)))*textureWidth;
            rippleVertices[(i*poolWidth+j)*2+1] = (i*(1.f/(poolHeight-1)))*textureHeight;

            rippleTexCoords[(i*poolWidth+j)*2+0] = (float)i/(poolWidth-1) * texCoordFactorS + texCoordOffsetS;
            rippleTexCoords[(i*poolWidth+j)*2+1] = ((float)j/(poolHeight-1)) * texCoordFactorT + texCoordFactorT;
        }
    }
    
    unsigned int index = 0;
    for (unsigned i=0; i<poolHeight-1; i++)
    {
        for (unsigned j=0; j<poolWidth; j++)
        {
            if (i%2 == 0)
            {
                // emit extra index to create degenerate triangle
                if (j == 0)
                {
                    rippleIndicies[index] = i*poolWidth+j;
                    index++;                    
                }
                
                rippleIndicies[index] = i*poolWidth+j;
                index++;
                rippleIndicies[index] = (i+1)*poolWidth+j;
                index++;
                
                // emit extra index to create degenerate triangle
                if (j == (poolWidth-1))
                {
                    rippleIndicies[index] = (i+1)*poolWidth+j;
                    index++;                    
                }
            }
            else
            {
                // emit extra index to create degenerate triangle
                if (j == 0)
                {
                    rippleIndicies[index] = (i+1)*poolWidth+j;
                    index++;
                }
                
                rippleIndicies[index] = (i+1)*poolWidth+j;
                index++;
                rippleIndicies[index] = i*poolWidth+j;
                index++;
                
                // emit extra index to create degenerate triangle
                if (j == (poolWidth-1))
                {
                    rippleIndicies[index] = i*poolWidth+j;
                    index++;
                }
            }
        }
    }

}

void RippleModel::freeBuffers() 
{
    free(rippleCoeff);
    
    free(rippleSource);
    free(rippleDest);
    
    free(rippleVertices);
    free(rippleTexCoords);
    free(rippleIndicies);  
}

bool RippleModel::initWithScreenWidth( unsigned int width, 
                         unsigned int height,
                         unsigned int factor, 
                         unsigned int radius,
                         unsigned int texWidth,
                         unsigned int texHeight) 
{
    _screenWidth = width;
    _screenHeight = height;
    aspectRatio = ((float)_screenWidth / _screenHeight);
    meshFactor = factor;
    poolWidth = texWidth/meshFactor;
    poolHeight = texHeight/meshFactor;
    touchRadius = radius;
    textureWidth = texWidth ;
    textureHeight = texHeight ;
    if ((float)_screenHeight/_screenWidth < (float)texWidth/texHeight)
    {            
        texCoordFactorS = (float)textureHeight/textureWidth;            
        texCoordOffsetS = 0.f;
        
        texCoordFactorT = 1.f;
        texCoordOffsetT = 0.f;
    }
    else
    {
        texCoordFactorS = 1.f;
        texCoordOffsetS = 0.f;
        
        texCoordFactorT = (float)textureWidth/textureHeight;
        texCoordOffsetT = 0.f;
    }
    
    rippleCoeff = (float *)malloc((touchRadius*2+1)*(touchRadius*2+1)*sizeof(float));
    memset(rippleCoeff, 0, (touchRadius*2+1)*(touchRadius*2+1)*sizeof(float));//cjh
    // +2 for padding the border
    rippleSource = (float *)malloc((poolWidth+2)*(poolHeight+2)*sizeof(float));
    memset(rippleSource, 0, (poolWidth+2)*(poolHeight+2)*sizeof(float));//cjh

    rippleDest = (float *)malloc((poolWidth+2)*(poolHeight+2)*sizeof(float));
    memset(rippleDest, 0, (poolWidth+2)*(poolHeight+2)*sizeof(float));//cjh

    rippleVertices = (GLfloat *)malloc(poolWidth*poolHeight*2*sizeof(GLfloat));
    memset(rippleVertices, 0, poolWidth*poolHeight*2*sizeof(GLfloat));//cjh

    rippleTexCoords = (GLfloat *)malloc(poolWidth*poolHeight*2*sizeof(GLfloat));
    memset(rippleTexCoords, 0, poolWidth*poolHeight*2*sizeof(GLfloat));//cjh

    rippleIndicies = (GLushort *)malloc((poolHeight-1)*(poolWidth*2+2)*sizeof(GLushort));
    memset(rippleIndicies, 0, (poolHeight-1)*(poolWidth*2+2)*sizeof(GLushort));//cjh

    if (!rippleCoeff || !rippleSource || !rippleDest || 
        !rippleVertices || !rippleTexCoords || !rippleIndicies)
    {
        freeBuffers();
        return false;
    }
    
    initRippleMap();
    
    initRippleCoeff();
    
    initMesh();
    
    return true ;
}
void RippleModel::setRippleStrength(bool repeat, float strength) 
{
    isRepeat = repeat ;
    rippleStrength = strength ;
}
void RippleModel::runSimulation() 
{
//cjh    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);

    // first pass for simulation buffers...
    if (! isNeedSimulation)
        return;

    isNeedSimulation = false;

    unsigned x = 0, y = 0;
    float resultModify = (isRepeat) ? 0.5f : (0.5f - 0.5f / rippleStrength);
    for (y = 0; y < poolHeight; y++)
    {
        int y0 = y*poolWidth;
        int y1 = (y)*(poolWidth+2);
        int y2 = (y+2)*(poolWidth+2);
        int y3 = (y+1)*(poolWidth+2);
        for (x=0; x<poolWidth; x++)
        {
            // * - denotes current pixel
            //
            //       a 
            //     c * d
            //       b 
            // +1 to both x/y values because the border is padded
            float a = rippleSource[y1 + x+1];
            float b = rippleSource[y2 + x+1];
            float c = rippleSource[y3 + x];
            float d = rippleSource[y3 + x+2];
            /* optimize this as follow
            float result = (a + b + c + d)/2.f - rippleDest[y3 + x+1];
            if ( !isRepeat )
            {
                result -= result/rippleStrength;
            }*/
            float result = (a + b + c + d - 2*rippleDest[y3 + x+1])*resultModify;

            if (rippleDest[y3 + x+1] != result)
            {
                rippleDest[y3 + x+1] = result;
                isNeedSimulation = true;
            }
        }
    }

    // second pass for modifying texture coord
    // For one line, the y's tex-coord and vector's y coord is constant,
    // malloc a buffer to store one line's y values.
    float *s_tcBuff = (float *)malloc(poolWidth*sizeof(float));
    memset(s_tcBuff, 0, poolWidth*sizeof(float));
    int fillCount = 0;
#if defined(USE_GLES10)
    // Because gles will auto align texture's size to 2^n, 
    // the tex-coords need do a scale to suit the feature.
    struct AdjustToTexRegion
    {
        void operator () (float& x, float& y)
        {
            // Define scale factor here. 
            RESO_DEF(float, xScaleFactor){480.0f / 512.0f, 0.0f, 960.0f / 1024.0f, 0.0f, 1280.0f / 2048.0f};
            RESO_DEF(float, yScaleFactor){320.0f / 512.0f,	0.0f, 640.0f / 1024.0f,	0.0f, 800.0f / 1024.0f};
            static float xScale = RESO_GET(xScaleFactor);
            static float yScale = RESO_GET(yScaleFactor);

            x = (x <= 0) ? 0 : (x >= 1.0f) ? (2.0f - x) * xScale : x * xScale;
            y = (y <= 0) ? 0 : (y >= 1.0f) ? (2.0f - y) * yScale : y * yScale;
        }
    };
    static AdjustToTexRegion adjustToTexRegion;

#endif
    static const float kOneDiv1048 = 1.0f / 1048.0f;
	// 第一次循环需要缓存数据 
    for (y = 0; y < 1; y++)
    {
        float t_tc = ((float)y/(poolWidth-1)) * texCoordFactorT + texCoordOffsetT;
        int y0     = y*poolWidth;
        int y1     = (y)*(poolWidth+2);
        int y2     = (y+2)*(poolWidth+2);
        int y3     = (y+1)*(poolWidth+2);

        for (x=0; x<poolWidth; x++)
        {
            // * - denotes current pixel
            //
            //       a
            //     c * d
            //       b
       
            // +1 to both x/y values because the border is padded
            float a = rippleDest[ y1 + x+1];
            float b = rippleDest[ y2 + x+1];
            float c = rippleDest[ y3 + x];
            float d = rippleDest[ y3 + x+2];
       
            float s_offset = ((b - a) * kOneDiv1048);
            float t_offset = ((c - d) * kOneDiv1048);
       
            // clamp
            s_offset = (s_offset < -0.5f) ? -0.5f : s_offset;
            t_offset = (t_offset < -0.5f) ? -0.5f : t_offset;
            s_offset = (s_offset > 0.5f) ? 0.5f : s_offset;
            t_offset = (t_offset > 0.5f) ? 0.5f : t_offset;
			// 缓存数据 
            float s_tc = (float)x/(poolHeight-1) * texCoordFactorS + texCoordOffsetS;
            s_tcBuff[x] = s_tc;
       
            rippleTexCoords[(y0 + x)*2+0] = (s_tc + s_offset);
            rippleTexCoords[(y0 + x)*2+1] = texCoordFactorS-(t_tc + t_offset);
       

            //rippleTexCoords[(y*poolWidth+x)*2+0] *= 0.91796875/*470.f/512.f*/;
            rippleTexCoords[(y0 + x)*2+1] *= aspectRatio;

#if defined(USE_GLES10)
            adjustToTexRegion(rippleTexCoords[(y0+x)*2+0], rippleTexCoords[(y0+x)*2+1]);
#endif
        }
    }
	// 第二次到最后一次循环直接使用缓存的数据 
    for (y = 1; y < poolHeight; y++)
    {
        float t_tc = ((float)y/(poolWidth-1)) * texCoordFactorT + texCoordOffsetT;
        int y0     = y*poolWidth;
        int y1     = (y)*(poolWidth+2);
        int y2     = (y+2)*(poolWidth+2);
        int y3     = (y+1)*(poolWidth+2);

        for (x=0; x<poolWidth; x++)
        {
            // * - denotes current pixel
            //
            //       a
            //     c * d
            //       b
       
            // +1 to both x/y values because the border is padded
            float a = rippleDest[ y1 + x+1];
            float b = rippleDest[ y2 + x+1];
            float c = rippleDest[ y3 + x];
            float d = rippleDest[ y3 + x+2];
       
            float s_offset = ((b - a) * kOneDiv1048);
            float t_offset = ((c - d) * kOneDiv1048);
       
            // clamp
            s_offset = (s_offset < -0.5f) ? -0.5f : s_offset;
            t_offset = (t_offset < -0.5f) ? -0.5f : t_offset;
            s_offset = (s_offset > 0.5f) ? 0.5f : s_offset;
            t_offset = (t_offset > 0.5f) ? 0.5f : t_offset;
			// 使用缓存 
            float s_tc = s_tcBuff[x];
       
            rippleTexCoords[(y0 + x)*2+0] = (s_tc + s_offset);
            rippleTexCoords[(y0 + x)*2+1] = texCoordFactorS-(t_tc + t_offset);
       

            //rippleTexCoords[(y*poolWidth+x)*2+0] *= 0.91796875/*470.f/512.f*/;
            rippleTexCoords[(y0 + x)*2+1] *= aspectRatio;

#if defined(USE_GLES10)
            adjustToTexRegion(rippleTexCoords[(y0+x)*2+0], rippleTexCoords[(y0+x)*2+1]);
#endif
        }
    }
    free(s_tcBuff);
    float *pTmp = rippleDest;
    rippleDest = rippleSource;
    rippleSource = pTmp;   
}

void RippleModel::initiateRippleAtLocation(cocos2d::CCPoint location) 
{
    isNeedSimulation = true;

    unsigned int xIndex = (unsigned int)((location.x /textureWidth) * poolWidth) ;
    unsigned int yIndex = (unsigned int)((location.y /textureHeight) * poolHeight) ;
    for (int y=(int)yIndex-(int)touchRadius; y<=(int)yIndex+(int)touchRadius; y++)
    {
        for (int x=(int)xIndex-(int)touchRadius; x<=(int)xIndex+(int)touchRadius; x++)
        {        
            if (x>=0 && x<(int)poolWidth &&
                y>=0 && y<(int)poolHeight)
            {
                // +1 to both x/y values because the border is padded
                rippleSource[(poolWidth+2)*(y+1)+x+1] += rippleCoeff[(y-(yIndex-touchRadius))*(touchRadius*2+1)+x-(xIndex-touchRadius)];   
            }
        }
    } 
}

GLfloat* RippleModel::getVertices() 
{
    return rippleVertices;
}

GLfloat* RippleModel::getTexCoords() 
{
    return rippleTexCoords;
}

GLushort* RippleModel::getIndices() 
{
    return rippleIndicies;
}

unsigned int RippleModel::getVertexSize() 
{
    return poolWidth*poolHeight*2*sizeof(GLfloat);
}

unsigned int RippleModel::getIndexSize() 
{
    return (poolHeight-1)*(poolWidth*2+2)*sizeof(GLushort);
}

unsigned int RippleModel::getIndexCount() 
{
    return getIndexSize()/sizeof(*rippleIndicies);
}
