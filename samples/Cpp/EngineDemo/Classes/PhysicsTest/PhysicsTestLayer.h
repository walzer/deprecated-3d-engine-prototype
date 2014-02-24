#ifndef __PHYSICS_TEST_LAYER_H__
#define __PHYSICS_TEST_LAYER_H__

#include "base_nodes/CCNode.h"
#include "touch_dispatcher/CCTouchDelegateProtocol.h"
#include "cocos2d.h"
#include "cocos3d.h"
#include "TestLayer.h"

#ifdef WIN32

#include <Physics.h>


USING_NS_CC;

class PhysicsTestLayer :  public TestLayer
{  

public:
    PhysicsTestLayer();
    virtual ~PhysicsTestLayer();   
	virtual bool init();   
		
	virtual void update(float dt);

	virtual void draw(void);//render the 3d contents

	CREATE_FUNC(PhysicsTestLayer);

	void setUpScene();

    // setup initialize camera
    void setUpCamera();
    //setup initialize light
    void setUpLight();

    virtual void touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex);

    // optional
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

    void menuCallback(CCObject * pSender);

    CCLayer* createUILayer();

	void shotPhysicObj();
protected:
	bool _touched;
	int _touchX;
	int _touchY;

	cocos3d::PhysicScene* _physicScene;
};

#endif //def win32

#endif 