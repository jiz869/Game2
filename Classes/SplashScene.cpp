/*
 * SplashScene.cpp
 *
 *  Created on: 2014年7月1日
 *      Author: Administrator
 */

#include "SplashScene.h"
#include "StartMenuScene.h"
#include "SimpleAudioEngine.h"
#include <unistd.h>
#include <pthread.h>

using namespace CocosDenshion;

extern bool gamecontrollerInited;

static void * initGameController(void * data){
    data = (void *)GameController::getGameController()->getUserData();
    return 0;
}

#define SPLASH_TIME 2 //seconds

SplashScene::SplashScene()
{
    // TODO Auto-generated constructor stub

}

SplashScene::~SplashScene()
{
    // TODO Auto-generated destructor stub
}

CCScene* SplashScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SplashScene *layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    scene->setTag(SPLASH_SCENE);

    // return the scene
    return scene;
}

bool SplashScene::init(){
    if (!CCLayer::init()) {
        return false;
    }

    winSize = CCDirector::sharedDirector()->getWinSize();
    ignoreAnchorPointForPosition(false);
    setPosition(ccp(winSize.width/2 , winSize.height/2));

    CCSprite * background = CCSprite::create("splash.png");

    CCSize size = background->getContentSize();
    background->setScaleY(winSize.height/size.height);
    background->setScaleX(winSize.width/size.width);
    background->setAnchorPoint(ccp(0,0));

    addChild(background);

//    CCAnimation * animation = CCAnimation::create();
//
//    for(int i = 0 ; i < 2 ; i++){
//    	CCString * imageName = CCString::createWithFormat("penguinside%d.png", i);
//    	CCSprite * sprite = CCSprite::create(imageName->getCString());
//    	CCRect rect = CCRectZero;
//        rect.size = sprite->boundingBox().size;
//    	CCSpriteFrame * frame = CCSpriteFrame::createWithTexture(sprite->getTexture() , rect);
//    	animation->addSpriteFrame(frame);
//    }
//
//    animation->setDelayPerUnit(0.2);
//
//    CCSprite * walkingPenguin = CCSprite::create();
//
//    walkingPenguin->setScale(0.6);
//
//    addChild(walkingPenguin);
//
//    walkingPenguin->setPosition(ccp(winSize.width*0.2 , winSize.height*0.4));
//
//    walkingPenguin->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
//
//    walkingPenguin->runAction(CCMoveTo::create(SPLASH_TIME , ccp(winSize.width*0.8 , winSize.height*0.4)));
//
//    SimpleAudioEngine::sharedEngine()->preloadEffect("splash.wav");
//
//    scheduleOnce(schedule_selector(SplashScene::splashOver) , SPLASH_TIME);

    initProgressBar();

    scheduleUpdate();

    progress = 0;
    
    // Init game controller
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprites.plist");
    pthread_t thread;
    pthread_create(&thread, NULL, initGameController, NULL);
    pthread_detach(thread);

    return true;
}

void SplashScene::initProgressBar(){
    CCSprite * emptyBar = CCSprite::create("progress_empty.png");
    emptyBar->setPosition(ccp(winSize.width/2, winSize.height * 0.4));
    addChild(emptyBar);

    bar100 = CCSprite::create("progress.png");
    bar75 = CCSprite::create("progress_75.png");
    bar50 = CCSprite::create("progress_50.png");
    bar25 = CCSprite::create("progress_25.png");

    addChild(bar100);
    addChild(bar75);
    addChild(bar50);
    addChild(bar25);

    bar100->setVisible(false);
    bar75->setVisible(false);
    bar50->setVisible(false);
    bar25->setVisible(false);

    bloodBar = CCProgressTimer::create(bar25);
    bloodBar->setType(kCCProgressTimerTypeBar);
    bloodBar->setPosition(ccp(winSize.width/2, winSize.height * 0.4));
    bloodBar->setBarChangeRate(ccp(1,0));
    bloodBar->setMidpoint(ccp(0 , 1));
    bloodBar->setPercentage(0);
    addChild(bloodBar);
}

void SplashScene::update(float dt){
    progress+=dt;
    if(progress > SPLASH_TIME){
        splashOver();
        return;
    }

    float percent = progress/SPLASH_TIME;

    if(percent < 0.5 && percent > 0.25) bloodBar->setSprite(bar50);
    else if(percent < 0.75 && percent > 0.5) bloodBar->setSprite(bar75);
    else if(percent < 1 && percent > 0.75) bloodBar->setSprite(bar100);

    bloodBar->setPercentage(percent * 100);
}

void SplashScene::splashOver(){
    unscheduleUpdate();
    while (gamecontrollerInited == false) {
        usleep(10000L);
    }
    SimpleAudioEngine::sharedEngine()->playEffect("splash.wav");
    CCDirector::sharedDirector()->replaceScene(StartMenuScene::scene());
}
