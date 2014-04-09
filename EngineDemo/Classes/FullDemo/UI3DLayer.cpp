#include "UI3DLayer.h"

#include "CCTouch.h"

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
#include "../CCFileUtils.h"
#include "FullDemoUILayer.h"
#include "MainPlayer.h"
#include "Enemy.h"
#include "Npc.h"
#include "C3DRay.h"
#include "C3DPlane.h"
#include "C3DRenderSystem.h"
#include "C3DRenderNodeManager.h"

using namespace cocos3d;

#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
namespace cocos2d
{
static UI3DLayer* __avatarInstance = NULL;

UI3DLayer::UI3DLayer()
{
	__avatarInstance = this;

	_touched  =false;
	_touchX = 0;
}

UI3DLayer::~UI3DLayer()
{
	__avatarInstance = NULL;
}

bool UI3DLayer::init()
{
	bool r = C3DLayer::init();

	C3DEnvConf conf;
	conf._clearColor = C3DVector4(30.f / 255.f, 60.f / 255.f, 100.f / 255.f, 0.0f);
	conf._fogColor = C3DVector4(30.0 / 255.0, 54.0 / 255.0, 84.0 / 255.0, 1.0);
	conf._fogType = C3DEnvConf::Linear;
	conf._fogStart = 6.f;
	conf._fogEnd = 30.f;
	setEnvConf(conf);

	setUpCamera();
    setUpLight();

	// _scene->getActiveCamera()->unproject(C3DRenderSystem::getInstance()->getViewport(),&_base,&_base);
	// setUpScene();
    return r;
}

UI3DLayer* UI3DLayer::getInstance()
{
	return __avatarInstance;
}

void UI3DLayer::setUpCamera()
{
  C3DCamera* camera = C3DCamera::createOrthographic(_width / 10, _height / 10, 1, 1, 200);

	camera->lookAt(C3DVector3(0,0,100), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));

    _scene->addChild(camera);
   _scene->setActiveCamera(0);
}
void UI3DLayer::setUpLight()
{
    C3DVector3 color(1.0f, 1.0f, 1.0f);
	C3DLight* light = C3DLight::create("main light");
    light->setPosition(0,0,20);
	light->lookAt(C3DVector3(40, 60, 20), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
	light->setComponent(C3DDirectionalLight::create(color));

    _scene->addChild(light);
}

void UI3DLayer::setUpScene()
{
	C3DSprite* fish = static_cast<cocos3d::C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/haigui/haigui.ckb"));
    fish->addAnimationClip("idle", 0, 60, 0, 1.0f);
    fish->playAnimationClip("idle");

    fish->setScale(1.0f);

    _scene->addChild(fish);

	fish->setScreenPos(_base.x,_base.y,0.5f);
}

void UI3DLayer::update(float dt)
{
	long elapsedTime = (long)(dt*1000.0f+0.5f);
	C3DLayer::update(elapsedTime);
}

//void UI3DLayer::draw(void)
//{
   // glClear(GL_DEPTH_BUFFER_BIT);
 //   C3DLayer::draw();
//}

void UI3DLayer::setBase(cocos3d::C3DVector3& base)
{
	_base = base;
	//_scene->getActiveCamera()->unproject(C3DRenderSystem::getInstance()->getViewport(),&_base,&_base);
}
}