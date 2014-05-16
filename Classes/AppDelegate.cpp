#include "AppDelegate.h"
//#include "HelloWorldScene.h"
#include "GameWorld.h"
#include "PlayScene.h"

USING_NS_CC;


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();

    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    //CCSize designSize = CCSizeMake(1280/2, 720/2);
    //CCSize designSize = CCSizeMake(480, 320);
    CCSize designSize = CCSizeMake(480*1.8, 320*1.8);
    std::vector<std::string> searchPaths;

    searchPaths.push_back("hd");
    //pDirector->setContentScaleFactor(480/640);

    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedHeight);

    GameController::getGameController();

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    //CCScene *pScene = HelloWorld::scene();

    //Example: Animation
    //CCScene *pScene = AnimationScene::scene();

    //CCScene *pScene =InputScene::scene();
#if 0
    CCScene *pScene =PlayScene::scene();
#else
    CCScene *pScene =GameWorld::scene();
#endif

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
