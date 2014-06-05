/*
 * MultiPlayScene.cpp
 *
 *  Created on: Jun 4, 2014
 *      Author: mli
 */

#include "MultiPlayScene.h"

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

    isFirstLaunch = true;

    connectToAppWarp();

    return true;
}

void MultiPlayScene::onConnectDone(int res)
{
    CCLog("onConnectDone %d", res);
    if (res==AppWarp::ResultCode::success)
    {
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->joinRoom(ROOM_ID);
    }
}

void MultiPlayScene::onJoinRoomDone(AppWarp::room revent)
{
    CCLog("onJoinRoomDone %d", revent.result);
}

void MultiPlayScene::connectToAppWarp(){
    AppWarp::Client *warpClientRef;
    if (isFirstLaunch)
    {
        CCLog("connectToAppWarp");
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
