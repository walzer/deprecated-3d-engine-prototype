#ifndef __UI3DLAYER_H__
#define __UI3DLAYER_H__

#include "base_nodes/CCNode.h"
#include "touch_dispatcher/CCTouchDelegateProtocol.h"
#include "cocos2d.h"
//#include "cocos3d.h"
#include "TestLayer.h"


USING_NS_CC;
namespace cocos2d
{

class  UI3DLayer :  public cocos3d::C3DLayer
{  

public:
    UI3DLayer();
    virtual ~UI3DLayer();   
	bool init();   
		
	virtual void update(float dt);

	virtual void draw(void);//render the 3d contents
	
	static UI3DLayer* getInstance();  

	CREATE_FUNC(UI3DLayer);
		

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
	
	void setBase(cocos3d::C3DVector3& base);

public:
	    
	
private:	

	bool _touched;
	int _touchX;
	int _touchY;	

	cocos3d::C3DVector3 _base;
	
};

}
#endif 
