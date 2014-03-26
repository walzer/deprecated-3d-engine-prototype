#ifndef __FULLDEMOLAYER_H__
#define __FULLDEMOLAYER_H__

#include "CCNode.h"
//#include "touch_dispatcher/CCTouchDelegateProtocol.h"
#include "cocos2d.h"
//#include "cocos3d.h"
#include "TestLayer.h"

namespace cocos3d {
   class C3DAnimatedLight;
   class C3DPlane;
}

USING_NS_CC;
namespace cocos2d
{
class C3DActor;
class MainPlayer;
class Npc;
class Enemy;

class  FullDemoLayer :  public TestLayer
{
public:

public:
    FullDemoLayer();
    virtual ~FullDemoLayer();
	bool init();

	virtual void update(float dt);

	virtual void draw3D(void);//render the 3d contents

	static FullDemoLayer* getInstance();

	 CREATE_FUNC(FullDemoLayer);

	void createStaticModel();
	void createParticleEffect();
	void createEffect();

	void createMainPlayer();

	void createLive();
	void createEnemy();
	void createNpc();

	MainPlayer* getMainPlayer()
	{
		return _mainPlayer;
	}

	void createAnimLight();

	void setUpScene();

    // setup initialize camera
    void setUpCamera();
    //setup initialize light
    void setUpLight();

    void setUpShadowMap();

    void setUpProfile();

    void switchProfileShow();

    virtual void touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex);
	
	C3DActor* pick( float x,float y );

	std::list<C3DActor*> getActors(void) const
	{
		return _actors;
	}

public:

    CCLayer* createUILayer();

private:

	bool _touched;
	int _touchX;
	int _touchY;

	cocos3d::C3DPlane* _plane;
	MainPlayer* _mainPlayer;

	std::list<C3DActor*> _actors;
};
}
#endif
