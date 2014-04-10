#include "ChangeClothesTestLayer.h"

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
#include "VisibleRect.h"
#include "ClothUILayer.h"
#include "C3DRenderNodeManager.h"

#include "../C3DActor.h"

using namespace cocos3d;
namespace cocos2d
{
static ChangeClothesTestLayer* __changeClothesInstance = NULL;

ChangeClothesTestLayer::ChangeClothesTestLayer()
{
	__changeClothesInstance = this;

	_actor = NULL;
}

ChangeClothesTestLayer::~ChangeClothesTestLayer()
{
	__changeClothesInstance = NULL;

	SAFE_RELEASE(_actor);
}

ChangeClothesTestLayer* ChangeClothesTestLayer::getInstance()
{
	return __changeClothesInstance;
}

bool ChangeClothesTestLayer::init()
{
    bool r = C3DLayer::init();

    setUpCamera();
    setUpLight();
    setUpScene();

    return r;
}

void ChangeClothesTestLayer::update( float dt )
{
    long elapsedTime = (long)(dt*1000.0f+0.5f);
    C3DLayer::update(elapsedTime);

    C3DLight* light = get3DScene()->getLight(0);

    if (light)
    {
        light->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), dt * 1.2f);
    }
}

void ChangeClothesTestLayer::draw3D()
{
    C3DLayer::draw3D();
}

void ChangeClothesTestLayer::createLive()
{
	cocos3d::C3DSprite* entity = static_cast<cocos3d::C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/girl/test.ckb"));
	if(entity != NULL)
	{
		std::string name = "girl_1";
		_actor = new C3DActor(name,entity,this);
		entity->addAnimationClip("idle",0,80,0,1.0f);
		entity->addAnimationClip("idle1",240,320,0,1.0f);
		entity->playAnimationClip("idle1");

		_actor->addPartConfig("cap",true);
		_actor->addPartConfig("hair",true);
		_actor->addPartConfig("face",false);
		_actor->addPartConfig("glasses",true);
		_actor->addPartConfig("shangshen",false);
		_actor->addPartConfig("hand",false);
		_actor->addPartConfig("xiashen",false);
		_actor->addPartConfig("shoes",false);

		_actor->addPart("cap",		"Girl_Maozi_01",	"demores/girl/Girl_Maozi_01.material");
		_actor->addPart("hair",		"Girl_Toufa_01",	"demores/girl/Girl_Toufa_01.material");
		_actor->addPart("hair",		"Girl_Toufa_02",	"demores/girl/Girl_Toufa_02.material");
		_actor->addPart("face",		"Girl_Lian_01",		"demores/girl/Girl_Lian_01.material");
		_actor->addPart("glasses",	"Girl_Yanjing_01",	"demores/girl/Girl_Yanjing_01.material");
		_actor->addPart("shangshen","Girl_Shangshen_01","demores/girl/Girl_Shangshen_01.material");
		_actor->addPart("shangshen","Girl_Shangshen_01","demores/girl/Girl_Shangshen_03.material");
		_actor->addPart("shangshen","Girl_Shangshen_02","demores/girl/Girl_Shangshen_02.material");
		_actor->addPart("hand",		"Girl_Shou_01",		"demores/girl/Girl_Shou_01.material");
		_actor->addPart("xiashen",	"Girl_Xiashen_01",	"demores/girl/Girl_Xiashen_01.material");
		_actor->addPart("xiashen",	"Girl_Xiashen_02",	"demores/girl/Girl_Xiashen_02.material");
		_actor->addPart("shoes",	"Girl_Xie_01",		"demores/girl/Girl_Xie_01.material");
		_actor->addPart("shoes",	"Girl_Xie_02",		"demores/girl/Girl_Xie_02.material");

		_actor->setPart("cap",1);
		_actor->setPart("hair",1);
		_actor->setPart("face",0);
		_actor->setPart("glasses",1);
		_actor->setPart("shangshen",0);
		_actor->setPart("hand",0);
		_actor->setPart("xiashen",1);
		_actor->setPart("shoes",0);
		_actor->loadParts();

		_actor->createMorph("Girl_Lian_01");
		_actor->setMorphToMesh("eye", 0,0.0f);
		_actor->setMorphToMesh("nose",1,0.0f);

		//entity->setRotationX(MATH_DEG_TO_RAD(-90));
		entity->setPosition(0.0f,0.0f,0.0f);
		entity->setScale(10.0f,10.0f,10.0f);

        _scene->addChild(entity);
	}
}
void ChangeClothesTestLayer::setUpScene()
{
    //C3DStaticObj* sm = C3DStaticObj::create("1");
    //
    //sm->loadFromFile("scene/1.ckb");

    //sm->setMaterial("scene/1_diffuse.material");
    //sm->translate(0, 0, 0);
    ////sm->rotateX(0.8f);
    //sm->scale(50, 50, 50);

    //_scene->addChild(sm);

	this->createLive();
}

void ChangeClothesTestLayer::setUpCamera()
{
    C3DCamera* camera = C3DCamera::createPerspective(45, 0.75f, 1, 1000);
    //camera->setPosition(0,0,100);
    camera->lookAt(C3DVector3(0,2,6), C3DVector3(0, 1, 0), C3DVector3(0, 2, 0));
    //camera->rotateX(MATH_DEG_TO_RAD(-20.0f));

    _scene->addChild(camera);
    _scene->setActiveCamera(0);
}

void ChangeClothesTestLayer::setUpLight()
{
    C3DVector3 color(1.0f, 1.0f, 1.0f);
    C3DLight* light = C3DLight::create("main light");
    light->setPosition(0,0,20);
    light->lookAt(C3DVector3(40, 60, 20), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
    light->setComponent(C3DDirectionalLight::create(color));

    _scene->addChild(light);
}

void ChangeClothesTestLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
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


CCLayer* ChangeClothesTestLayer::createUILayer()
{
	ClothUILayer* uiLayer = ClothUILayer::create();

    return uiLayer;
}

void ChangeClothesTestLayer::changePart(std::string partType)
{
	//cocos3d::C3DSprite* entity = ((C3DSprite*)_scene->findNode("girl_1"));

	if( _actor!=NULL )
	{
		_actor->changePart(partType);
	}
}

void ChangeClothesTestLayer::changeMorph(std::string type,float weight)
{
	//cocos3d::C3DSprite* entity = ((C3DSprite*)_scene->findNode("girl_1"));
	if( _actor!=NULL )
	{
		_actor->changeMorph(type,weight);
	}
}
}