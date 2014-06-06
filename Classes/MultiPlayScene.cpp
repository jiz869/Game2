/*
 * MultiPlayScene.cpp
 *
 *  Created on: Jun 4, 2014
 *      Author: mli
 */

#include "MultiPlayScene.h"
#include "StartMenuScene.h"

MultiPlayScene::MultiPlayScene()
{
    // TODO Auto-generated constructor stub

}

MultiPlayScene::~MultiPlayScene()
{
    // TODO Auto-generated destructor stub
}

CCScene* MultiPlayScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MultiPlayScene *layer = MultiPlayScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool MultiPlayScene::init(){
    if (!CCLayerColor::initWithColor(ccc4(0x9f,0x9f,0x5f,255))) {
        return false;
    }
    
    initMisc();
    
    CCLog("initMisc");

    connectToAppWarp();

    return true;
}

void MultiPlayScene::initMisc(){
    PlayScene::initMisc();
    
    infoLabel = CCLabelTTF::create("", "Verdana", 32);
    infoLabel->setColor( ccc3(54, 255, 0) );
    infoLabel->setPosition(ccp(winSize.width/2 , winSize.height/2));
    addChild(infoLabel);
    
    isFirstLaunch = true;
}

void MultiPlayScene::onConnectDone(int res)
{
	CCString * str = CCString::createWithFormat("onConnectDone %d", res);
    CCLOG("%s", str->getCString());
    infoLabel->setString(str->getCString());
    if (res==AppWarp::ResultCode::success)
    {
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->joinRoom(ROOM_ID);
    }
    else if (res==AppWarp::ResultCode::connection_error_recoverable)
    {
        schedule(schedule_selector(MultiPlayScene::recover), 5.0f);
    }
    else
    {
        unschedule(schedule_selector(MultiPlayScene::recover));
        connectionFailed();
    }
}

void MultiPlayScene::recover(){
    AppWarp::Client::getInstance()->recoverConnection();
}

void MultiPlayScene::onJoinRoomDone(AppWarp::room revent)
{
	CCString * str = CCString::createWithFormat("onJoinRoomDone %d", revent.result);
    CCLOG("%s", str->getCString());
    infoLabel->setString(str->getCString());
    if (revent.result==0)
    {
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->subscribeRoom(ROOM_ID);
    }else{
        connectionFailed();
    }
}

void MultiPlayScene::onSubscribeRoomDone(AppWarp::room revent)
{
    CCString * str = CCString::createWithFormat("onSubscribeRoomDone %d", revent.result);
    CCLOG("%s", str->getCString());
    infoLabel->setString(str->getCString());
    if (revent.result==0)
    {
    }
}

void MultiPlayScene::connectionFailed(){
	infoLabel->setString("Connection failed !!! Please retry later");
    runAction(CCSequence::create( CCDelayTime::create(2.5) ,CCCallFunc::create(this, callfunc_selector(MultiPlayScene::returnOnConnectionFailed)), NULL ));
}

void MultiPlayScene::returnOnConnectionFailed(){
    CCDirector::sharedDirector()->replaceScene(StartMenuScene::scene());
}

void MultiPlayScene::connectToAppWarp(){
    AppWarp::Client *warpClientRef;
    if (isFirstLaunch)
    {
        infoLabel->setString("connectToAppWarp");
        CCLOG("connectToAppWarp");
        isFirstLaunch = !isFirstLaunch;
        AppWarp::Client::initialize(APPWARP_APP_KEY,APPWARP_SECRET_KEY);
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->setRecoveryAllowance(60);
        warpClientRef->setConnectionRequestListener(this);
        warpClientRef->setNotificationListener(this);
        warpClientRef->setRoomRequestListener(this);
        warpClientRef->setZoneRequestListener(this);
        warpClientRef->setChatRequestListener(this);
        warpClientRef->connect("limin");
    }
    else
    {
        AppWarp::Client::getInstance()->connect("limin");
    }
}

void MultiPlayScene::update(float dt){

}

void MultiPlayScene::onUserJoinedRoom(AppWarp::room event, string username){
    CCLOG("onUserJoinedRoom %s", username.c_str());
    infoLabel->setString(username.c_str());
    
    order = FIRST;
    prepareToStart();
}

void MultiPlayScene::prepareToStart(){
    initBoundary();
    
    CCLog("initBoundary");
    
    initCityObj();
    
    CCLog("initCityObj");
    
    initPlayer();
    
    CCLog("initPlayer");
    
    initControlMenu();
    
    CCLog("initMenu");
}

void MultiPlayScene::initPlayer(){
    player = new PlayerObj();
    enemy = new PlayerObj();
    if (order == FIRST) {
        player->resetPos = ccp(winSize.width/2-30 , winSize.height/2+2);
        enemy->resetPos = ccp(winSize.width/2+30 , winSize.height/2+2);
    }else{
        enemy->resetPos = ccp(winSize.width/2-30 , winSize.height/2+2);
        player->resetPos = ccp(winSize.width/2+30 , winSize.height/2+2);
    }
	addChild(player->load());
    addChild(enemy->load());
}
