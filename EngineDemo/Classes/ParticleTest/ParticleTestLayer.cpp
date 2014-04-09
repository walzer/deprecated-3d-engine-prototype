#include "ParticleTestLayer.h"

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
#include "C3DParticleSystem.h"
#include "C3DLineRender.h"
#include "C3DSprite.h"
#include "C3DRenderNodeManager.h"

using namespace cocos3d;

AnimListenerObject::AnimListenerObject(C3DParticleSystem* particle)
{
    _particle = particle;
}
void AnimListenerObject::onFlowerStart()
{
    _particle->start();
}
void AnimListenerObject::onFlowerEnd()
{
    _particle->stop();
}

ParticleTestLayer::ParticleTestLayer()
	: _lightingTime( 0.0 )
	, _lighting( NULL )
{
	_totalTime = 0;
    _listenerObj = NULL;
}

ParticleTestLayer::~ParticleTestLayer()
{
    if (_listenerObj)
        delete _listenerObj;
}

bool ParticleTestLayer::init()
{
    bool r = C3DLayer::init();

    setUpCamera();
    setUpLight();
    setUpShadowMap();
    setUpScene();

    return r;
}

void ParticleTestLayer::update( float dt )
{
    long elapsedTime = (long)(dt*1000.0f+0.5f);
    C3DLayer::update(elapsedTime);

	_totalTime += dt;
	_lightingTime += dt;

    cocos3d::C3DSprite* fish = ((C3DSprite*)_scene->findNode("denglongyu"));
    if(fish)
    {
        // m2->setScreenPos(210, 200, 0.51);
        //   m->setScale(0.2f, 0.2f, 0.2f);

        //	_scene->removeChild(m);
        //	 createFish("denglongyu","2.5D/fish","denglongyu","denglongyu",0,0,0,0,60);
        fish->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), -dt * 0.5);
    }

	std::vector<C3DLineRender::Line> lines;

	static float starSize = 40;
	static C3DVector3 starPoints[5] = { C3DVector3(0	*starSize, 1	*starSize, 0),
										C3DVector3(0.95	*starSize, 0.31	*starSize, 0),
										C3DVector3(0.59	*starSize, -0.81*starSize ,0),
										C3DVector3(-0.59*starSize, -0.81*starSize, 0),
										C3DVector3(-0.95*starSize, 0.31	*starSize, 0) };
	static int starPointIndex[10] = { 0, 2,
									2, 4,
									4, 1,
									1, 3,
									3, 0 };

	lines.clear();
	float lightingLength = _lightingTime*500;
	for ( int i = 0; i < 5; ++i )
	{
		float starSegmentLength = starPoints[starPointIndex[i*2]].distance( starPoints[starPointIndex[i*2+1]] );
		float length = C3D_Min( starSegmentLength, lightingLength );
		C3DVector3 dir = (starPoints[starPointIndex[i*2+1]]-starPoints[starPointIndex[i*2]]);
		dir.normalize();

		if ( length > 0.0 )
		{
			C3DVector3 segStart = starPoints[starPointIndex[i*2]];
			C3DVector3 segEnd = starPoints[starPointIndex[i*2]]+dir*length;

			lines.push_back( C3DLineRender::Line( segStart, segEnd, i*1000 ) );
			lightingLength -= length;
		}
		else
		{
			break;
		}
	}

	_lighting->setLines( lines );
}

void ParticleTestLayer::draw3D()
{
    C3DLayer::draw3D();
}

void ParticleTestLayer::setUpScene()
{
    C3DSprite* shuicao = static_cast<cocos3d::C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/hua_01/hua_01.ckb"));
	shuicao->setId("shuicao");
    shuicao->addAnimationClip("all", 0 , 240, 0, 1.0f);

    shuicao->playAnimationClip("all");
    shuicao->setPosition(40.0f, 0, 0.0f);
    shuicao->rotateX(0.5f);
    shuicao->showAABB(false);
    shuicao->showOBB(false);
    _scene->addChild(shuicao);

    C3DParticleSystem* particle;

    particle = C3DParticleSystem::create("particle2");
    particle->load("demores/particles/beijingpaopao01.particle");
    particle->setPosition(-20.f, 0.f, 0.f);
    _scene->addChild(particle);

	particle = C3DParticleSystem::create("particle22");
	particle->load("demores/particles/beijingpaopao01.particle");
	particle->setPosition(-40.f, 0.f, 0.f);
	_scene->addChild(particle);

	particle = C3DParticleSystem::create("particle23");
	particle->load("demores/particles/beijingpaopao01.particle");
	particle->setPosition(40.f, 0.f, 0.f);
	_scene->addChild(particle);

    particle = C3DParticleSystem::create("particle3");
    particle->load("demores/particles/beijingpaopao04.particle");
    particle->setPosition(0.f, 0.f, 0.f);
    //particle->setRotationY(MATH_PI / 2.f);
    _scene->addChild(particle);

    particle = C3DParticleSystem::create("particle4");
    particle->load("demores/particles/beijingpaopao03.particle");
    particle->setPosition(20.f, 0.f, 0.f);
    _scene->addChild(particle);

 //   C3DSprite* fish = C3DSprite::create("denglongyu");
 //   fish->loadFromFile("2.5D/fish/denglongyu/denglongyu.ckb",true);
	//fish->setMaterial("body", "2.5D/fish/denglongyu/denglongyu.material");
 //   fish->addAnimationClip("idle", 0, 60, 0, 1.0f);
 //   fish->playAnimationClip("idle");
 //   fish->setPosition(15, 10, 0);
 //   //fish->rotateX(0.5f);
 //   fish->showOBB(false);
 //   fish->showAABB(false);
 //   fish->setScale(5.0f);
 //
 //   particle = C3DParticleSystem::create("paritlce1");
 //   particle->load("particles/firetest.particle");
 //   _scene->addChild(particle);
 //
 //   _scene->addChild(fish);
 //   C3DNode* attach = fish->findNode("Bone03");
	//fish->attach("Bone03", particle);
 //   particle->scale(0.3f, 0.30f, 0.3f);

    particle = C3DParticleSystem::create("xinguang");
    particle->load("demores/particles/hua01_xinguang01.particle");
    C3DNode* attach = shuicao->findNode("Bone004");
    if (attach)
    {
		_scene->addChild(particle);
		shuicao->attach("Bone004", particle);
        //attach->addChild(particle);
        particle->stop();
        C3DAnimationClip* clip = shuicao->getAnimationClip("all");
        _listenerObj = new AnimListenerObject(particle);
        C3DActionListener * start = cocos3d::C3DActionListener::create(_listenerObj,((cocos3d::ListenerFunction)(&AnimListenerObject::onFlowerStart)));
        clip->addActionEvent(start, clip->getDuration() - 6000);
        C3DActionListener * end = cocos3d::C3DActionListener::create(_listenerObj,((cocos3d::ListenerFunction)(&AnimListenerObject::onFlowerEnd)));
        clip->addActionEvent(end, clip->getDuration() - 3000);
		//_scene->removeChild(particle);
    }

    particle = C3DParticleSystem::create("chenai");
    particle->load("demores/particles/ChenAi.particle");
    _scene->addChild(particle);

    particle = C3DParticleSystem::create("suidao");
    particle->load("demores/particles/shikongsuidao02.particle");
    particle->scale(0.2f, 0.2f, 0.2f);
    _scene->addChild(particle);

	particle = C3DParticleSystem::create("xinguang2");
	particle->load("demores/particles/hua01_xinguang01.particle");
	particle->setPosition(-30, 30, 0);
	particle->scale(3.0f);
	_scene->addChild(particle);

	particle = C3DParticleSystem::create("xinguang3");
	particle->load("demores/particles/hua01_xinguang01.particle");
	particle->setPosition(30, 30, 0);
	particle->scale(4.0f);
	_scene->addChild(particle);

	particle = C3DParticleSystem::create("xinguang4");
	particle->load("demores/particles/hua01_xinguang01.particle");
	particle->setPosition(30, -30, 0);
	particle->scale(6.0f);
	_scene->addChild(particle);

	_lighting = C3DLineRender::create( "demores/sceneEffect/lighting.material" );
	_lighting->setForceTexLoop( false );
	_lighting->setWidth( 20 );
	_lighting->setTextueLength( 100 );
	_lighting->setStep( 2 );
	_lighting->setAmplitude0( 4 );
	_lighting->setFrequency0( 5 );
	_lighting->setAmplitude1( 1 );
	_lighting->setFrequency1( 400 );
	_lighting->setTimeFactor( 0.5 );
	_scene->addChild( _lighting );
}

void ParticleTestLayer::setUpCamera()
{
    C3DCamera* camera = C3DCamera::createPerspective(45, 0.75f, 1, 1000);
    //camera->setPosition(0,0,100);
    camera->lookAt(C3DVector3(0,0,100), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
    //camera->rotateX(MATH_DEG_TO_RAD(-20.0f));

    _scene->addChild(camera);
    _scene->setActiveCamera(0);
}

void ParticleTestLayer::setUpLight()
{
    C3DVector3 color(1.0f, 1.0f, 1.0f);
    C3DLight* light = C3DLight::create("main light");
    light->setPosition(0,0,20);
    light->lookAt(C3DVector3(40, 60, 20), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
    light->setComponent(C3DDirectionalLight::create(color));

    _scene->addChild(light);
}

void ParticleTestLayer::setUpShadowMap()
{
}

void ParticleTestLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
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


CCLayer* ParticleTestLayer::createUILayer()
{
	CCLayer* layer = CCLayer::create();
	CCMenu* pItemMenu = CCMenu::create();

	CCLabelTTF* label = CCLabelTTF::create("Lighting", "Arial", 20);
	// #endif
	CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, CC_CALLBACK_1(ParticleTestLayer::menuCallback,this));

	pItemMenu->addChild(pMenuItem, 10000);
	pMenuItem->setPosition( ccp( 20 + VisibleRect::left().x + label->getContentSize().width / 2, (VisibleRect::top().y -  24) ));

	//pItemMenu->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, (MATERIAL_NUM + 1) * (40)));
	pItemMenu->setPosition(0, 0);
	layer->addChild(pItemMenu, 1000);
    return layer;
}
void ParticleTestLayer::menuCallback( CCObject * pSender )
{
	// get the userdata, it's the index of the menu item clicked
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int nIdx = pMenuItem->getZOrder() - 10000;

	if ( nIdx == 0 )
	{
		_lightingTime = 0.0;
	}
}