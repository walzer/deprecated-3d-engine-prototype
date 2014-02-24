#include "PostEffectTestLayer.h"
#include "touch_dispatcher/CCTouch.h"
#include <map>

#include "C3DViewport.h"
#include "C3DMatrix.h"

#include "C3DPostProcess.h"
#include "C3DPostEffect.h"
#include "VisibleRect.h"

#include "PEBloom.h"
#include "PEBlur.h"
#include "PEColor.h"
#include "PEGlow.h"
#include "PEOutLine.h"
#include "PEPointWarp.h"
#include "PESpaceWarp.h"
#include "PEVortex.h"
#include "PESceneChange.h"


using namespace cocos3d;

enum PostEffectType
{   
	NONE_EFFECT,
    BLUR,
    COLOR,
	OUTLINE,
	WRAP,
    BLOOM,
	GLOW,
	SCENE_CHANGE,
	VORTEX,
	POINT_WARP,
    POSTEFFECT_NUM,
};

static const char* postEffectTypes[POSTEFFECT_NUM] =
{
	"None",
    "Blur",
    "Color", 
	"Outline",
	"Wrap",
    "Bloom",
	"Glow",
	"SccenChange",
	"Vortex",
	"PointWarp"
};

PostEffectTestLayer::PostEffectTestLayer()
{

}

PostEffectTestLayer::~PostEffectTestLayer()
{

}

bool PostEffectTestLayer::init()
{
    bool r = C3DLayer::init();

    setUpCamera();
    setUpLight();
    setUpScene();   

	setUpPostEffect();

    return r;
}


void PostEffectTestLayer::onExit()
{
	C3DPostProcess* pp = _scene->getActivePostProcess();
	std::vector<std::string> names = pp->getEffectNames();
	for ( unsigned int i = 0; i < names.size(); ++i )
	{
		pp->removePostEffect( names[i] );	
	}
	_scene->removeChild( pp );
	_scene->setActivePostProcess( NULL );

}

void PostEffectTestLayer::update( float dt )
{
    long elapsedTime = (long)(dt*1000.0f+0.5f);
    C3DLayer::update(elapsedTime);
	 
    C3DLight* light = getScene()->getLight(0);

    if (light)
    {
        light->rotateAlong(C3DVector3(0, 0, 0), C3DVector3(0, 1, 0), dt * 1.2f);
    }
	C3DPostProcess* pp = _scene->getActivePostProcess();
	if ( pp != NULL )
	{
		C3DPostEffect* postEffect = pp->getActiveEffect();
		if ( postEffect != NULL )
		{
			postEffect ->update( dt );
		}
	}
}

void PostEffectTestLayer::draw()
{
    C3DLayer::draw();
}

void PostEffectTestLayer::setUpScene()
{	
    C3DStaticObj* sm = C3DStaticObj::create("1");
    
    sm->loadFromFile("scene/1/1.ckb");

    //sm->setMaterial("scene/1/1_specular.material");
	sm->setMaterial("scene/1/1_no_glow.material");
    sm->translate(0, 0, 0);
    //sm->rotateX(0.8f);
    sm->scale(50, 50, 50);   
    _scene->addChild(sm);


	C3DSprite* fish = C3DSprite::create("haigui");

    fish->loadFromFile("2.5D/fish/haigui/haigui.ckb",true);
	fish->setMaterial("body", "2.5D/fish/haigui/haigui.material");
    fish->addAnimationClip("idle", 0, 60, 0, 1.0f);			

//    fish->addPartConfig("body",false);		

//    fish->addPart("body", "body", "2.5D/fish/denglongyu.material");		

//    fish->setPart("body",0);
//    fish->loadParts();      

    fish->playAnimationClip("idle");
    fish->setPosition(35, 10, 0);
    //fish->rotateX(0.5f);
    fish->setScale(5.0f);

    _scene->addChild(fish);

}

void PostEffectTestLayer::setUpPostEffect()
{
	float width = CCEGLView::sharedOpenGLView ()->getFrameSize().width;
	float height = CCEGLView::sharedOpenGLView ()->getFrameSize().height;
	C3DPostProcess* pp = C3DPostProcess::create("PostProcessTest",width, height);
	_scene->addChild(pp);

	_scene->setActivePostProcess( pp );

	
	pp->addPostEffect( PEColor::create( postEffectTypes[COLOR], "posteffect/postprocess_color.material", pp ) );
	pp->addPostEffect( PEBlur::create( postEffectTypes[BLUR], "posteffect/postprocess_blur.material", pp ) );
	pp->addPostEffect( PEOutLine::create( postEffectTypes[OUTLINE], "posteffect/postprocess_outline.material", pp ) );
	pp->addPostEffect( PESpaceWrap::create( postEffectTypes[WRAP], "posteffect/postprocess_spaceWrap.material", pp ) );
	pp->addPostEffect( PEBloom::create( postEffectTypes[BLOOM], "posteffect/postprocess_bloom.material", pp ) );
	pp->addPostEffect( PEGlow::create( postEffectTypes[GLOW], "posteffect/postprocess_glow.material", pp ) );
	pp->addPostEffect( PESceneChange::create( postEffectTypes[SCENE_CHANGE], "posteffect/postprocess_scenechange.material", pp ) );
	pp->addPostEffect( PEVortex::create( postEffectTypes[VORTEX], "posteffect/postprocess_vortex.material", pp ) );
	pp->addPostEffect( PEPointWarp::create( postEffectTypes[POINT_WARP], "posteffect/postprocess_point_warp.material", pp ) );


	C3DPostEffect* pe = pp->getPostEffect( postEffectTypes[VORTEX] );
	pe->setGridSize( width*0.2, height*0.2 ); 
	pe = pp->getPostEffect( postEffectTypes[POINT_WARP] );
	pe->setGridSize( width*0.2, height*0.2 ); 
}

void PostEffectTestLayer::setUpCamera()
{
    C3DCamera* camera = C3DCamera::createPerspective(45, 0.75f, 1, 1000);
    //camera->setPosition(0,0,100);
    camera->lookAt(C3DVector3(0,50,100), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
    //camera->rotateX(MATH_DEG_TO_RAD(-20.0f));

    _scene->addChild(camera);
    _scene->setActiveCamera(0);
}

void PostEffectTestLayer::setUpLight()
{
    C3DVector3 color(1.0f, 1.0f, 1.0f);
    C3DLight* light = C3DLight::create("main light");
    light->setPosition(0,0,20);
    light->lookAt(C3DVector3(40, 60, 20), C3DVector3(0, 1, 0), C3DVector3(0, 0, 0));
    light->setComponent(C3DDirectionalLight::create(color));

    _scene->addChild(light);

}

void PostEffectTestLayer::touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex)
{
    switch (evt)
    {
    case TouchEvent_PRESS:
        {
            _touched = true;
            _touchX = x;
            _touchY = y;


			float width = CCEGLView::sharedOpenGLView()->getFrameSize().width;
			float height = CCEGLView::sharedOpenGLView()->getFrameSize().height;
			postEffectTouchPress( x/width, y/height );
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


void PostEffectTestLayer::postEffectTouchPress(float x, float y)
{
	C3DPostProcess* pp = _scene->getActivePostProcess();
	if ( pp != NULL )
	{
		C3DPostEffect* postEffect = pp->getActiveEffect();
		if ( postEffect != NULL )
		{
			if ( postEffect->getName() == postEffectTypes[POINT_WARP] )
			{
				PEPointWarp* pe= (PEPointWarp*)postEffect;
				pe->addClick( x, 1-y );

			}
		}
	}

}
void PostEffectTestLayer::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
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

void PostEffectTestLayer::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
    CCTouch *pTouch;
    CCSetIterator setIter;
    for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
    {
        pTouch = (CCTouch *)(*setIter);
        CCPoint touchPoint = pTouch->getLocationInView();
        
        touchEvent(cocos3d::TouchEvent_MOVE, touchPoint.x , touchPoint.y , pTouch->getID());


    }

}

void PostEffectTestLayer::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
    CCTouch *pTouch;
    CCSetIterator setIter;
    for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
    {
        pTouch = (CCTouch *)(*setIter);
        CCPoint touchPoint = pTouch->getLocationInView();
        
        touchEvent(cocos3d::TouchEvent_RELEASE, touchPoint.x, touchPoint.y, pTouch->getID());
    }

}

void PostEffectTestLayer::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{

}

void PostEffectTestLayer::menuCallback( CCObject * pSender )
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;

    TestLayer* layer = NULL;

    C3DRenderNode* sm = (C3DRenderNode*)_scene->findNode("1");

    if (sm)
    {
		C3DPostProcess* pp = _scene->getActivePostProcess();

		pp->setActiveEffect( postEffectTypes[nIdx] );
    }
}

CCLayer* PostEffectTestLayer::createUILayer()
{
    CCLayer* layer = CCLayer::create();

    CCMenu* pItemMenu = CCMenu::create();


    for (int i = 0; i < POSTEFFECT_NUM; ++i)
    {
        // #if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
        //         CCLabelBMFont* label = CCLabelBMFont::create(g_aTestNames[i].c_str(),  "fonts/arial16.fnt");
        // #else
        CCLabelTTF* label = CCLabelTTF::create(postEffectTypes[i], "Arial", 20);
        // #endif        
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(PostEffectTestLayer::menuCallback));

        pItemMenu->addChild(pMenuItem, i + 10000);
        pMenuItem->setPosition( ccp( 20 + VisibleRect::left().x + label->getContentSize().width / 2, (VisibleRect::top().y - (i + 1) * 24) ));
    }

    //pItemMenu->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, (MATERIAL_NUM + 1) * (40)));
    pItemMenu->setPosition(0, 0);
    layer->addChild(pItemMenu, 1000);


    return layer;
}
