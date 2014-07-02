/*
 * SplashScene.cpp
 *
 *  Created on: 2014年7月1日
 *      Author: Administrator
 */

#include "SplashScene.h"
#include "StartMenuScene.h"

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

    splash = CCSprite::create("splash.png");

    splash->setPosition(ccp(winSize.width/2 , winSize.height/2));

    splash->setScale(0.8);

    addChild(splash);

    scheduleOnce(schedule_selector(SplashScene::initGameController) , 0.05);

    return true;
}

void SplashScene::initGameController(){
    data = GameController::getGameController()->getUserData();
    CCDirector::sharedDirector()->replaceScene(StartMenuScene::scene());
}
