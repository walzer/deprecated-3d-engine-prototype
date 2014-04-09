#include "PerformanceTestLayer.h"

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

enum ColorType
{
    ORIGNAL,
    RED,
    GREEN,
    BLUE,
    WHITE,

    COLOR_NUM
};

static const char* colorTypes[COLOR_NUM] =
{
    "Orininal",
    "Red",
    "Green",
    "Blue",
    "White",
};

PerformanceTestLayer::PerformanceTestLayer()
{
}

PerformanceTestLayer::~PerformanceTestLayer()
{
}

bool PerformanceTestLayer::init()
{
    bool r = C3DLayer::init();

    setUpCamera();
    setUpLight();
    setUpScene();

    return r;
}

void PerformanceTestLayer::update( float dt )
{
    long elapsedTime = (long)(dt*1000.0f+0.5f);
    C3DLayer::update(elapsedTime);

    C3DLight* light = get3DScene()->getLight(0);

    if (light)
    {
        light->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), dt * 1.2f);
    }
}

void PerformanceTestLayer::draw3D()
{
    C3DLayer::draw3D();
}

void PerformanceTestLayer::setUpScene()
{
    for (int i = 0; i < 10; i++)
    {
		for (int j = 0; j < 10; j++)
		{
			C3DSprite* sm = static_cast<cocos3d::C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/haigui/haigui.ckb"));
			sm->addAnimationClip("all", 0 , 240, 0, 1.0f);
			sm->setMaterial("body", "demores/haigui/haigui.material");
			sm->playAnimationClip("all");
			sm->setPosition(-50 +i * 10, -50 + j * 10, 0);
			sm->setScale(1.0f);
			_scene->addChild(sm);
		}
    }
}

void PerformanceTestLayer::setUpCamera()
{
    C3DCamera* camera = C3DCamera::createPerspective(45, 0.75f, 1, 1000);
    camera->lookAt(C3DVector3(0,0,100), C3DVector3(0, 0, 0), C3DVector3(0, 0, 0));

    _scene->addChild(camera);
    _scene->setActiveCamera(0);
}

void PerformanceTestLayer::setUpLight()
{
}

void PerformanceTestLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
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

void PerformanceTestLayer::menuCallback( CCObject * pSender )
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;

    C3DRenderNode* sm = (C3DRenderNode*)_scene->findNode("hua");
    if (sm)
    {
        MaterialParameter* mp = sm->getMaterial("body")->getParameter("u_diffuseColor");
        switch (nIdx)
        {
        case ORIGNAL:
            mp->setValue(C3DVector4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        case RED:
            mp->setValue(C3DVector4(1.0f, 0.3f, 0.3f, 1.0f));
            break;
        case GREEN:
            mp->setValue(C3DVector4(0.3f, 1.0f, 0.3f, 1.0f));
            break;
        case BLUE:
            mp->setValue(C3DVector4(0.3f, 0.3f, 1.0f, 1.0f));
            break;
        case WHITE:
            mp->setValue(C3DVector4(2.0f, 5.0f, 2.0f, 1.0f));
            break;
        }
    }
}

CCLayer* PerformanceTestLayer::createUILayer()
{
    CCLayer* layer = CCLayer::create();

    CCMenu* pItemMenu = CCMenu::create();

    for (int i = 0; i < COLOR_NUM; ++i)
    {
        CCLabelTTF* label = CCLabelTTF::create(colorTypes[i], "Arial", 20);
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, CC_CALLBACK_1(PerformanceTestLayer::menuCallback,this));

        pItemMenu->addChild(pMenuItem, i + 10000);
        pMenuItem->setPosition( ccp( 20 + VisibleRect::left().x + label->getContentSize().width / 2, (VisibleRect::top().y - (i + 1) * 24) ));
    }

    pItemMenu->setPosition(0, 0);
    layer->addChild(pItemMenu, 1000);

    return layer;
}