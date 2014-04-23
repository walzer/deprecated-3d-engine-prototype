#include "CarTestLayer.h"

//#include "touch_dispatcher/CCTouch.h"
#include <map>

#include "C3DViewport.h"
#include "C3DMatrix.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "C3DFrameBuffer.h"
#include "C3DTexture.h"

#include "C3DPostProcess.h"
#include "C3DPostEffect.h"
#include "VisibleRect.h"
#include "C3DMaterial.h"

#include "C3DRenderNodeManager.h"

using namespace cocos3d;
	
CarTestLayer::CarTestLayer()
{

}

CarTestLayer::~CarTestLayer()
{
	//SAFE_RELEASE(_obj);
	SAFE_RELEASE(_sm);
}

bool CarTestLayer::init()
{
	bool r = C3DLayer::init();
		
	setUpCamera();
	setUpLight();
	setUpScene();
	setUpCar();
	return r;
}

void CarTestLayer::update(float dt)
{
	long elapsedTime = (long)(dt*1000.0f+0.5f);
	C3DLayer::update(elapsedTime);

}

void CarTestLayer::draw3D(void)
{
	C3DLayer::draw3D();
}	

void CarTestLayer::setUpScene()
{
	
    _sm = static_cast<cocos3d::C3DStaticObj*>(C3DRenderNodeManager::getInstance()->getResource("demores/showcase/garage.ckb"));
	
	C3DMaterial* _mat = _sm->getMaterial("floor");
	_sm->scale(40, 40, 40);

	C3DSampler* sampler = C3DSampler::create("demores/showcase/fs01.jpg", true);
	sampler->retain();

	sampler->setFilterMode(Texture_Filter_LINEAR, Texture_Filter_LINEAR);
	sampler->setWrapMode(Texture_Wrap_REPEAT, Texture_Wrap_REPEAT);

	_mat->getTechnique(0u)->getPass(0u)->getParameter("u_diffuseTexture1")->setValue(sampler);

	_scene->addChild(_sm);
	_sm->retain();

}

void CarTestLayer::setUpCar()
{

	//_sm = C3DStaticObj::create("car");
	//_sm->loadFromFile("demores/showcase/supercar.ckb");
    
    _sm = static_cast<cocos3d::C3DStaticObj*>(C3DRenderNodeManager::getInstance()->getResource("demores/showcase/supercar.ckb"));
    
	_scene->addChild(_sm);
	_sm->scale(10, 10, 10);
	_sm->setPositionY(11);
	_sm->retain();
}

void CarTestLayer::setUpCamera()
{
	C3DCamera* camera = C3DCamera::createPerspective(45, 0.75f, 1, 1000);
	camera->lookAt(C3DVector3(90,90,90), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));

	_scene->addChild(camera);
	_scene->setActiveCamera(0);	
}

void CarTestLayer::setUpLight()
{
	C3DVector3 color(1.5f, 1.5f, 1.5f);
	C3DLight* light = C3DLight::create("main light");
	light->setPosition(0,50,20);
	light->lookAt(C3DVector3(0, 50, 20), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
	light->setComponent(C3DDirectionalLight::create(color));
	//light->active(true);
	_scene->addChild(light);

	C3DVector3 color1(0.5f, 0.5f, 0.5f);
	C3DLight* light2 = C3DLight::create("sub light");
	light2->setPosition(0,-50,20);
	light2->lookAt(C3DVector3(0, -50 ,20), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
	light2->setComponent(C3DDirectionalLight::create(color1));
	//light2->active(true);
	_scene->addChild(light2);


	//C3DVector3 pointLightColor(1.0f, 1.0f, 1.0f);
	//C3DLight* pointLight = C3DLight::create("pointLight");
	//pointLight->setComponent(C3DPointLight::create(pointLightColor, 100));
	//pointLight->setLightEnable(true);
	//pointLight->setPosition(20,20,20);
	//_scene->addChild(pointLight);

	//C3DVector3 pointLightColor(1.0f, 1.0f, 1.0f);
	//C3DLight* pointLight = C3DLight::create("pointLight");
	//pointLight->setComponent(C3DPointLight::create(pointLightColor, 100));
	//pointLight->setLightEnable(false);

	//std::string strPointLightUrl = "demores/ball/ball.ckb";
	//cocos3d::C3DStaticObj* pointLightModel = cocos3d::C3DStaticObj::create("pointLightModel");
	//pointLightModel->loadFromFile(strPointLightUrl, true);
	//pointLightModel->setScale(3.0f);

	//_scene->addChild(pointLightModel);
	//pointLightModel->addChild(pointLight);

	//pointLightModel->setMaterial("demores/ball/projected_texture_ball.material");
	//pointLightModel->active(false);
}

void CarTestLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
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

			int deltaY = y - _touchY;
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


CCLayer* CarTestLayer::createUILayer()
{
	CCLayer* layer = CCLayer::create();

	CCMenu* pItemMenu = CCMenu::create();

/*
	for (int i = 0; i < MATERIAL_NUM; ++i)
	{
		// #if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
		//         CCLabelBMFont* label = CCLabelBMFont::create(g_aTestNames[i].c_str(),  "fonts/arial16.fnt");
		// #else
		CCLabelTTF* label = CCLabelTTF::create(materialTypes[i], "Arial", 20);
		// #endif        
		CCMenuItemLabel* pMenuItem = CCMenuItemLabel::
(label, this, menu_selector(MaterialTestLayer::menuCallback));

		pItemMenu->addChild(pMenuItem, i + 10000);
		pMenuItem->setPosition( ccp( 20 + VisibleRect::left().x + label->getContentSize().width / 2, (VisibleRect::top().y - (i + 1) * 24) ));
	}
*/
	//pItemMenu->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, (MATERIAL_NUM + 1) * (40)));
	pItemMenu->setPosition(0, 0);
	layer->addChild(pItemMenu, 1000);


	return layer;
}
