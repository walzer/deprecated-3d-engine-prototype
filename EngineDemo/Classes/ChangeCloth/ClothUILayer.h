#ifndef __CLOTH_UILAYER_H__
#define __CLOTH_UILAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class ClothUILayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void menuCallback_changeCap(CCObject* pSender);
	void menuCallback_changeHair(CCObject* pSender);
	void menuCallback_changeGlasses(CCObject* pSender);
	void menuCallback_changeUpBody(CCObject* pSender);
	void menuCallback_changeBottomBody(CCObject* pSender);
	void menuCallback_changeShoot(CCObject* pSender);
	void menuCallback_changeNose(CCObject* pSender);
	void menuCallback_changeEye(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(ClothUILayer);
};

#endif
