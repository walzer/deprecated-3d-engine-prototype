#ifndef __POSTEFFECT_TEST_LAYER_H__
#define __POSTEFFECT_TEST_LAYER_H__

#include "base_nodes/CCNode.h"
#include "touch_dispatcher/CCTouchDelegateProtocol.h"
#include "cocos2d.h"
#include "cocos3d.h"
#include "TestLayer.h"

USING_NS_CC;

namespace cocos3d
{
}

class  PostEffectTestLayer :  public TestLayer
{
public:
    PostEffectTestLayer();
    virtual ~PostEffectTestLayer();
	virtual bool init();
	virtual void onExit();

	virtual void update(float dt);

	virtual void draw(void);//render the 3d contents

	CREATE_FUNC(PostEffectTestLayer);

	void setUpScene();

    // setup initialize camera
    void setUpCamera();
    //setup initialize light
    void setUpLight();

	void setUpPostEffect();

    virtual void touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex);

    // optional
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

    void menuCallback(CCObject * pSender);

    CCLayer* createUILayer();

protected:
	void postEffectTouchPress(float x, float y);
protected:
	bool _touched;
	int _touchX;
	int _touchY;
};
#endif 