#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorldScene : public cocos2d::Layer
{
public:
    // a selector callback
    void menuCloseCallback(Ref* pSender);
	virtual void onEnter() override;
};

#endif // __HELLO3D_SCENE_H__
