#ifndef __SHADOW_TEST_LAYER_H__
#define __SHADOW_TEST_LAYER_H__

#include "CCNode.h"
//#include "touch_dispatcher/CCTouchDelegateProtocol.h"
#include "cocos2d.h"
#include "cocos3d.h"
#include "TestLayer.h"

USING_NS_CC;

class ShadowTestLayer :  public TestLayer
{
public:
    ShadowTestLayer();
    virtual ~ShadowTestLayer();
	virtual bool init();

	virtual void update(float dt);

	virtual void draw3D(void);//render the 3d contents

	CREATE_FUNC(ShadowTestLayer);

	void setUpScene();

    // setup initialize camera
    void setUpCamera();
    //setup initialize light
    void setUpLight();

    virtual void touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex);
	

    void menuCallback(CCObject * pSender);

    CCLayer* createUILayer();
    void setUpShadowMap();
protected:
	bool _touched;
	int _touchX;
	int _touchY;
};
#endif 