#ifndef __FULLDEMO_UILAYER_H__
#define __FULLDEMO_UILAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class FullDemoUILayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    void menuCallback_nearAttack(CCObject* pSender);
	void menuCallback_farAttack(CCObject* pSender);
	void menuCallback_lightingSkill(CCObject* pSender);

	void menuCallback_showMini(CCObject* pSender);

    void menuCallback_showPerformance(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(FullDemoUILayer);
};

#endif
