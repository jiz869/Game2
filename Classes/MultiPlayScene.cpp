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

    PlayScene::initMisc();

    infoLabel = CCLabelTTF::create("", "Verdana", 32);
    infoLabel->setColor( ccc3(54, 255, 0) );
    infoLabel->setPosition(ccp(winSize.width/2 , winSize.height/2));
    addChild(infoLabel);

    isFirstLaunch = true;

    connectToAppWarp();

    return true;
}

void MultiPlayScene::onConnectDone(int res)
{
	CCString * str = CCString::createWithFormat("onConnectDone %d", res);
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
    infoLabel->setString(str->getCString());
    if (revent.result==0)
    {
    }else{
        connectionFailed();
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
        isFirstLaunch = !isFirstLaunch;
        AppWarp::Client::initialize(APPWARP_APP_KEY,APPWARP_SECRET_KEY);
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->setRecoveryAllowance(60);
        warpClientRef->setConnectionRequestListener(this);
        warpClientRef->setNotificationListener(this);
        warpClientRef->setRoomRequestListener(this);
        warpClientRef->setZoneRequestListener(this);
        warpClientRef->setChatRequestListener(this);
        warpClientRef->connect("welcomelm");
    }
    else
    {
        AppWarp::Client::getInstance()->connect("welcomelm");
    }
}

void MultiPlayScene::update(float dt){

}
