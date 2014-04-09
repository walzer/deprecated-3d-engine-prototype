#include "TunnelTestLayer.h"

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

TunnelTestLayer::TunnelTestLayer()
{
}

TunnelTestLayer::~TunnelTestLayer()
{
}

bool TunnelTestLayer::init()
{
    bool r = C3DLayer::init();

    setUpCamera();
    setUpLight();
    setUpScene();

    return r;
}

void TunnelTestLayer::update(float dt)
{
    long elapsedTime = (long)(dt*1000.0f+0.5f);
    C3DLayer::update(elapsedTime);
    static float time = 0;
    time += dt;
    C3DRenderNode* cylinder = (C3DRenderNode*)_scene->findNode("demores/cylinder/cylinder.ckb");
    if (cylinder)
    {
        C3DMaterial* material = cylinder->getMaterial("Cylinder001");
        if (material)
        {
            material->getParameter("u_time")->setValue(time);
        }
    }
}

void TunnelTestLayer::draw3D()
{
    C3DLayer::draw3D();
}

void TunnelTestLayer::setUpScene()
{
    C3DStaticObj* sm = static_cast<cocos3d::C3DStaticObj*>(C3DRenderNodeManager::getInstance()->getResource("demores/cylinder/cylinder.ckb"));

    sm->translate(0, 0, -330);
    sm->rotateX(1.57f);
    sm->scale(50, 50, 50);
    _scene->addChild(sm);
}

void TunnelTestLayer::setUpCamera()
{
    C3DCamera* camera = C3DCamera::createOrthographic(_width / 10, _height / 10, 1, 1, 900);
    camera->setPosition(0,0,100);

    _scene->addChild(camera);
    _scene->setActiveCamera(0);
}

void TunnelTestLayer::setUpLight()
{
}

void TunnelTestLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
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
            _touchX = x;
            _touchY = y;
        }
        break;
    default:
        break;
    };
}



CCLayer* TunnelTestLayer::createUILayer()
{
    CCLayer* layer = CCLayer::create();

    return layer;
}