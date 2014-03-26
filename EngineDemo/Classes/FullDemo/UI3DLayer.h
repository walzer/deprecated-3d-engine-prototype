#ifndef __UI3DLAYER_H__
#define __UI3DLAYER_H__

#include "CCNode.h"

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

	//virtual void draw(void);//render the 3d contents
	 virtual void draw(cocos2d::Renderer* renderer, const kmMat4 &transform, bool transformUpdated)
    {
        C3DLayer::draw3D();
    }
	static UI3DLayer* getInstance();

	CREATE_FUNC(UI3DLayer);

	void setUpScene();

    // setup initialize camera
    void setUpCamera();
    //setup initialize light
    void setUpLight();

   
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
