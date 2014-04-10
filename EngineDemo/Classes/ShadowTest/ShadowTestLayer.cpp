#include "ShadowTestLayer.h"

#include <map>

#include "C3DViewport.h"
#include "C3DMatrix.h"
#include "C3DAnimatedLight.h"
#include "C3DNoise.h"
#include "C3DMaterial.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "MaterialParameter.h"
#include "C3DFrameBuffer.h"
#include "C3DSampler.h"
#include "C3DRenderTarget.h"
#include "C3DDepthStencilTarget.h"
#include "C3DShadowMap.h"
#include "VisibleRect.h"
#include "C3DRenderNodeManager.h"

using namespace cocos3d;

ShadowTestLayer::ShadowTestLayer()
{
}

ShadowTestLayer::~ShadowTestLayer()
{
}

bool ShadowTestLayer::init()
{
    bool r = C3DLayer::init();

    setUpCamera();
    setUpLight();
    setUpShadowMap();
    setUpScene();

    return r;
}

void ShadowTestLayer::update( float dt )
{
    long elapsedTime = (long)(dt*1000.0f+0.5f);
    C3DLayer::update(elapsedTime);

    C3DLight* light = get3DScene()->getLight(0);

    if (light)
    {
        light->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), dt * 1.2f);
    }

    cocos3d::C3DSprite* fish = ((C3DSprite*)_scene->findNode("denglongyu"));
    if(fish)
    {
        // m2->setScreenPos(210, 200, 0.51);
        //   m->setScale(0.2f, 0.2f, 0.2f);

        //	_scene->removeChild(m);
        //	 createFish("denglongyu","2.5D/fish","denglongyu","denglongyu",0,0,0,0,60);
        //fish->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), -dt * 0.5);
    }
}

void ShadowTestLayer::draw3D()
{
    C3DLayer::draw3D();
}

void ShadowTestLayer::setUpScene()
{
    C3DStaticObj* sm = static_cast<cocos3d::C3DStaticObj*>(C3DRenderNodeManager::getInstance()->getResource("demores/materialtest/1.ckb"));
    sm->setMaterial("demores/materialtest/1_shadow.material");
    sm->translate(0, 0, 0);
    sm->scale(50, 50, 50);

    _scene->addChild(sm);

//     C3DSprite* fish = static_cast<cocos3d::C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/haigui/haigui.ckb"));
//     fish->addAnimationClip("idle", 0, 60, 0, 1.0f);
// 
//     fish->playAnimationClip("idle");
//     fish->setPosition(35, 10, 0);
// 	//fish->showSkeleton(true);
//     fish->setScale(5.0f);
// 	//fish->showBoundingBox(true);
// 	//fish->showSkeleton(true);
// 	fish->showOBB(false);
    //_scene->addChild(fish);
}

void ShadowTestLayer::setUpCamera()
{
    C3DCamera* camera = C3DCamera::createPerspective(45, 0.75f, 1, 1000);
    //camera->setPosition(0,0,100);
    camera->lookAt(C3DVector3(0,50,100), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
    //camera->rotateX(MATH_DEG_TO_RAD(-20.0f));

    _scene->addChild(camera);
    _scene->setActiveCamera(0);
}

void ShadowTestLayer::setUpLight()
{
    C3DVector3 color(1.0f, 1.0f, 1.0f);
    C3DLight* light = C3DLight::create("main light");
    light->setPosition(0,0,20);
    light->lookAt(C3DVector3(40, 60, 20), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
    light->setComponent(C3DDirectionalLight::create(color));

    _scene->addChild(light);
}

void ShadowTestLayer::setUpShadowMap()
{
    C3DNode* light = _scene->findNode("main light");
    if (!light)
        return;
    C3DShadowMap* shadowMap = C3DShadowMap::create("shadow", 512, 512);
	if(shadowMap == NULL)
		return;

    shadowMap->setDimension(60, 60, 160);
    light->addChild(shadowMap);
    _scene->setActiveShadowMap(0);
}

void ShadowTestLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
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



CCLayer* ShadowTestLayer::createUILayer()
{
    CCLayer* layer = CCLayer::create();

    return layer;
}