#ifndef __PARTICLE_TEST_LAYER_H__
#define __PARTICLE_TEST_LAYER_H__

#include "CCNode.h"
//#include "touch_dispatcher/CCTouchDelegateProtocol.h"
#include "cocos2d.h"
#include "cocos3d.h"
#include "TestLayer.h"

USING_NS_CC;

namespace cocos3d {
    class C3DParticleSystem;
	class C3DLineRender;
}

class AnimListenerObject : public cocos3d::ListenerObject
{
public:
    AnimListenerObject(cocos3d::C3DParticleSystem* particle);
    void onFlowerStart();
    void onFlowerEnd();

protected:
    cocos3d::C3DParticleSystem* _particle;
};

class ParticleTestLayer :  public TestLayer
{
public:
    ParticleTestLayer();
    virtual ~ParticleTestLayer();
	virtual bool init();

	virtual void update(float dt);

	virtual void draw3D(void);//render the 3d contents

	CREATE_FUNC(ParticleTestLayer);

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

	float _totalTime;
	float _lightingTime;

    AnimListenerObject* _listenerObj;
	cocos3d::C3DLineRender* _lighting;
};
#endif 