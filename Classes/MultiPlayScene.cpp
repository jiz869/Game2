/*
 * MultiPlayScene.cpp
 *
 *  Created on: Jun 4, 2014
 *      Author: mli
 */

#include "MultiPlayScene.h"
#include "StartMenuScene.h"
#include "MultiPlayControlMenu.h"

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

    unscheduleUpdate();

    infoLabel = CCLabelTTF::create("", "Verdana", 32);
    infoLabel->setColor( ccc3(54, 255, 0) );
    infoLabel->setPosition(ccp(winSize.width/2 , winSize.height/2));
    addChild(infoLabel);

    isFirstLaunch = true;
    order = ORDER_MAX;
    syncCount = 0;
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
        warpClientRef->connect("welcomelm");
    }
    else
    {
        AppWarp::Client::getInstance()->connect("welcomelm");
    }
}

void MultiPlayScene::update(float dt){
    PlayScene::update(dt);
}

void MultiPlayScene::onUserJoinedRoom(AppWarp::room event, string username){
    CCLOG("onUserJoinedRoom %s", username.c_str());
    infoLabel->setString(username.c_str());

    enemyName = username;
    order = FIRST;
    userData->order = order;
    scheduleOnce(schedule_selector(MultiPlayScene::prepareToStart) , 0);
    latency = getCurrentTime();
    sendSync();
}

void MultiPlayScene::sendStart(){
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->sendPrivateChat(enemyName, "start");
}

void MultiPlayScene::sendSync(){
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->sendPrivateChat(enemyName, "sync");
}

void MultiPlayScene::sendScore(float score){
    CCString * str = CCString::createWithFormat("{%.1f,%.1f}", 0 , score);
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->sendPrivateChat(enemyName, str->getCString());
}

void MultiPlayScene::sendOver(){
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->sendPrivateChat(enemyName, "over");
}

void MultiPlayScene::gameOver(){
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->leaveRoom(ROOM_ID);
}

void MultiPlayScene::sendPlayPos(){
    CCPoint pos = player->getPosition();
    CCString * str = CCString::createWithFormat("{%f,%f}", pos.x , pos.y);
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->sendPrivateChat(enemyName, str->getCString());
}

void MultiPlayScene::prepareToStart(){
    initBoundary();

    CCLog("initBoundary");

    initCityObj();

    CCLog("initCityObj");

    initPlayer();

    CCLog("initPlayer");
}

void MultiPlayScene::initPlayer(){
    player = new PlayerObj();
    enemy = new PlayerObj();
    if (order == FIRST) {
        addChild(player->load(PlayerObj::LEFT));
        addChild(enemy->load(PlayerObj::RIGHT));
    }else{
        addChild(player->load(PlayerObj::RIGHT));
        addChild(enemy->load(PlayerObj::LEFT));
    }
    enemy->setTag(ENEMY);
}

void MultiPlayScene::onPrivateChatReceived(std::string sender, std::string message){
    CCLOG("onPrivateChatReceived %s %s", sender.c_str() , message.c_str());
    string str = sender + string(" send ") + message;
    infoLabel->setString(str.c_str());

    if(message == "sync"){
        if(order == ORDER_MAX){
        	enemyName = sender;
            sendSync();
            if(syncCount == 0){
                latency = getCurrentTime();
            }
            syncCount++;
            if(syncCount == SYNC_TIMES){
                latency = (getCurrentTime()-latency)/(SYNC_TIMES-1);
                CCLOG("latency %lu", latency);
            }
        }else{
            syncCount++;
            sendSync();
            if(syncCount == SYNC_TIMES){
                latency = (getCurrentTime()-latency)/SYNC_TIMES;
                CCLOG("latency %lu", latency);
                sendStart();
                scheduleOnce(schedule_selector(MultiPlayScene::startGame) , 2.0 + (float)latency/1000.0);
            }
        }
    }else if(message == "start"){
        order = SECOND;
        userData->order=order;
        scheduleOnce(schedule_selector(MultiPlayScene::prepareToStart) , 0);
        scheduleOnce(schedule_selector(MultiPlayScene::startGame) , 2.0);
    }else if(message == "over"){
        ((MultiPlayControlMenu *)controlMenu)->enemyOver();
    }else{
        CCPoint point = CCPointFromString(message.c_str());
        if(point.x < 0.1){
            ((MultiPlayControlMenu *)controlMenu)->updateEnemyScore(point.y);
        }else{
            enemy->setPosition(point);
        }
    }
}

void MultiPlayScene::startGame(){
    PlayScene::initLanes();
    initControlMenu();
    schedule(schedule_selector(MultiPlayScene::sendPlayPos), latency, kCCRepeatForever, 0.05f);
}

void MultiPlayScene::initControlMenu(){
    controlMenu = MultiPlayControlMenu::create();
    addChild(controlMenu);
}

unsigned long MultiPlayScene::getCurrentTime(){
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
