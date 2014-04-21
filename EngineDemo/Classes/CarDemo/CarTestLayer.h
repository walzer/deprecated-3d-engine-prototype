#ifndef _CARTESTLAYER_H_ 
#define _CARTESTLAYER_H_

#include "CCNode.h"
//#include "touch_dispatcher/CCTouchDelegateProtocol.h"
#include "cocos2d.h"
#include "cocos3d.h"
#include "TestLayer.h"

USING_NS_CC;

class CarTestLayer : public TestLayer
{
public:
	CarTestLayer();
	virtual ~CarTestLayer();

	virtual bool init();   

	virtual void update(float dt);

	virtual void draw3D(void);//render the 3d contents

	CREATE_FUNC(CarTestLayer);


	void setUpScene();

	void setUpCar();

	// setup initialize camera
	void setUpCamera();
	//setup initialize light
	void setUpLight();

	virtual void touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex);


	void menuCallback(CCObject * pSender);

	CCLayer* createUILayer();

protected:
	bool _touched;
	int _touchX;
	int _touchY;

	cocos3d::C3DStaticObj* _sm;
	cocos3d::C3DSprite* _obj;

};

#endif