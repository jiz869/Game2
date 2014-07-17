/*
 * MultiPlayScene.cpp
 *
 *  Created on: Jun 4, 2014
 *      Author: mli
 */

#ifdef MULTIPLAY

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
    data = GameController::getGameController()->getPlaySceneData(userData->maxLevel);
    unscheduleUpdate();

    initCityObj();

    CCLog("initCityObj");

    infoLabel = CCLabelTTF::create("", FONT, 32);
    infoLabel->setColor( ccc3(168, 0, 0) );
    infoLabel->setPosition(ccp(winSize.width/2 , winSize.height - 50));
    addChild(infoLabel);

    order = ORDER_MAX;
    syncCount = 0;
    roomId=NULL_ROOM_ID;
    controlMenu = NULL;
}

void MultiPlayScene::onConnectDone(int res)
{
	CCString * str = CCString::createWithFormat("onConnectDone %d", res);
    CCLOG("%s", str->getCString());
    infoLabel->setString(str->getCString());
    if (res==AppWarp::ResultCode::success || res==AppWarp::ResultCode::success_recovered)
    {
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        userData->pvpMode = CONNECTED;
        warpClientRef->joinRoomInUserRange(1,1,true);
#ifdef USE_UDP
        warpClientRef->initUDP();
#endif
    }
    else
    {
        connectionFailed("connection failed");
    }
}

void MultiPlayScene::onInitUDPDone(int result){
    CCString * str = CCString::createWithFormat("onInitUDPDone %d", result);
    CCLOG("%s", str->getCString());
    infoLabel->setString(str->getCString());

    if(result != AppWarp::ResultCode::success){
        connectionFailed(str->getCString());
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

    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    if (revent.result==0)
    {
        if(userData->pvpMode == CONNECTED){
            userData->pvpMode = JOIN_ROOM1;
        }else if(userData->pvpMode == CREATE_ROOM){
            userData->pvpMode = JOIN_ROOM2;
        }

        roomId = revent.roomId;
        warpClientRef->subscribeRoom(roomId);
    }else if(userData->pvpMode == CONNECTED){
        CCLOG("createRoom");
        warpClientRef->subscribeLobby();
        warpClientRef->createRoom("play_room", userName, 2);
    }
    else{
        connectionFailed("join room failed");
    }
}

void MultiPlayScene::onRoomCreated(AppWarp::room rData){
    CCString * str = CCString::createWithFormat("onRoomCreated %d", rData.result);
    CCLOG("%s", str->getCString());
    infoLabel->setString(str->getCString());

    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->unsubscribeLobby();
    warpClientRef->joinRoom(rData.roomId);
    userData->pvpMode == CREATE_ROOM;
}

void MultiPlayScene::onSubscribeRoomDone(AppWarp::room revent)
{
    CCString * str = CCString::createWithFormat("onSubscribeRoomDone %d", revent.result);
    CCLOG("%s", str->getCString());
    infoLabel->setString(str->getCString());
    if (revent.result==0){
        userData->pvpMode == SUBSCRIBE_ROOM;
    }else{
        connectionFailed("subscribe room failed");
    }
}

void MultiPlayScene::connectionFailed(const char * message){
    if(controlMenu) controlMenu->hideMenu();
	infoLabel->setString(message);
    runAction(CCSequence::create( CCDelayTime::create(2.5) ,CCCallFunc::create(this, callfunc_selector(MultiPlayScene::returnOnConnectionFailed)), NULL ));
}

void MultiPlayScene::returnOnConnectionFailed(){
    gameOver();
    CCDirector::sharedDirector()->replaceScene(StartMenuScene::scene());
}

void MultiPlayScene::connectToAppWarp(){
    AppWarp::Client *warpClientRef;

    userName = getUserName();
    infoLabel->setString("connectToAppWarp");
    CCLOG("connectToAppWarp");
    AppWarp::Client::initialize(APPWARP_APP_KEY,APPWARP_SECRET_KEY);
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);
    warpClientRef->setNotificationListener(this);
    warpClientRef->setRoomRequestListener(this);
    warpClientRef->setZoneRequestListener(this);
    warpClientRef->setChatRequestListener(this);
    warpClientRef->setGeo("us");
    warpClientRef->connect(userName);
}

string MultiPlayScene::getUserName()
{
	std::string charStr;
	srand (time(NULL));

	for (int i = 0; i < 10; ++i) {
		charStr += (char)(65+(rand() % (26)));
	}

	return charStr;
}

void MultiPlayScene::update(float dt){
    PlayScene::update(dt);
    enemy->step(dt);
}

void MultiPlayScene::onUserJoinedRoom(AppWarp::room event, string username){
    if(username == userName) return;
    if(event.roomId != roomId) return;

    CCLOG("onUserJoinedRoom %s", username.c_str());
    infoLabel->setString(username.c_str());

    enemyName = username;
    order = FIRST;
    userData->order = order;
    scheduleOnce(schedule_selector(MultiPlayScene::prepareToStart) , 0);
    latency = getCurrentTime();
    sendSync();
}

void MultiPlayScene::onUserLeftRoom(AppWarp::room rData, std::string user){
    if(user == userName) return;
    if(rData.roomId != roomId) return;

    CCLOG("onUserLeftRoom");
    if(controlMenu && ((MultiPlayControlMenu *)controlMenu)->isEnemyOver == true) return;
    CCString * str = CCString::createWithFormat("user %s left room", user.c_str());
    connectionFailed(str->getCString());
}

void MultiPlayScene::sendStart(){
    sendUpdate("start" , false);
}

void MultiPlayScene::sendSync(){
    sendUpdate("sync" , false);
}

void MultiPlayScene::sendScore(){
    sendUpdate("score" , true);
}

void MultiPlayScene::sendOver(){
    sendUpdate("over" , false);
}

void MultiPlayScene::gameOver(){
    CCLOG("gameOver %s", roomId.c_str());
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setConnectionRequestListener(NULL);
    warpClientRef->setNotificationListener(NULL);
    warpClientRef->setRoomRequestListener(NULL);
    warpClientRef->setZoneRequestListener(NULL);
    warpClientRef->setChatRequestListener(NULL);

    if(roomId != NULL_ROOM_ID){
        warpClientRef->leaveRoom(roomId);
        warpClientRef->deleteRoom(roomId);
    }
    warpClientRef->disconnect();
}

//void MultiPlayScene::sendPlayPos(){
//    CCPoint pos = player->getPosition();
//    CCString * str = CCString::createWithFormat("{%f,%f}", pos.x , pos.y);
//    AppWarp::Client *warpClientRef;
//    warpClientRef = AppWarp::Client::getInstance();
//    sendUpdate(str->getCString());
//}

void MultiPlayScene::sendUp(){
    sendUpdate("up" , true);
}

void MultiPlayScene::sendDown(){
    sendUpdate("down" , true);
}

void MultiPlayScene::sendWait(){
    sendUpdate("wait" , true);
}

void MultiPlayScene::sendHit(){
    sendUpdate("hit" , true);
}

void MultiPlayScene::sendUpdate(const char * command , bool useUdp){
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();

#ifdef USE_UDP
    if(!useUdp){
        warpClientRef->sendPrivateChat(enemyName , command);
        return;
    }
    char * cmd = strdup(userName.c_str());
    strcat(cmd , command);
    warpClientRef->sendUdpUpdate((byte *)cmd , strlen(cmd) + 1);
#else
    warpClientRef->sendPrivateChat(enemyName , command);
#endif
}

void MultiPlayScene::onChatReceived(AppWarp::chat chatevent){
    onPrivateChatReceived(chatevent.sender , chatevent.chat);
}

void MultiPlayScene::onUpdatePeersReceived(AppWarp::byte update[], int len, bool isUDP){
    char * command = (char *)update;

    onPrivateChatReceived(string(command , 10) , string(&command[10]));
}

void MultiPlayScene::prepareToStart(){

    userData->pvpMode = PREPARE_PLAY;

    infoLabel->setZOrder(100000);

    infoLabel->setString("Preparing to start");

    initBoundary();

    CCLog("initBoundary");

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

    if(sender == userName) return;

    CCLOG("%s , %s , %lu", sender.c_str() , message.c_str() , getCurrentTime());

    if(message == "sync"){
        if(order == ORDER_MAX || order ==SECOND){
            if(syncCount == 0){
                enemyName=sender;
                order = SECOND;
                userData->order=order;
                scheduleOnce(schedule_selector(MultiPlayScene::prepareToStart) , 0);
                latency = getCurrentTime();
            }
            syncCount++;
            if(syncCount > SYNC_TIMES) return;
            sendSync();
            if(syncCount == SYNC_TIMES){
                latency = (getCurrentTime()-latency)/(SYNC_TIMES-1);
                CCLOG("latency %lu", latency);
            }
        }else{
            syncCount++;

            if(syncCount > SYNC_TIMES) return;

            if(syncCount == SYNC_TIMES){
                latency = (getCurrentTime()-latency)/(SYNC_TIMES-1);
                CCLOG("latency %lu", latency);
                sendStart();
                scheduleOnce(schedule_selector(MultiPlayScene::startGame) , 2.0 + (float)latency/1000.0/2.0);
            }else{
                sendSync();
            }
        }
    }else if(message == "start"){
        scheduleOnce(schedule_selector(MultiPlayScene::startGame) , 2.0);
    }else if(message == "up"){
        upHandler(ENEMY);
    }else if(message == "down"){
        downHandler(ENEMY);
    }else if(message == "wait"){
        touchendHandler(ENEMY);
    }else if(message == "over"){
        ((MultiPlayControlMenu *)controlMenu)->isEnemyOver=true;
    }else if(message == "score"){
        ((MultiPlayControlMenu *)controlMenu)->updateEnemyScore(1.0);
        enemy->reset();
    }else if(message == "hit"){
        ((MultiPlayControlMenu *)controlMenu)->updateEnemyScore(-0.1);
        enemy->reset();
    }
}

void MultiPlayScene::startGame(){
    infoLabel->setString("");
    scheduleUpdate();
    PlayScene::initLanes();
    initControlMenu();
}

void MultiPlayScene::initControlMenu(){
    controlMenu = MultiPlayControlMenu::create();
    addChild(controlMenu);
}

void MultiPlayScene::upHandler(int tag){
    if (tag == PLAYER) {
        sendUp();
        player->jumpUp();
    }else{
        enemy->jumpUp();
    }
}

void MultiPlayScene::downHandler(int tag){
    if (tag == PLAYER) {
        sendDown();
        player->jumpDown();
    }else{
        enemy->jumpDown();
    }
}

void MultiPlayScene::touchendHandler(int tag){
    if (tag == PLAYER) {
        sendWait();
        player->wait();
    }else{
        enemy->wait();
    }
}

void MultiPlayScene::processContact(float dt){
	if( contact->getTag() == UPPER_BOUNDARY ) {
		if(controlMenu->doScore() == true){
            sendScore();
        }
	}
    if(player->processContact(contact) == true){
        sendHit();
    }
}

void MultiPlayScene::BeginContact(b2Contact *contact){
    PlayScene::BeginContact(contact);

    CCSprite * contactA = (CCSprite *)contact->GetFixtureA()->GetBody()->GetUserData();
	CCSprite * contactB = (CCSprite *)contact->GetFixtureB()->GetBody()->GetUserData();

    if ((contactA->getTag() == ENEMY && contactB->getTag() == LOWER_BOUNDARY) ||
        (contactB->getTag() == ENEMY && contactA->getTag() == LOWER_BOUNDARY)) {
        scheduleOnce(schedule_selector(MultiPlayScene::resetEnemy), 0);
    }
}

void MultiPlayScene::resetEnemy(){
    enemy->reset();
}

#endif
