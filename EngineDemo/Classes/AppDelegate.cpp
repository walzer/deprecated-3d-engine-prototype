#include "AppDelegate.h"

#include <vector>
#include <string>

#include "MainLayer.h"
#include "AppMacros.h"
#include "cocos3d.h"
#include "C3DRenderSystem.h"

USING_NS_CC;
using namespace std;

cocos3d::C3DSprite* g_entity;
AppDelegate::AppDelegate() {
    _designSizes.push_back(Size(480, 320));
    _designSizes.push_back(Size(800, 480));
    _designSizes.push_back(Size(960, 640));
    _designSizes.push_back(Size(1024, 768));
    _designSizes.push_back(Size(2048, 1536));
}

AppDelegate::~AppDelegate()
{
}

cocos2d::Size AppDelegate::getDesignSize()
{
    CCDirector* pDirector = CCDirector::getInstance();
	CCEGLView* pEGLView = pDirector->getOpenGLView();

    Size screenSize = pEGLView->getFrameSize();
    size_t i;
    for (i = 0; i < _designSizes.size(); i++) {
        if (_designSizes[i].height >= screenSize.height)
            break;
    }
    if (i >= _designSizes.size())
        i = _designSizes.size() - 1;
    Size s = _designSizes[i];
    s.width = s.height * screenSize.width / screenSize.height;
    return s;
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
	CCDirector* pDirector = CCDirector::getInstance();
	CCEGLView* pEGLView = pDirector->getOpenGLView();
			
    if(!pEGLView) {
        pEGLView = GLView::create("Engine Demo");
        pDirector->setOpenGLView(pEGLView);
    }
	    

	CCSize frameSize = pEGLView->getFrameSize();
    // Set the design resolution
    designResolutionSize = getDesignSize();
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{
        searchPath.push_back(largeResource.directory);

        pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPath.push_back(mediumResource.directory);

        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }

    // set searching path
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = MainLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	//cocos3d::C3DRenderSystem::getInstance()->onLostDevice();
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}