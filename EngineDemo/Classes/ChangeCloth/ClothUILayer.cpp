#include "ClothUILayer.h"
#include "ChangeClothesTestLayer.h"
#include "CCNode.h"
#include "VisibleRect.h"

static CCMenu* pMenu1;

// on "init" you need to initialize your instance
bool ClothUILayer::init()
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
	CCMenuItemImage *pItem1 = CCMenuItemImage::create("cap_normal.png",		 "cap_selected.png",		CC_CALLBACK_1(ClothUILayer::menuCallback_changeCap,this) );
	CCMenuItemImage *pItem2 = CCMenuItemImage::create("hair_normal.png",	 "hair_selected.png",		CC_CALLBACK_1(ClothUILayer::menuCallback_changeHair,this) );
	CCMenuItemImage *pItem3 = CCMenuItemImage::create("glasses_normal.png",	 "glasses_selected.png",	CC_CALLBACK_1(ClothUILayer::menuCallback_changeGlasses,this) );
	CCMenuItemImage *pItem4 = CCMenuItemImage::create("coat_normal.png",	 "coat_selected.png",		CC_CALLBACK_1(ClothUILayer::menuCallback_changeUpBody,this) );
	CCMenuItemImage *pItem5 = CCMenuItemImage::create("trousers_normal.png", "trousers_selected.png",	CC_CALLBACK_1(ClothUILayer::menuCallback_changeBottomBody,this) );
	CCMenuItemImage *pItem6 = CCMenuItemImage::create("shoe_normal.png",	 "shoe_selected.png",		CC_CALLBACK_1(ClothUILayer::menuCallback_changeShoot,this) );
	CCMenuItemImage *pItem7 = CCMenuItemImage::create("nose_normal.png",	 "nose_selected.png",		CC_CALLBACK_1(ClothUILayer::menuCallback_changeNose,this) );
	CCMenuItemImage *pItem8 = CCMenuItemImage::create("eyemorph_normal.png", "eyemorph_selected.png",	CC_CALLBACK_1(ClothUILayer::menuCallback_changeEye,this) );

    pItem1->setPosition( ccp(VisibleRect::left().x+50, VisibleRect::bottom().y+pItem1->getContentSize().height ) );
	pItem2->setPosition( ccp(VisibleRect::left().x+50, VisibleRect::bottom().y+pItem1->getContentSize().height *2 ) );
	pItem3->setPosition( ccp(VisibleRect::left().x+50, VisibleRect::bottom().y+pItem1->getContentSize().height*3 ) );
	pItem4->setPosition( ccp(VisibleRect::left().x+50, VisibleRect::bottom().y+pItem1->getContentSize().height *4 ) );
	pItem5->setPosition( ccp(VisibleRect::left().x+50, VisibleRect::bottom().y+pItem1->getContentSize().height *5 ) );
	pItem6->setPosition( ccp(VisibleRect::left().x+50, VisibleRect::bottom().y+pItem1->getContentSize().height *6 ) );
	pItem7->setPosition( ccp(VisibleRect::left().x+50, VisibleRect::bottom().y+pItem1->getContentSize().height *7 ) );
	pItem8->setPosition( ccp(VisibleRect::left().x+50, VisibleRect::bottom().y+pItem1->getContentSize().height *8 ) );

    // create menu, it's an autorelease object
    pMenu1 = CCMenu::create(pItem1,pItem2,pItem3,pItem4,pItem5,pItem6,pItem7,pItem8,NULL);
    pMenu1->setPosition( CCPointZero );
    this->addChild(pMenu1, 10);

    return true;
}

void ClothUILayer::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void ClothUILayer::menuCallback_changeCap(CCObject* pSender)
{
	if( ChangeClothesTestLayer::getInstance()==NULL )
		return;
	ChangeClothesTestLayer::getInstance()->changePart("cap");
}
void ClothUILayer::menuCallback_changeHair(CCObject* pSender)
{
	if( ChangeClothesTestLayer::getInstance()==NULL )
		return;
	ChangeClothesTestLayer::getInstance()->changePart("hair");
}
void ClothUILayer::menuCallback_changeGlasses(CCObject* pSender)
{
	if( ChangeClothesTestLayer::getInstance()==NULL )
		return;
	ChangeClothesTestLayer::getInstance()->changePart("glasses");
}
void ClothUILayer::menuCallback_changeUpBody(CCObject* pSender)
{
	if( ChangeClothesTestLayer::getInstance()==NULL )
		return;
	ChangeClothesTestLayer::getInstance()->changePart("shangshen");
}
void ClothUILayer::menuCallback_changeBottomBody(CCObject* pSender)
{
	if( ChangeClothesTestLayer::getInstance()==NULL )
		return;
	ChangeClothesTestLayer::getInstance()->changePart("xiashen");
}
void ClothUILayer::menuCallback_changeShoot(CCObject* pSender)
{
	if( ChangeClothesTestLayer::getInstance()==NULL )
		return;
	ChangeClothesTestLayer::getInstance()->changePart("shoes");
}

static float g_eyeWeight = 0.0f;
static float g_noseWeight = 0.0f;

void ClothUILayer::menuCallback_changeNose(CCObject* pSender)
{
	if( ChangeClothesTestLayer::getInstance()==NULL )
		return;
	g_noseWeight += 0.1f;
	if(g_noseWeight > 1.0f)
		g_noseWeight = 0.0f;
	ChangeClothesTestLayer::getInstance()->changeMorph("nose",g_noseWeight);
}
void ClothUILayer::menuCallback_changeEye(CCObject* pSender)
{
	if( ChangeClothesTestLayer::getInstance()==NULL )
		return;
	g_eyeWeight += 0.1f;
	if(g_eyeWeight > 1.0f)
		g_eyeWeight = 0.0f;
	ChangeClothesTestLayer::getInstance()->changeMorph("eye",g_eyeWeight);
}