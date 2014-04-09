#include "FullDemoLayer.h"

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
#include "C3DAnimation.h"
#include "C3DProfile.h"

using namespace cocos3d;

#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
namespace cocos2d
{
static FullDemoLayer* __avatarInstance = NULL;

FullDemoLayer::FullDemoLayer()
{
	__avatarInstance = this;

	_touched  =false;
	_touchX = 0;

	_plane = new C3DPlane(cocos3d::C3DVector3(0,1,0),0);
	_mainPlayer = NULL;
}

FullDemoLayer::~FullDemoLayer()
{
	__avatarInstance = NULL;

	SAFE_DELETE(_plane);

	SAFE_RELEASE(_mainPlayer);

	if (_actors.size() > 0)
    {
		std::list<C3DActor*>::iterator iter = _actors.begin();
        while (iter != _actors.end())
        {
            C3DActor* an = *iter;
            SAFE_RELEASE(an);
            ++iter;
        }
		_actors.clear();
    }
}

bool FullDemoLayer::init()
{
	bool r = C3DLayer::init();

	C3DEnvConf conf;
	conf._clearColor = C3DVector4(30.f / 255.f, 60.f / 255.f, 100.f / 255.f, 1.f);
	conf._fogColor = C3DVector4(30.f / 255.f, 54.f / 255.f, 84.f / 255.f, 1.f);
	conf._fogType = C3DEnvConf::Linear;
	conf._fogStart = 6.f;
	conf._fogEnd = 30.f;
	setEnvConf(conf);

	setUpCamera();
    setUpLight();

	//setUpShadowMap();

    setUpScene();
    return r;
}

FullDemoLayer* FullDemoLayer::getInstance()
{
	return __avatarInstance;
}

void FullDemoLayer::setUpCamera()
{
    //C3DCamera* camera = C3DCamera::createOrthographic(getViewport()->width, getViewport()->height, 1, 1, 1000);

	float rate = (float)C3DRenderSystem::getInstance()->getViewport()->width/(float)C3DRenderSystem::getInstance()->getViewport()->height;
    C3DCamera* camera = C3DCamera::createPerspective(45, rate, 1, 1000);

	//camera->setPosition(0,0,100);
    camera->lookAt(C3DVector3(0,30,10), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
	//camera->rotateX(MATH_DEG_TO_RAD(-20.0f));

   _scene->addChild(camera);
    _scene->setActiveCamera(0);

  /* camera = C3DCamera::createOrthographic(_width / 10, _height / 10, 1, 1, 200);
    camera->setPosition(0,0,100);

    _scene->addChild(camera);
   _scene->setActiveCamera(0);*/
}

void FullDemoLayer::setUpLight()
{
    C3DVector3 color(0.3f, 0.3f, 0.3f);
	C3DLight* light = C3DLight::create("main light");
    light->setPosition(0,0,20);
	light->lookAt(C3DVector3(40, 60, 20), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
	light->setComponent(C3DDirectionalLight::create(color));

    _scene->addChild(light);

    createAnimLight();
}

void FullDemoLayer::setUpShadowMap()
{
    C3DNode* light = _scene->findNode("main light");
    if (!light)
        return;
    C3DShadowMap* shadowMap = C3DShadowMap::create("shadow", 512, 512);
    shadowMap->setDimension(60, 60, 160);
    light->addChild(shadowMap);
    _scene->setActiveShadowMap(0);
}

void FullDemoLayer::setUpScene()
{
	//_scene->useOctree(true);

	createMainPlayer();

	createStaticModel();
	createEnemy();
	createParticleEffect();
    setUpProfile();
}

void FullDemoLayer::createStaticModel()
{
	C3DStaticObj* sm = static_cast<cocos3d::C3DStaticObj*>(C3DRenderNodeManager::getInstance()->getResource("demores/fulldemo/scene/scene.ckb"));
	sm->translate(0, -1, 0);
	sm->scale(1, 1, 1);
	_scene->addChild(sm);
}

void FullDemoLayer::switchProfileShow()
{
    CCNode* node = this->getChildByTag(9999);
    if (node)
        node->setVisible(!node->isVisible());
}

void FullDemoLayer::setUpProfile()
{
    CCLabelBMFont* label = CCLabelBMFont::create("", "fonts/arial16.fnt");
    label->setTag(9999);
    addChild(label);
    SET_PROFILE_DISPLAY(label);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    label->setPosition(size.width / 10, size.height / 5);
    label->setVisible(false);
}

void FullDemoLayer::createParticleEffect()
{
	C3DParticleSystem* ps = C3DParticleSystem::create("huopeng_ps");
	if(ps!=NULL)
	{
		ps->load("demores/particles/fire.particle");
		ps->setScale(1.2f);
		_scene->addChild(ps);
		ps->rotateX(MATH_DEG_TO_RAD(90));
	}
	else
	{
		return;
	}
    
	C3DParticleSystem* ps2 = C3DParticleSystem::create("secondhuopeng_ps");
	if(ps2!=NULL)
	{
		ps2->load("demores/particles/fire01.particle");
		ps2->setScale(1.0f);
		_scene->addChild(ps2);
		ps2->rotateX(MATH_DEG_TO_RAD(90));
	}
	else
	{
		return;
	}

	for(int i = 1 ;i<=20; ++i)
	{
		C3DNode::CloneContext context;
		context.cloneChildren = true;

		context.idSuffix = StringTool::toString(i);
		cocos3d::C3DParticleSystem* newPS = NULL;

		if (rand() % 2 == 0)
		   newPS = (cocos3d::C3DParticleSystem*)ps->clone(context);
		else
			newPS = (cocos3d::C3DParticleSystem*)ps2->clone(context);

		newPS->setScale(CCRANDOM_0_1() * 0.5f + 1.0f );
		_scene->addChild(newPS);

		// zhukaixy: Node的名字先这么凑合用一下吧
		cocos3d::C3DRenderNode* m = ((C3DRenderNode*)_scene->findNode("demores/fulldemo/scene/scene.ckb"));
		if(m)
		{
			std::string name;
			if(i < 10)
			{
				name = "guaidian_00" + context.idSuffix;
			}
		    else
			{
				name = "guaidian_0" + context.idSuffix;
		    }

			m->attach(name, newPS);
		}
	}

	_scene->removeChild(ps);
	_scene->removeChild(ps2);

	C3DParticleSystem* particle = C3DParticleSystem::create("tansform_effect");
	particle->load("demores/particles/nengliangtai.particle");
	particle->rotateX(MATH_DEG_TO_RAD(90));
	_scene->addChild(particle);
	particle->setPosition(0.0f, -5.0f, 0.0f);
}
    
void FullDemoLayer::createEffect()
{
    C3DStaticObj* sm = static_cast<cocos3d::C3DStaticObj*>(C3DRenderNodeManager::getInstance()->getResource("effect/fadecircle.ckb"));

    sm->setMaterial("effect/fadecircle.material");
    sm->translate(0, 0, 0);
    sm->rotateX(MATH_DEG_TO_RAD(-90.0f));

    _scene->addChild(sm);
}

void FullDemoLayer::createMainPlayer()
{
	cocos3d::C3DSprite* entity = NULL;
	entity = static_cast<cocos3d::C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/fulldemo/warrior/warrior.ckb"));
	if(entity != NULL)
	{
		std::string name = "mainPlayer";
		_mainPlayer = new MainPlayer(name,entity,this);

		entity->addAnimationClip("run",0,24,0,1.0f);
		entity->addAnimationClip("idle",100,116,0,1.0f);
		entity->addAnimationClip("attack_normal",170,206,1,1.0f);
		entity->addAnimationClip("attack_skill",370,416,1,1.0f);
		entity->playAnimationClip("idle");

	//	_mainPlayer->addPartConfig("body",false);
	//	_mainPlayer->addPart("body","body","demores/fulldemo/warrior/material/body.material");
	//	_mainPlayer->setPart("body",0);
	//	_mainPlayer->loadParts();

		entity->setPosition(0.0f,0.0f,0.0f);
		entity->setScale(1.0f,1.0f,1.0f);

		_scene->addChild(entity);

		_mainPlayer->init();

	   C3DLight* light = C3DLight::create("head light");
	   light->setComponent(C3DPointLight::create(C3DVector3(0.8f, 0.2f, 0.0f), 8.f));
	   light->setPosition(0.0f, 1.0f, 0.0f);
	   entity->addChild(light);
	}
}

void FullDemoLayer::createEnemy()
{
	cocos3d::C3DSprite* entity = NULL;

	entity = static_cast<cocos3d::C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/fulldemo/guai/guai.ckb"));
	entity->getAnimation()->setQuality(C3DAnimation::Low);

	if(entity != NULL)
	{
		entity->addAnimationClip("run",0,24,0,1.0f);
		entity->addAnimationClip("idle",50,74,0,1.0f);
		entity->addAnimationClip("injure", 100, 118, 1, 1.0f);
		entity->addAnimationClip("vertigo", 200, 232, 1, 1.0f);
		entity->addAnimationClip("die", 270, 271, 0, 0.1f);
		entity->addAnimationClip("standup", 271, 314, 1, 1.0f);
		entity->playAnimationClip("idle");

        entity->setPosition(0.8f,0.0f,7.5f);
		entity->rotateY(MATH_DEG_TO_RAD(90));
        entity->setScale(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < 5; i++)
		{
	
			C3DSprite* newEntity = static_cast<cocos3d::C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/fulldemo/guai/guai.ckb"));

			newEntity->playAnimationClip("idle");

			if(i == 1)
			{
				newEntity->setPosition(-4.0f,0.0f,0.4f);
			}
			else if(i == 2)
			{
				newEntity->setPosition(11.0f,0.0f,0.8f);
			}
			else if( i ==3)
			{
				newEntity->setPosition(5.0f,0.0f,7.0f);
			}
			else if( i ==4)
			{
				newEntity->setPosition(4.0f,0.0f,-9.0f);
			}

			_scene->addChild(newEntity);

			std::string name = newEntity->getId();
			C3DActor* enemy = new Enemy(name,newEntity,this);
            
		    _actors.push_back(enemy);
		}
	}
}

void FullDemoLayer::createNpc()
{
	cocos3d::C3DSprite* entity = static_cast<cocos3d::C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/girl/test.ckb"));
	if(entity != NULL)
	{
		std::string name = "npc";
		C3DActor* npc = new Npc(name,entity,this);
		_actors.push_back(npc);

		entity->addAnimationClip("idle",0,80,0,1.0f);
		entity->addAnimationClip("speak",81,320,1,1.0f);
		entity->playAnimationClip("idle");

		npc->addPartConfig("cap",true);
		npc->addPartConfig("hair",true);
		npc->addPartConfig("face",false);
		npc->addPartConfig("glasses",true);
		npc->addPartConfig("shangshen",false);
		npc->addPartConfig("hand",false);
		npc->addPartConfig("xiashen",false);
		npc->addPartConfig("shoes",false);

		npc->addPart("cap","Girl_Maozi_01","demores/girl/Girl_Maozi_01.material");
		npc->addPart("hair","Girl_Toufa_01","demores/girl/Girl_Toufa_01.material");
		npc->addPart("hair","Girl_Toufa_02","demores/girl/Girl_Toufa_02.material");
		npc->addPart("face","Girl_Lian_01","demores/girl/Girl_Lian_01.material");
		npc->addPart("glasses","Girl_Yanjing_01","demores/girl/Girl_Yanjing_01.material");
		npc->addPart("shangshen","Girl_Shangshen_01","demores/girl/Girl_Shangshen_01.material");
		npc->addPart("shangshen","Girl_Shangshen_01","demores/girl/Girl_Shangshen_03.material");
		npc->addPart("shangshen","Girl_Shangshen_02","demores/girl/Girl_Shangshen_02.material");
		npc->addPart("hand","Girl_Shou_01","demores/girl/Girl_Shou_01.material");
		npc->addPart("xiashen","Girl_Xiashen_01","demores/girl/Girl_Xiashen_01.material");
		npc->addPart("xiashen","Girl_Xiashen_02","demores/girl/Girl_Xiashen_02.material");
		npc->addPart("shoes","Girl_Xie_01","demores/girl/Girl_Xie_01.material");
		npc->addPart("shoes","Girl_Xie_02","demores/girl/Girl_Xie_02.material");

		npc->setPart("cap",1);
		npc->setPart("hair",1);
		npc->setPart("face",0);
		npc->setPart("glasses",1);
		npc->setPart("shangshen",2);
		npc->setPart("hand",0);
		npc->setPart("xiashen",1);
		npc->setPart("shoes",0);
		npc->loadParts();

		npc->createMorph("Girl_Lian_01");
		npc->setMorphToMesh("eye", 0,0.0f);
		npc->setMorphToMesh("nose",1,0.0f);

		entity->setPosition(-42.0f,0.0f,0.0f);
		entity->setScale(8.0f,8.0f,8.0f);

        _scene->addChild(entity);
	}
}

void FullDemoLayer::update(float dt)
{
	long elapsedTime = (long)(dt*1000.0f+0.5f);
	C3DLayer::update(elapsedTime);

	if(_mainPlayer != NULL)
		_mainPlayer->update(elapsedTime);

    static long s = 0;
    static float time = 0;
    s+= elapsedTime;
	time += dt;

    C3DLight* light = (C3DLight*)_scene->findNode("main light");

    if (light)
    {
        light->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), dt * 0.02f);
    }

	for (std::list<C3DActor*>::iterator it = _actors.begin(); it != _actors.end(); it++)
	{
		(*it)->update(elapsedTime);
	}
}

void FullDemoLayer::draw3D(void)
{
    C3DLayer::draw3D();
}

C3DActor* FullDemoLayer::pick( float x,float y )
{
    C3DRay* ray = new C3DRay();
	_scene->getActiveCamera()->getAxes(C3DRenderSystem::getInstance()->getViewport(),x,y,ray);

	for( std::list<C3DActor*>::const_iterator iter = _actors.begin(); iter != _actors.end(); ++iter )
	{
		C3DActor * pCurr = *iter;

		//BoundingBox* bb = (static_cast<C3DSprite*>(pCurr->getNode()))->getAABB();

		C3DOBB* obb = (static_cast<C3DSprite*>(pCurr->getNode()))->getOBB();

		//bool res = ray->intersects(bb);
		bool res = ray->intersects(obb);
		if(res == true)
		{
            SAFE_DELETE(ray);
			return pCurr;
		}
	}
    SAFE_DELETE(ray);
	return NULL;
}

void FullDemoLayer::touchEvent(TouchEvent evt, float x, float y, unsigned int contactIndex)
{
    switch (evt)
    {
    case TouchEvent_PRESS:
        {
            _touched = true;
            _touchX = x;
			_touchY = y;

			C3DActor* picked = pick(_touchX, _touchY);

            if (picked)
            {
				switch(picked->getType())
				{
				case C3DActor::ActorType_Enemy:
					{
						EAttackType attackType = _mainPlayer->getAttackType();
						if ( attackType & AT_MeleeMask )
						{
							_mainPlayer->meleeAttack(picked);
						}
						else if ( attackType & AT_RangeMask )
						{
							_mainPlayer->remoteAttack(picked);
						}
					}
					break;
				case C3DActor::ActorType_Npc:
					{
						_mainPlayer->speak(picked);
					}
					break;
                default:
                    break;
				}
            }
			else
			{
				 static C3DRay ray;
	             _scene->getActiveCamera()->getAxes(C3DRenderSystem::getInstance()->getViewport(),_touchX,_touchY,&ray);
				 C3DVector3 p = ray.intersects(_plane);
				 if(_mainPlayer)
					 _mainPlayer->moveTo(p);
			}
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


void FullDemoLayer::createAnimLight()
{
	C3DLight* light = C3DLight::create("anim light");
	C3DAnimatedLight* animLight = C3DAnimatedLight::create(C3DVector3(1.0f, 0.0f, 0.0f), "demores/effect/sceneEffect/caustics.png");
	animLight->setOffsetUV(0, 0, 0, 0);
	animLight->setScaleUV(0.017f, 0.017f, 0.03f, 0.03f);
	animLight->setSpeedUV(0.05f, 0.03f, 0.01f, 0.01f);
	animLight->setRotateUV(0.0f, MATH_DEG_TO_RAD(90));

	animLight->setIntensity(1.0f, 0.5f, 0.1f, 5.0f);
	light->setComponent(animLight);
	light->lookAt(C3DVector3(20, 100, 50), C3DVector3(0, 0, 1), C3DVector3(0, 0, 0));
	_scene->addChild(light);
}

CCLayer* FullDemoLayer::createUILayer()
{
	FullDemoUILayer* uiLayer = FullDemoUILayer::create();

    return uiLayer;
}

}