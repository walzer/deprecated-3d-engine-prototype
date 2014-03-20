#ifndef __TEST_LAYER_H__
#define __TEST_LAYER_H__

#include "CCNode.h"
#include "cocos2d.h"
#include "cocos3d.h"

USING_NS_CC;


class  TestLayer :  public cocos3d::C3DLayer
{
public:
	virtual CCLayer* createUILayer() = 0;

	virtual void touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex) = 0;


	virtual void onTouchesBegan( const std::vector<Touch*>& touches, Event *unused_event )
	{
		float scalex = cocos2d::CCDirector::getInstance()->getOpenGLView()->getScaleX();
		float scaley = cocos2d::CCDirector::getInstance()->getOpenGLView()->getScaleY();

		Touch *pTouch;
		std::vector<Touch*>::const_iterator iter;
		for (iter = touches.begin(); iter != touches.end(); ++iter)
		{
			pTouch = (Touch *)(*iter);		
			Point touchPoint = pTouch->getLocationInView();

			touchPoint.x *= scalex;
			touchPoint.y *= scaley;

			touchEvent(cocos3d::TouchEvent_PRESS, touchPoint.x , touchPoint.y , pTouch->getID());
		}   

	}

	virtual void onTouchesMoved( const std::vector<Touch*>& touches, Event *unused_event )
	{
		float scalex = cocos2d::CCDirector::getInstance()->getOpenGLView()->getScaleX();
		float scaley = cocos2d::CCDirector::getInstance()->getOpenGLView()->getScaleY();

		Touch *pTouch;    
		std::vector<Touch*>::const_iterator iter;
		for (iter = touches.begin(); iter != touches.end(); ++iter)
		{
			pTouch = (Touch *)(*iter);	
			Point touchPoint = pTouch->getLocationInView();

			touchPoint.x *= scalex;
			touchPoint.y *= scaley;

			touchEvent(cocos3d::TouchEvent_MOVE, touchPoint.x , touchPoint.y , pTouch->getID());
			
		}

	}

	virtual void onTouchesEnded( const std::vector<Touch*>& touches, Event *unused_event )
	{
		float scalex = cocos2d::CCDirector::getInstance()->getOpenGLView()->getScaleX();
		float scaley = cocos2d::CCDirector::getInstance()->getOpenGLView()->getScaleY();

		Touch *pTouch;    
		std::vector<Touch*>::const_iterator iter;
		for (iter = touches.begin(); iter != touches.end(); ++iter)
		{
			pTouch = (Touch *)(*iter);	
			Point touchPoint = pTouch->getLocationInView();

			touchPoint.x *= scalex;
			touchPoint.y *= scaley;

			touchEvent(cocos3d::TouchEvent_RELEASE, touchPoint.x , touchPoint.y , pTouch->getID());
		}
	}

	virtual void onTouchesCancelled( const std::vector<Touch*>&touches, Event *unused_event )
	{

	}
    
    virtual void draw(cocos2d::Renderer* renderer, const kmMat4 &transform, bool transformUpdated)
    {
        C3DLayer::draw3D();
    }
    


};

#endif