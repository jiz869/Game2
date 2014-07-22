/*
 * SplashScene.cpp
 *
 *  Created on: 2014年7月1日
 *      Author: Administrator
 */

#include "SplashScene.h"
#include "StartMenuScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

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

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    ignoreAnchorPointForPosition(false);
    setPosition(ccp(winSize.width/2 , winSize.height/2));

    CCSprite * background = CCSprite::create("background.png");

    CCSize size = background->getContentSize();
    background->setScaleY(winSize.height/size.height);
    background->setScaleX(winSize.width/size.width);
    background->setAnchorPoint(ccp(0,0));

    addChild(background);

    background = CCSprite::create("background_main.png");

    background->setScaleY(winSize.height/size.height);
    background->setScaleX(winSize.width/size.width);
    background->setAnchorPoint(ccp(0,0));

    addChild(background);

    CCAnimation * animation = CCAnimation::create();

    for(int i = 0 ; i < 2 ; i++){
    	CCString * imageName = CCString::createWithFormat("penguinside%d.png", i);
    	CCSprite * sprite = CCSprite::create(imageName->getCString());
    	CCRect rect = CCRectZero;
        rect.size = sprite->getContentSize();
    	CCSpriteFrame * frame = CCSpriteFrame::createWithTexture(sprite->getTexture() , rect);
    	animation->addSpriteFrame(frame);
    }

    animation->setDelayPerUnit(0.2);

    CCSprite * walkingPenguin = CCSprite::create();

    addChild(walkingPenguin);

    walkingPenguin->setPosition(ccp(winSize.width*0.2 , winSize.height*0.25));

    walkingPenguin->runAction(CCRepeatForever::create(CCAnimate::create(animation)));

    walkingPenguin->runAction(CCMoveTo::create(SPLASH_TIME , ccp(winSize.width*0.8 , winSize.height*0.25)));

    SimpleAudioEngine::sharedEngine()->preloadEffect("splash.wav");

    scheduleOnce(schedule_selector(SplashScene::splashOver) , SPLASH_TIME);
    scheduleOnce(schedule_selector(SplashScene::initGameController) , 0.4);

    return true;
}

void SplashScene::initGameController(){
    data = GameController::getGameController()->getUserData();
}

void SplashScene::splashOver(){
    SimpleAudioEngine::sharedEngine()->playEffect("splash.wav");
    CCDirector::sharedDirector()->replaceScene(StartMenuScene::scene());
}
