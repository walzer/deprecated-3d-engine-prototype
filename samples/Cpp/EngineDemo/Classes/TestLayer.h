#ifndef __TEST_LAYER_H__
#define __TEST_LAYER_H__

#include "base_nodes/CCNode.h"
#include "cocos2d.h"
#include "cocos3d.h"


USING_NS_CC;

class  TestLayer :  public cocos3d::C3DLayer
{  

public:
    virtual CCLayer* createUILayer() = 0;

	

};

#endif