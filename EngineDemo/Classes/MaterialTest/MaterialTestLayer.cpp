#include "MaterialTestLayer.h"

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
#include "C3DModelNode.h"
#include "C3DRenderNodeManager.h"

using namespace cocos3d;

enum MaterialType
{
    NOLIGHT,
    DIFFUSE,
    SPECLUAR,
    TRANSMISSION,
    NORMALMAP,
	WOOD,
	TOON,
    REFLECTIVE,
	PROJECTED_TEXTURE,
    MATERIAL_NUM
};

static const char* materialTypes[MATERIAL_NUM] =
{
    "No Light",
    "Diffuse Light",
    "With Specular",
    "With Transmission",
    "With Normal Mapping",
	"With Wood",
	"With Toon",
    "Reflective cube map",
	"Projected Texture"
};

MaterialTestLayer::MaterialTestLayer()
{
}

MaterialTestLayer::~MaterialTestLayer()
{
    SAFE_RELEASE(_fish);
    SAFE_RELEASE(_sm);
}

bool MaterialTestLayer::init()
{
    bool r = C3DLayer::init();

    setUpCamera();
    setUpLight();
    setUpScene();

    return r;
}

void MaterialTestLayer::update( float dt )
{
    long elapsedTime = (long)(dt*1000.0f+0.5f);
    C3DLayer::update(elapsedTime);

    C3DLight* light = get3DScene()->getLight(0);

    if (light)
    {
        light->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), dt * 1.2f);
    }

	C3DNode* pointLightNode = _scene->findNode("PTBall");
	if(pointLightNode)
	{
		static float dtAcc = dt;
		dtAcc += dt*0.6f;

		pointLightNode->setRotationZ(dtAcc/2);
		pointLightNode->setPositionX(sin(dtAcc)*20);
		pointLightNode->setPositionY(sin(dtAcc/2)*15 + 20);
		pointLightNode->setPositionZ(sin(dtAcc/3)*30);

		C3DVector3 pointLightColor(cos(dtAcc)*0.1f + 0.9f, sin(dtAcc)*0.1f + 0.9f, cos(dtAcc)*0.2f + 0.8f);

		C3DLight* pointLight = static_cast<C3DLight*>(pointLightNode->findNode("pointLight"));
		C3DPointLight* pLightComponent = static_cast<C3DPointLight*>(pointLight->getComponent());
		if(pLightComponent)
		{
			pLightComponent->setColor(pointLightColor);
		}

		if(_sm)
		{
			std::vector<C3DNode*> vNodes = _sm->getChildrenList();
			for(std::vector<C3DNode*>::iterator i = vNodes.begin(); i!=vNodes.end(); ++i)
			{
				if ((*i)->getType() != C3DNode::NodeType_Model)
					continue;

				C3DMaterial* pMaterial = ((C3DModelNode*)(*i))->getModel()->getMaterial();
				if(pMaterial)
				{
					C3DVector3 vTrans = pointLight->getTranslationWorld();
					pMaterial->getParameter("u_ligthPos")->setValue(vTrans);

					C3DMatrix matWorld = pointLight->getWorldMatrix();
					matWorld.invert();
					pMaterial->getParameter("u_lightInvWorld")->setValue(matWorld);
				}
			}
		}
	}
}

void MaterialTestLayer::draw3D()
{
    C3DLayer::draw3D();
}

void MaterialTestLayer::setUpScene()
{
    _sm = static_cast<cocos3d::C3DStaticObj*>(C3DRenderNodeManager::getInstance()->getResource("demores/materialtest/1.ckb"));

    _sm->setMaterial("demores/materialtest/1_diffuse.material");
    _sm->translate(0, 0, 0);
    _sm->scale(50, 50, 50);

    _scene->addChild(_sm);
    _sm->retain();

    //create fish
	_fish = static_cast<cocos3d::C3DSprite*>(C3DRenderNodeManager::getInstance()->getResource("demores/shayunew/shayu.ckb"));
	_fish->setId("shayu");
	_fish->setScale(2.0f);
	_fish->addAnimationClip("idle",0,600,0,1.0f);
	_fish->playAnimationClip("idle");
	_fish->setPosition(15.0f, 10.0f, 0.0f);
	//_scene->addChild(_fish);
    _fish->retain();
}

void MaterialTestLayer::setUpCamera()
{
    C3DCamera* camera = C3DCamera::createPerspective(45, 0.75f, 1, 1000);
    camera->lookAt(C3DVector3(0,50,100), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));

    _scene->addChild(camera);
    _scene->setActiveCamera(0);
}

void MaterialTestLayer::setUpLight()
{
    C3DVector3 color(1.0f, 1.0f, 1.0f);
    C3DLight* light = C3DLight::create("main light");
    light->setPosition(0,0,20);
    light->lookAt(C3DVector3(40, 60, 20), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
    light->setComponent(C3DDirectionalLight::create(color));

    _scene->addChild(light);

	C3DVector3 pointLightColor(1.0f, 1.0f, 1.0f);
	C3DLight* pointLight = C3DLight::create("pointLight");
	pointLight->setComponent(C3DPointLight::create(pointLightColor, 100));
	pointLight->setLightEnable(false);

	std::string strPointLightUrl = "demores/ball/ball.ckb";
	cocos3d::C3DStaticObj* pointLightModel = static_cast<cocos3d::C3DStaticObj*>(C3DRenderNodeManager::getInstance()->getResource(strPointLightUrl));
	pointLightModel->setId("PTBall");
	pointLightModel->setScale(3.0f);

	_scene->addChild(pointLightModel);
	pointLightModel->addChild(pointLight);

	pointLightModel->setMaterial("demores/ball/projected_texture_ball.material");
	pointLightModel->setVisible(false);
}

void MaterialTestLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
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

void MaterialTestLayer::menuCallback( CCObject * pSender )
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;

    C3DRenderNode* sm = (C3DRenderNode*)_scene->findNode("1");
    C3DSprite* fish = (C3DSprite*)_scene->findNode("shayu");

    if (nIdx == REFLECTIVE)
    {
        if (sm)
            _scene->removeChild(sm);
        if (fish == NULL)
            _scene->addChild(_fish);
    }
    else
    {
        if (sm == NULL)
            _scene->addChild(_sm);
        if (fish)
            _scene->removeChild(fish);
    }

	C3DNode*	pointLightModelNode = _scene->findNode("PTBall");
	C3DLight*	pointLight = static_cast<C3DLight*>(pointLightModelNode->findNode("pointLight"));

	if(pointLightModelNode)
		pointLightModelNode->setVisible(false);

	if(pointLight)
		pointLight->setLightEnable(false);

    switch (nIdx)
    {
        case NOLIGHT:
            _sm->setMaterial("demores/materialtest/1_nolight.material");
            break;
        case DIFFUSE:
            _sm->setMaterial("demores/materialtest/1_diffuse.material");
            break;
        case SPECLUAR:
            _sm->setMaterial("demores/materialtest/1_specular.material");
            break;
        case TRANSMISSION:
            _sm->setMaterial("demores/materialtest/1_transmission.material");
            break;
        case NORMALMAP:
            _sm->setMaterial("demores/materialtest/1_normalmap.material");
			break;
		case WOOD:
			_sm->setMaterial("demores/materialtest/1_wood.material");
            break;
		case TOON:
			_sm->setMaterial("demores/materialtest/1_toon.material");
			break;
		case PROJECTED_TEXTURE:
			_sm->setMaterial("demores/materialtest/1_projected_texture.material");
			if(pointLightModelNode)
				pointLightModelNode->setVisible(true);
			if(pointLight)
				pointLight->setLightEnable(true);
			break;
    }
}

CCLayer* MaterialTestLayer::createUILayer()
{
    CCLayer* layer = CCLayer::create();

    CCMenu* pItemMenu = CCMenu::create();

    for (int i = 0; i < MATERIAL_NUM; ++i)
    {

        CCLabelTTF* label = CCLabelTTF::create(materialTypes[i], "Arial", 20);
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, CC_CALLBACK_1(MaterialTestLayer::menuCallback,this));

        pItemMenu->addChild(pMenuItem, i + 10000);
        pMenuItem->setPosition( ccp( 20 + VisibleRect::left().x + label->getContentSize().width / 2, (VisibleRect::top().y - (i + 1) * 24) ));
    }

    pItemMenu->setPosition(0, 0);
    layer->addChild(pItemMenu, 1000);

    return layer;
}