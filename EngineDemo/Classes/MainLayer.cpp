#include "MainLayer.h"
#include "VisibleRect.h"
#include "tests.h"
#include "C3DLayer.h"
#include "MaterialTest/MaterialTestLayer.h"
#include "TestLayer.h"
#include "ColorTest/ColorTestLayer.h"
#include "ShadowTest/ShadowTestLayer.h"
#include "AnimLightTest/AnimLightTestLayer.h"
#include "TunnelTest/TunnelTestLayer.h"
#include "PostEffect/PostEffectTestLayer.h"
#include "ParticleTest/ParticleTestLayer.h"
#include "ChangeCloth/ChangeClothesTestLayer.h"
#include "FullDemo/FullDemoLayer.h"
#include "PostEffect/PostEffectTestLayer.h"
#include "FullDemo/UI3DLayer.h"
#include "PerformanceTest/PerformanceTestLayer.h"
#include "C3DRenderSystem.h"

#define LINE_SPACE          50

using namespace cocos3d;

static CCPoint s_tCurPos = CCPointZero;
MainLayer* MainLayer::_instance = NULL;

bool MainLayer::init()
{
    m_tBeginPos = CCPointZero;
    if ( !CCLayer::init() )
    {
        return false;
    }

    // add close menu
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("close_normal.png", "close_selected.png",  CC_CALLBACK_1(MainLayer::closeCallback,this) );
    CCMenu* pMenu =CCMenu::create(pCloseItem, NULL);

    pMenu->setPosition( CCPointZero );
    pCloseItem->setPosition(ccp( VisibleRect::right().x - 40, VisibleRect::top().y - 30));

    // add menu items for tests
    m_pItemMenu = CCMenu::create();
    for (int i = 0; i < TESTS_COUNT; ++i)
    {
        // #if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
        //         CCLabelBMFont* label = CCLabelBMFont::create(g_aTestNames[i].c_str(),  "fonts/arial16.fnt");
        // #else
        CCLabelTTF* label = CCLabelTTF::create(g_aTestNames[i].c_str(), "Arial", 60);
        // #endif
        CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, CC_CALLBACK_1(MainLayer::menuCallback,this));

        m_pItemMenu->addChild(pMenuItem, i + 10000);
        pMenuItem->setPosition( ccp( VisibleRect::center().x, (VisibleRect::top().y - (i + 1) * LINE_SPACE) ));
    }

    m_pItemMenu->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, (TESTS_COUNT + 1) * (LINE_SPACE)));
    m_pItemMenu->setPosition(s_tCurPos);
    addChild(m_pItemMenu);

    setTouchEnabled(true);

    addChild(pMenu, 1);

    CCLabelTTF* label = CCLabelTTF::create("MainMenu", "Arial", 32);
    //#endif
    CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, CC_CALLBACK_1(MainLayer::menuBackCallback,this));

    m_pMenuBack =CCMenu::create(pMenuItem, NULL);

    m_pMenuBack->setPosition( CCPointZero );
    pMenuItem->setPosition( ccp( VisibleRect::right().x - 80, VisibleRect::bottom().y + 30) );

    addChild(m_pMenuBack, 1);

    m_pMenuBack->setEnabled(false);
    m_pMenuBack->setVisible(false);
    m_pLayer3D = NULL;

	_instance = this;
    return true;
}

cocos2d::CCScene* MainLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MainLayer *layer = MainLayer::create();
    //layer->init();
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void MainLayer::closeCallback(CCObject* pSender)
{
    //CCDirector::sharedDirector()->end();
	cocos3d::C3DRenderSystem::getInstance()->reload();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainLayer::menuCallback(CCObject * pSender)
{
    if (m_pLayer3D)
        return;

    //CCDirector::sharedDirector()->setDisplayStats(false);
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;

    TestLayer* layer = NULL;

    switch (nIdx)
    {
    case TEST_MATERIALS:
        layer = MaterialTestLayer::create();
        break;
    case TEST_COLOR:
        layer = ColorTestLayer::create();
        break;
    case TEST_SHADOW:
        layer = ShadowTestLayer::create();
        break;
    case TEST_ANIMLIGHT:
        layer = AnimLightTestLayer::create();
        break;
    case TEST_TUNNEL:
        layer = TunnelTestLayer::create();
        break;
	case TEST_POSTEFFECT:
		layer = PostEffectTestLayer::create();
		break;
    case TEST_PARTICLE:
        layer = ParticleTestLayer::create();
        break;
	case TEST_CHANGECLOTHES:
		layer = ChangeClothesTestLayer::create();
		break;
	case TEST_FULLDEMO:
		layer = FullDemoLayer::create();
		break;
	case TEST_PERFORMANCE:
		layer = PerformanceTestLayer::create();
		break;
//#ifdef WIN32
//	case TEST_PHYSICS:
//		layer = PhysicsTestLayer::create();
//		break;
//#endif // WIN32
    }

    if (!layer)
    {
        return;
    }

    m_pUILayer = layer->createUILayer();
    addChild(layer);
    addChild(m_pUILayer);

    //m_pUILayer->addChild(layer);

   // CCScene* scene = CCScene::create();
   // scene->addChild(layer);
   // scene->addChild(m_pUILayer);

     //  CCDirector::sharedDirector()->replaceScene(scene);

    m_pItemMenu->setEnabled(false);
    m_pItemMenu->setVisible(false);

    m_pMenuBack->setEnabled(true);
    m_pMenuBack->setVisible(true);

    setTouchEnabled(false);
    // create the test scene and run it
    //CCDirector::sharedDirector()->setDisplayStats(false);

    m_pLayer3D = layer;
}

void MainLayer::onTouchesBegan( const std::vector<Touch*>& touches, Event *unused_event )
{
    std::vector<Touch*>::const_iterator iter = touches.begin();
    Touch* touch = (Touch*)(*iter);

    m_tBeginPos = touch->getLocation();
}


void MainLayer::onTouchesMoved( const std::vector<Touch*>& touches, Event *unused_event )
{
	std::vector<Touch*>::const_iterator iter = touches.begin();
    Touch* touch = (Touch*)(*iter);

    CCPoint touchLocation = touch->getLocation();
    float nMoveY = touchLocation.y - m_tBeginPos.y;

    CCPoint curPos  = m_pItemMenu->getPosition();
    CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);

    if (nextPos.y < 0.0f)
    {
        m_pItemMenu->setPosition(CCPointZero);
        return;
    }

    if (nextPos.y > ((TESTS_COUNT + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height))
    {
        m_pItemMenu->setPosition(ccp(0, ((TESTS_COUNT + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height)));
        return;
    }

    m_pItemMenu->setPosition(nextPos);
    m_tBeginPos = touchLocation;
    s_tCurPos   = nextPos;
}

void MainLayer::menuBackCallback(CCObject * pSender)
{
    ccGLUseProgram(0);

    //CCScene* scene = MainLayer::scene();

    //CCDirector::sharedDirector()->replaceScene(scene);

   // return;

    if (m_pLayer3D)
    {
      //  m_pLayer3D->endRender();
        removeChild(m_pLayer3D);
        m_pLayer3D = NULL;
    }
    if (m_pUILayer)
    {
        removeChild(m_pUILayer);
        m_pUILayer = NULL;
   }

    m_pItemMenu->setEnabled(true);
    m_pItemMenu->setVisible(true);

    m_pMenuBack->setEnabled(false);
    m_pMenuBack->setVisible(false);

    setTouchEnabled(true);
}