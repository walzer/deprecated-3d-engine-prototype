#ifndef __HELLO3DLAYER_H__
#define __HELLO3DLAYER_H__

#include "cocos2d.h"
#include "cocos3d.h"
#include "C3DLayer.h"

class Hello3DLayer : public cocos3d::C3DLayer
{
public:
    Hello3DLayer();
    virtual ~Hello3DLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    virtual void update(float dt);

    void setUpScene();

    // setup initialize camera
    void setUpCamera();
    //setup initialize light
    void setUpLight();

    virtual void touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex);

    // optional
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    
    // implement the "static node()" method manually
    CREATE_FUNC(Hello3DLayer);

protected:

    bool _touched;
    int _touchX;
    int _touchY;


};

#endif // __HELLO3DLAYER_H__
