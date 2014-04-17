#include "Hello3DLayer.h"
#include "C3DRenderNodeManager.h"

USING_NS_CC;
using namespace cocos3d;

Hello3DLayer::Hello3DLayer():_touched(false), _touchX(0), _touchY(0)
{

}
Hello3DLayer::~Hello3DLayer()
{
  
}

// on "init" you need to initialize your instance
bool Hello3DLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !C3DLayer::init() )
    {
        return false;
    }
    
    setUpCamera();
    setUpLight();
    setUpScene();   
    
    return true;
}

void Hello3DLayer::setUpScene()
{
    //create fish
    cocos3d::C3DSprite* fish = static_cast<C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/haigui/haigui.ckb"));
	fish->setId("haigui");
    fish->addAnimationClip("idle", 0, 60, 0, 1.0f);	
    fish->scale(3.0f);
    fish->playAnimationClip("idle");
    fish->setPosition(0.0f, 0.0f, 0.0f);
    _scene->addChild(fish);
}

void Hello3DLayer::update(float dt)
{
    long elapsedTime = (long)(dt*1000.0f+0.5f);
    C3DLayer::update(elapsedTime);
}

// setup initialize camera
void Hello3DLayer::setUpCamera()
{
    C3DCamera* camera = C3DCamera::createPerspective(45, 0.75f, 1, 1000);
    camera->lookAt(C3DVector3(0,50,100), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));

    _scene->addChild(camera);
    _scene->setActiveCamera(0);
}
//setup initialize light
void Hello3DLayer::setUpLight()
{
    C3DVector3 color(1.0f, 1.0f, 1.0f);
    C3DLight* light = C3DLight::create("main light");
    light->setPosition(0,0,20);
    light->lookAt(C3DVector3(40, 60, 20), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
    light->setComponent(C3DDirectionalLight::create(color));

    _scene->addChild(light);
}

void Hello3DLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
{
    switch (evt)
    {
    case TouchEvent_PRESS:
        {
            _touched = true;
            _touchX = x;
            _touchY = y;
        }
        break;
    case TouchEvent_RELEASE:
        {
            _touched = false;
            _touchX = 0;
            _touchY = 0;
        }
        break;
    case TouchEvent_MOVE:
        {
            int deltaX = x - _touchX;
            _touchX = x;
            _touchY = y;

            {				
                C3DCamera* camera = _scene->getActiveCamera();
                if (camera)
                    camera->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), MATH_DEG_TO_RAD(deltaX * 0.5f));
            }	
        }
        break;
    default:
        break;
    };
}

void Hello3DLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCTouch *pTouch;
    std::vector<Touch*>::const_iterator iter = touches.begin();
    for (iter = touches.begin(); iter != touches.end(); ++iter)
    {
        pTouch = (Touch *)(*iter);		
        CCPoint touchPoint = pTouch->getLocationInView();

        touchEvent(cocos3d::TouchEvent_PRESS, touchPoint.x , touchPoint.y , pTouch->getID());
    }    
}

void Hello3DLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCTouch *pTouch;
    std::vector<Touch*>::const_iterator iter = touches.begin();
    for (iter = touches.begin(); iter != touches.end(); ++iter)
    {
        pTouch = (CCTouch *)(*iter);
        CCPoint touchPoint = pTouch->getLocationInView();

        touchEvent(cocos3d::TouchEvent_MOVE, touchPoint.x , touchPoint.y , pTouch->getID());
    }
}

void Hello3DLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCTouch *pTouch;
    std::vector<Touch*>::const_iterator iter = touches.begin();
    for (iter = touches.begin(); iter != touches.end(); ++iter)
    {
        pTouch = (CCTouch *)(*iter);
        CCPoint touchPoint = pTouch->getLocationInView();

        touchEvent(cocos3d::TouchEvent_RELEASE, touchPoint.x , touchPoint.y , pTouch->getID());
    }
}