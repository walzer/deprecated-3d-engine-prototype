#include "FullDemoUILayer.h"
#include "FullDemoLayer.h"
#include "base_nodes/CCNode.h"
#include "VisibleRect.h"
#include "UI3DLayer.h"
#include "MainPlayer.h"

// on "init" you need to initialize your instance
bool FullDemoUILayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object

	CCMenuItemImage *pItemNearAttack = CCMenuItemImage::create("icons/melee_attack.png","icons/melee_attack_selected.png",this,menu_selector(FullDemoUILayer::menuCallback_nearAttack) );
	CCMenuItemImage *pItemFarAttack = CCMenuItemImage::create("icons/range_attack.png","icons/range_attack_selected.png",this,menu_selector(FullDemoUILayer::menuCallback_farAttack) );
	CCMenuItemImage *pItemLightingSkill = CCMenuItemImage::create("icons/lighting.png","icons/lighting_selected.png",this,menu_selector(FullDemoUILayer::menuCallback_lightingSkill) );
	CCMenuItemImage *pItemShowMiniMap = CCMenuItemImage::create("icons/mini_map.png","icons/mini_map_selected.png",this,menu_selector(FullDemoUILayer::menuCallback_showMini) );
    CCLabelTTF* label = CCLabelTTF::create("Performance", "Arial", 20);
    CCMenuItemLabel *pItemShowPerformance = CCMenuItemLabel::create(label, this, menu_selector(FullDemoUILayer::menuCallback_showPerformance) );

	size_t placeHolderWidth = pItemNearAttack->getContentSize().width + 5;
	size_t itemHalfWidth	= pItemNearAttack->getContentSize().width/2;
	size_t menuStartX		= VisibleRect::left().x + itemHalfWidth + 20;
	pItemNearAttack->setPosition(		ccp(menuStartX + placeHolderWidth*0, VisibleRect::bottom().y + 50 ) );
	pItemFarAttack->setPosition(		ccp(menuStartX + placeHolderWidth*1, VisibleRect::bottom().y + 50 ) );
	pItemLightingSkill->setPosition(	ccp(menuStartX + placeHolderWidth*2, VisibleRect::bottom().y + 50 ) );
	pItemShowMiniMap->setPosition(		ccp(menuStartX + placeHolderWidth*3, VisibleRect::bottom().y + 50 ) );
    pItemShowPerformance->setPosition(	ccp(menuStartX + placeHolderWidth*5, VisibleRect::bottom().y + 50 ) );

	CCMenu* pMenu = CCMenu::create(pItemNearAttack,pItemFarAttack,pItemLightingSkill,pItemShowMiniMap, pItemShowPerformance,NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 10);

	CCSprite* pSprite1 = CCSprite::create("minigame.png");
	CCSize size(VisibleRect::right().x-VisibleRect::left().x,VisibleRect::top().y-VisibleRect::bottom().y);
	pSprite1->setPosition( ccp(VisibleRect::left().x+size.width *0.5f,VisibleRect::bottom().y +size.height*0.5f ));
	pSprite1->setOpacity(50);

	this->addChild(pSprite1,20,10009);

	cocos3d::C3DVector3 base;
	base.x = pSprite1->getPosition().x;
	base.y = pSprite1->getPosition().y;
	base.z = 0;
	UI3DLayer* ui3dLayer = UI3DLayer::create();
	ui3dLayer->setBase(base);
	ui3dLayer->setUpScene();
    pSprite1->addChild(ui3dLayer);
	ui3dLayer->setPosition(0, 0);
	ui3dLayer->setSize(VisibleRect::right().x-VisibleRect::left().x,VisibleRect::top().y-VisibleRect::bottom().y);
	pSprite1->setVisible(false);

    return true;
}

void FullDemoUILayer::menuCallback_showPerformance(CCObject* pSender)
{
    if( FullDemoLayer::getInstance()==NULL )
        return;
    FullDemoLayer::getInstance()->switchProfileShow();
}
void FullDemoUILayer::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void FullDemoUILayer::menuCallback_nearAttack(CCObject* pSender)
{
	if( FullDemoLayer::getInstance()==NULL )
		return;
	FullDemoLayer::getInstance()->getMainPlayer()->setAttackType(AT_Melee);
}

void FullDemoUILayer::menuCallback_farAttack(CCObject* pSender)
{
	if( FullDemoLayer::getInstance()==NULL )
		return;
	FullDemoLayer::getInstance()->getMainPlayer()->setAttackType(AT_Range);
}

void FullDemoUILayer::menuCallback_lightingSkill(CCObject* pSender)
{
	if( FullDemoLayer::getInstance()==NULL )
		return;
	FullDemoLayer::getInstance()->getMainPlayer()->setAttackType(AT_Lighting);
}

void FullDemoUILayer::menuCallback_showMini(CCObject* pSender)
{
	if( FullDemoLayer::getInstance()==NULL )
		return;

	CCNode* sprite = this->getChildByTag(10009);
	if(sprite != NULL)
	{
		if(sprite->isVisible())
			sprite->setVisible(false);
		else
			sprite->setVisible(true);
	}
}