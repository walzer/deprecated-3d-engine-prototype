#include "AnimLightTestLayer.h"

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

static bool s_lightTranslate = false;
static bool s_lightRotate = false;

AnimLightTestLayer::AnimLightTestLayer()
	: _totalTime( 0.0 )
{
}

AnimLightTestLayer::~AnimLightTestLayer()
{
}

bool AnimLightTestLayer::init()
{
    bool r = C3DLayer::init();

    setUpCamera();
    setUpLight();
    setUpScene();

    return r;
}

void AnimLightTestLayer::update( float dt )
{
    long elapsedTime = (long)(dt*1000.0f+0.5f);
    C3DLayer::update(elapsedTime);

	_totalTime += dt;

    C3DLight* light = (C3DLight*) _scene->findNode("anim light");
    if (light)
	{
		C3DLightComponent* lightComp = light->getComponent();
		C3DAnimatedLight* pAnimLight = static_cast<C3DAnimatedLight*> (lightComp);
        if ( pAnimLight != NULL )
        {
			if (s_lightTranslate)
			{
				pAnimLight->setOffsetUV( _totalTime*0.1, 0, _totalTime*0.2, 0 );
				//light->translate(dt * C3DVector3(10, 1, 0.5));
			}
			if (s_lightRotate)
			{
				//pAnimLight->setOffsetUV( dt*0.1, 0, dt*0.2, 0 );
				light->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), dt);
			}
        }
    }
}

void AnimLightTestLayer::draw3D()
{
    C3DLayer::draw3D();
}

void AnimLightTestLayer::setUpScene()
{
    C3DStaticObj* sm = static_cast<cocos3d::C3DStaticObj*>(C3DRenderNodeManager::getInstance()->getResource("demores/materialtest/1.ckb"));
    sm->setMaterial("demores/materialtest/1.material");
    sm->translate(0, 0, 0);
    sm->scale(50, 50, 50);

    _scene->addChild(sm);
}

void AnimLightTestLayer::setUpCamera()
{
    C3DCamera* camera = C3DCamera::createPerspective(45, 0.75f, 1, 1000);
    //camera->setPosition(0,0,100);
    camera->lookAt(C3DVector3(0,50,100), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
    //camera->rotateX(MATH_DEG_TO_RAD(-20.0f));

    _scene->addChild(camera);
    _scene->setActiveCamera(0);
}

void AnimLightTestLayer::setUpLight()
{
    createAnimLight();
}

void AnimLightTestLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
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


CCLayer* AnimLightTestLayer::createUILayer()
{
    CCLayer* layer = CCLayer::create();

    CCMenu* pItemMenu = CCMenu::create();

    static const char* name[] = { "Light Translate", "Light Rotate" };
    for (int i = 0; i < 2; ++i)
    {
        // #if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
        //         CCLabelBMFont* label = CCLabelBMFont::create(g_aTestNames[i].c_str(),  "fonts/arial16.fnt");
        // #else
        CCLabelTTF* label = CCLabelTTF::create(name[i], "Arial", 20);
        // #endif
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, CC_CALLBACK_1(AnimLightTestLayer::menuCallback,this));

        pItemMenu->addChild(pMenuItem, i + 10000);
        pMenuItem->setPosition( ccp( 20 + VisibleRect::left().x + label->getContentSize().width / 2, (VisibleRect::top().y - (i + 1) * 24) ));
    }

    //pItemMenu->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, (MATERIAL_NUM + 1) * (40)));
    pItemMenu->setPosition(0, 0);
    layer->addChild(pItemMenu, 1000);

    return layer;
}

void AnimLightTestLayer::createAnimLight()
{
    C3DLight* light = C3DLight::create("anim light");
    C3DAnimatedLight* animLight = C3DAnimatedLight::create(C3DVector3(1.0f, 0.0f, 0.0f), "demores/effect/sceneEffect/caustics.png");
    animLight->setOffsetUV(0, 0, 0, 0);
    animLight->setScaleUV(0.017f, 0.017f, 0.03f, 0.03f);
    animLight->setSpeedUV(0.01f, 0.01f, 0.01f, 0.01f);
    //animLight->setRotateUV(0.0f, MATH_DEG_TO_RAD(90));
    //animLight->setRotateSpeedUV(0.01f, -0.05f);
    animLight->setIntensity(1.0f, 0.5f, 0.1f, 5.0f);
    light->setComponent(animLight);
    light->lookAt(C3DVector3(0, 150, 0), C3DVector3(0, 0, 1), C3DVector3(0, 0, 0));
    _scene->addChild(light);

	light->showAABB( true );
}

void AnimLightTestLayer::menuCallback( CCObject * pSender )
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;

    if (nIdx == 0)
    {
        s_lightTranslate = !s_lightTranslate;
    }
    if (nIdx == 1)
    {
        s_lightRotate = !s_lightRotate;
    }
}