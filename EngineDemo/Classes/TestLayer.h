#ifndef __TEST_LAYER_H__
#define __TEST_LAYER_H__

#include "base_nodes/CCNode.h"
#include "cocos2d.h"
#include "cocos3d.h"

USING_NS_CC;

class  TestLayer :  public cocos3d::C3DLayer
{
public:
    virtual CCLayer* createUILayer() = 0;
	
	virtual void touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex) = 0;
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
	{	
		float scalex = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getScaleX();
		float scaley = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getScaleY();
		CCTouch *pTouch;
		CCSetIterator setIter;
		for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
		{
			pTouch = (CCTouch *)(*setIter);
			CCPoint touchPoint = pTouch->getLocationInView();

			touchPoint.x = touchPoint.x * scalex;
			touchPoint.y = touchPoint.y * scaley;
	
			//-----------------------------------------------------------------
			touchEvent(cocos3d::TouchEvent_PRESS, touchPoint.x, touchPoint.y, pTouch->getID());
		}
	}

	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
	{
		float scalex = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getScaleX();
		float scaley = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getScaleY();

		CCTouch *pTouch;
		CCSetIterator setIter;
		for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
		{
			pTouch = (CCTouch *)(*setIter);
			CCPoint touchPoint = pTouch->getLocationInView();

			touchPoint.x = touchPoint.x * scalex;
			touchPoint.y = touchPoint.y * scaley;

			touchEvent(cocos3d::TouchEvent_MOVE, touchPoint.x, touchPoint.y, pTouch->getID());
		}
	}

	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
	{
		float scalex = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getScaleX();
		float scaley = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getScaleY();

		CCTouch *pTouch;
		CCSetIterator setIter;
		for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
		{
			pTouch = (CCTouch *)(*setIter);
			CCPoint touchPoint = pTouch->getLocationInView();

			touchPoint.x = touchPoint.x * scalex;
			touchPoint.y = touchPoint.y * scaley;

			touchEvent(cocos3d::TouchEvent_RELEASE, touchPoint.x, touchPoint.y, pTouch->getID());
		}
	}

	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
	{
	}

};

#endif