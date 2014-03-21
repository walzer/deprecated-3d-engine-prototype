#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

// Main layer for executing demos

namespace cocos3d
{
    class C3DLayer;
}

class MainLayer : public cocos2d::CCLayer
{
	friend class Npc;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

	static MainLayer* getMainLayer()
	{
		return _instance;
	}

    // implement the "static node()" method manually
    CREATE_FUNC(MainLayer);

    void menuCallback(CCObject * pSender);
    void menuBackCallback(CCObject * pSender);
    void closeCallback(CCObject * pSender);

	virtual void onTouchesBegan( const std::vector<Touch*>& touches, Event *unused_event );
	virtual void onTouchesMoved( const std::vector<Touch*>& touches, Event *unused_event );
	




public:
    CCPoint m_tBeginPos;
    CCMenu* m_pItemMenu;
    CCMenu* m_pMenuBack;
    cocos3d::C3DLayer* m_pLayer3D;
    CCLayer* m_pUILayer;
	static MainLayer* _instance;
};

#endif
