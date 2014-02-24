#include "PhysicsTestLayer.h"

#ifdef WIN32

#include "touch_dispatcher/CCTouch.h"

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

#include "Physics.h"
#include "C3DCapsule.h"
#include "C3DRenderSystem.h"

using namespace cocos3d;


PhysicsTestLayer::PhysicsTestLayer():_physicScene(NULL)
{

}

PhysicsTestLayer::~PhysicsTestLayer()
{
	if (_physicScene)
		delete _physicScene;
}

bool PhysicsTestLayer::init()
{
    bool r = C3DLayer::init();

    setUpCamera();
    setUpLight();
    setUpScene();   

    return r;
}

void PhysicsTestLayer::update(float dt)
{
    long elapsedTime = (long)(dt*1000.0f+0.5f);
    C3DLayer::update(elapsedTime);
    static float time = 0;
    time += dt;
    C3DRenderNode* cylinder = (C3DRenderNode*)_scene->findNode("cylinder");
    if (cylinder)
    {
        C3DMaterial* material = cylinder->getMaterial("Cylinder001");
        if (material)
        {
            material->getParameter("u_time")->setValue(time);
        }
    }

	if (_physicScene)
		_physicScene->update(dt);
}

void PhysicsTestLayer::draw()
{
	if (_physicScene)
		_physicScene->debugDraw(_scene->getGeoWireRender());

    C3DLayer::draw();
	
}

void PhysicsTestLayer::setUpScene()
{
    C3DStaticObj* sm = C3DStaticObj::create("scene");
    sm->loadFromFile("scene/ChangJing_DiMian/ChangJing_DiMian.ckb", true);

    sm->translate(0, 0, 0);
    sm->rotateX(-1.57f);
    sm->scale(0.8f, 0.8f, 0.8f);
    _scene->addChild(sm);   

	if (_physicScene == NULL)
	{
		PhysicSceneParam param;
		param._scene = _scene;
		param._drawDebug = true;
		_physicScene = PhysicScene::create(param);
		C3DVector3 ext = (sm->getAABB()->_max - sm->getAABB()->_min) * 0.5f;
		
		_physicScene->createGround(C3DVector3(0.0f, 0.0f, 0.0f), ext);

		PhysicBoxShape* box = PhysicBoxShape::create(C3DVector3(1.f, 1.f, 1.f));
		PhysicSphere* sphere = PhysicSphere::create(1.f);
		C3DCapsule capsule;
		capsule.setRadius(1.f);
		capsule.setCylinderHeight(1.0f);
		PhysicCapsule* pycapsule = PhysicCapsule::create(capsule);

		PhysicRigidBodyParam RbParam;
		RbParam._mass = 10.f;
		RbParam._shape = box;
		//RbParam._shape = sphere;
		//RbParam._shape = pycapsule;
		
		RbParam._position.set(15, 10, 0);
		
		   C3DSprite* fish = C3DSprite::create("denglongyu");
		   fish->loadFromFile("2.5D/fish/denglongyu/denglongyu.ckb",true);
		
		   fish->addAnimationClip("idle", 0, 60, 0, 1.0f);
		   fish->playAnimationClip("idle");
		   fish->setPosition(15, 10, 0);
		  
		   fish->setScale(2.0f);
	
		
		PhysicRigidBodyObject* rb = PhysicRigidBodyObject::create(RbParam);
		rb->setNode(fish);
		_scene->addChild(fish);
		
		_physicScene->addPhysicObject(rb);
	}
}

void PhysicsTestLayer::setUpCamera()
{
    C3DCamera* camera = C3DCamera::createPerspective(45.f, 1.333f, 1.f, 1000.f);
	camera->lookAt(C3DVector3(0.f, 30.f, 50.f), C3DVector3(0.0f, 1.0f, 0.0f), C3DVector3(0.0f, 0.0f, 0.0f));

    _scene->addChild(camera);
    _scene->setActiveCamera(0);

}

void PhysicsTestLayer::setUpLight()
{

}


void PhysicsTestLayer::shotPhysicObj()
{
	if (_physicScene && _scene && _scene->getActiveCamera())
	{
		C3DVector3 src(_touchX, _touchY, 0.f);
		C3DVector3 dst;
		_scene->getActiveCamera()->unproject(C3DRenderSystem::getInstance()->getViewport(), &src, &dst);

		C3DVector3 dir = dst - _scene->getActiveCamera()->getTranslationWorld();

		dir.normalize();
		
		int type = rand() % 4;
		PhysicShape* shape = NULL;
		switch(type)
		{
		case 0:
			{
				float ext = CCRANDOM_0_1() * 1.0f + 1.0f;
				shape = PhysicBoxShape::create(C3DVector3(ext, ext, ext));
			}
			break;
		case 1:
			{
				float ext = CCRANDOM_0_1() * 1.0f + 1.0f;
				shape = PhysicSphere::create(ext);
			}
			break;
		case 2:
			{
				C3DCapsule capsule;
				capsule.setRadius(CCRANDOM_0_1() * 1.0f + 1.0f);
				capsule.setCylinderHeight(CCRANDOM_0_1() * 1.0f + 1.0f);
				shape = PhysicCapsule::create(capsule);
			}
			break;
		case 3:
			{
				C3DCylinder clinder(C3DVector3::zero(), C3DCylinder::yAxis, CCRANDOM_0_1() * 1.0f + 1.0f, CCRANDOM_0_1() * 1.0f + 1.0f);
				shape = PhysicCylinder::create(clinder);
			}
			break;
		}

		PhysicRigidBodyParam RbParam;
		RbParam._mass = 10.f;
		RbParam._shape = shape;
		RbParam._linearVelocity = dir * 30.f;
		RbParam._position = _scene->getActiveCamera()->getTranslationWorld();
		static int idx = 0;
		idx++;
		C3DNode* node = _scene->findNode("denglongyu");
		if (node)
		{
			//C3DNode::CloneContext context;
			//context.idSuffix = StringTool::toString(idx);
			//C3DNode* cloneNode = node->clone(context);

			std::string str = "denglongyu";
			str += StringTool::toString(idx);
			C3DSprite* fish = C3DSprite::create(str.c_str());
			fish->loadFromFile("2.5D/fish/denglongyu/denglongyu.ckb",true);

			fish->addAnimationClip("idle", 0, 60, 0, 1.0f);
			fish->playAnimationClip("idle");

			PhysicRigidBodyObject* rb = PhysicRigidBodyObject::create(RbParam);
			rb->setNode(fish);
			_scene->addChild(fish);

			_physicScene->addPhysicObject(rb);
		}
		
	}
}

void PhysicsTestLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
{
    switch (evt)
    {
    case TouchEvent_PRESS:
        {
            _touched = true;
            _touchX = x;
            _touchY = y;
			shotPhysicObj();
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
                //if (camera)
                //    camera->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), MATH_DEG_TO_RAD(deltaX * 0.5f));

            }	
        }
        break;
    default:
        break;
    };
}

void PhysicsTestLayer::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
    CCTouch *pTouch;
    CCSetIterator setIter;
    for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
    {
        pTouch = (CCTouch *)(*setIter);		
        CCPoint touchPoint = pTouch->getLocationInView();
       
        touchEvent(cocos3d::TouchEvent_PRESS, touchPoint.x, touchPoint.y, pTouch->getID());
    }    

}

void PhysicsTestLayer::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
    CCTouch *pTouch;
    CCSetIterator setIter;
    for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
    {
        pTouch = (CCTouch *)(*setIter);
        CCPoint touchPoint = pTouch->getLocationInView();
        
        touchEvent(cocos3d::TouchEvent_MOVE, touchPoint.x, touchPoint.y, pTouch->getID());
    }

}

void PhysicsTestLayer::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
    CCTouch *pTouch;
    CCSetIterator setIter;
    for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
    {
        pTouch = (CCTouch *)(*setIter);
        CCPoint touchPoint = pTouch->getLocationInView();
        
        touchEvent(cocos3d::TouchEvent_RELEASE, touchPoint.x , touchPoint.y , pTouch->getID());
    }
}

void PhysicsTestLayer::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{

}


CCLayer* PhysicsTestLayer::createUILayer()
{
    CCLayer* layer = CCLayer::create();

	CCLabelTTF* label = CCLabelTTF::create("physics test", "Arial", 20);
	label->setPosition(ccp(50, 50));
	layer->addChild(label);

    return layer;
}

#endif