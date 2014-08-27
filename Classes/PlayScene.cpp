//
//  PlayScene.cpp
//  crossRoadMain
//
//  Created by Min Li on 2014-05-15.
//
//

#include "PlayScene.h"
#include "MenuForArrowButton.h"
#include "Lane.h"
#include "ControlMenu.h"
#include "SimpleAudioEngine.h"
#include "City.h"
#include "MultiPlayControlMenu.h"

using namespace CocosDenshion;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>

extern "C"
{
	void Java_ca_welcomelm_crossRoad_crossRoad_up( JNIEnv* env, jobject thiz ){
		CCScene * scene = CCDirector::sharedDirector()->getRunningScene();
		if(scene && scene->getTag() != PLAY_SCENE) return;

		PlayScene * playScene = (PlayScene *)scene->getChildren()->objectAtIndex(0);
		playScene->upHandler(0);
	}

	void Java_ca_welcomelm_crossRoad_crossRoad_down( JNIEnv* env, jobject thiz ){
		CCScene * scene = CCDirector::sharedDirector()->getRunningScene();
		if(scene && scene->getTag() != PLAY_SCENE) return;

		PlayScene * playScene = (PlayScene *)scene->getChildren()->objectAtIndex(0);
		playScene->downHandler(0);
	}

	void Java_ca_welcomelm_crossRoad_crossRoad_end( JNIEnv* env, jobject thiz ){
		CCScene * scene = CCDirector::sharedDirector()->getRunningScene();
		if(scene && scene->getTag() != PLAY_SCENE) return;

		PlayScene * playScene = (PlayScene *)scene->getChildren()->objectAtIndex(0);
		playScene->touchendHandler(0);
	}
}
#endif

CCScene* PlayScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    PlayScene *layer = PlayScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    scene->setTag(PLAY_SCENE);

    // return the scene
    return scene;
}

PlayScene::PlayScene() : player(NULL) , contact(NULL) {
}

PlayScene::~PlayScene(){
    if (player){
        player->removeAllSpecials();
        player->removeAllBadSpecials();
    }

    b2Body *node = world->GetBodyList();

    while(node){

        b2Body *body = node;
        node = node->GetNext();
    	CCSprite *sprite = (CCSprite *)body->GetUserData();
    	if(sprite) delete (GameObj *)(sprite->getUserData());
    }

    delete world;

    CCLOG("~PlayScene");
}

bool PlayScene::init(){
    if (!CCLayerColor::initWithColor(ccc4(255,255,255,255))) {
        return false;
    }

    initMisc();

    CCLog("initMisc %d", userData->currentLevel);

    initBoundary();

    CCLog("initBoundary");

    initCityObj();

    CCLog("initCityObj");

    initLanes();

    CCLog("initLanes");

    initPlayer();

    CCLog("initPlayer");

    initControlMenu();

    CCLog("initMenu");

    return true;
}

void PlayScene::initCityObj(){
	city = City::create();
	addChild(city);
}

void PlayScene::initMisc(){
    world = new b2World(b2Vec2(0 , 0));
    world->SetContactListener(this);

    userData = GameController::getGameController()->getUserData();
    data = GameController::getGameController()->getPlaySceneData(userData->currentLevel);

    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(data->music->getCString());
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(data->music->getCString() , true);

    winSize = CCDirector::sharedDirector()->getWinSize();
    ptmRatio = winSize.height / 10;

    GameObj::setB2world(world , ptmRatio);

    scheduleUpdate();

    setKeypadEnabled(true);
}

void PlayScene::keyBackClicked(){
	CCLayer::keyBackClicked();
	CCDirector::sharedDirector()->end();
}

void PlayScene::initPlayer(){
	player = new PlayerObj();
	addChild(player->load(PlayerObj::MIDDLE));
}

void PlayScene::initLanes(){
	lanes.reserve(data->laneNumber);
	for(int i = 0 ; i < data->laneNumber ; i++){
		lanes.push_back(Lane::creatWithDescription(data->laneDescriptions[i]));
		addChild(lanes[i]);
	}
}

void PlayScene::initControlMenu(){
    controlMenu = ControlMenu::create();
	addChild(controlMenu);
}

void PlayScene::initBoundary(){
	CCSize size = CCSizeMake(winSize.width , 0);

	lowerBoundary = new GameObj();
	B2Sprite * boundary = lowerBoundary->load(b2_staticBody , &size , LOWER_BOUNDARY);
	lowerBoundary->setPosition(ccp(winSize.width / 2 , 0));
	addChild(boundary);

	upperBoundary = new GameObj();
	boundary = upperBoundary->load(b2_staticBody , &size , UPPER_BOUNDARY);
	upperBoundary->setPosition(ccp(winSize.width / 2 , winSize.height));
	addChild(boundary);
}

void PlayScene::upHandler(int tag){
	player->jumpUp();
}

void PlayScene::downHandler(int tag){
	player->jumpDown();
}

void PlayScene::touchendHandler(int tag){
	player->wait();
}

void PlayScene::BeginContact(b2Contact *contact){
	CCSprite * contactA = (CCSprite *)contact->GetFixtureA()->GetBody()->GetUserData();
	CCSprite * contactB = (CCSprite *)contact->GetFixtureB()->GetBody()->GetUserData();

    if (contactA->getTag() == PLAYER) {
        this->contact = contactB;
    }else if (contactB->getTag() == PLAYER) {
        this->contact = contactA;
    }
}

void PlayScene::processContact(float dt)
{
    player->processContact(contact);
}

void PlayScene::update(float dt){

	world->Step(dt , 8 , 3);

	if(contact != NULL){
		processContact(0);
		contact = NULL;
	}

	player->step(dt);

	for(int i = 0 ; i < lanes.size() ; i++){
		lanes[i]->step(dt);
	}

	b2Body *node = world->GetBodyList();

	while(node){

	    b2Body *body = node;
	    node = node->GetNext();

		CCSprite *sprite = (CCSprite *)body->GetUserData();

		if (sprite != NULL && (sprite->getTag() == CAR)) {
			if(sprite->getPosition().x < 0 || sprite->getPosition().x > winSize.width){
				delete (GameObj *)(sprite->getUserData());
                body->SetUserData(NULL);
				world->DestroyBody(body);
			}
		}else if(sprite != NULL && (sprite->getTag() == SPECIAL)){
			SpecialObj * special = (SpecialObj *)sprite->getUserData();

			if(special->getSpecialId() > SPECIAL_NUM){
				if(sprite->getPosition().x < 0 || sprite->getPosition().x > winSize.width){
					special->expire();
				}
			}

			if(special->isExpired() == true && special->isTaken() == false ){
				delete special;
                body->SetUserData(NULL);
				world->DestroyBody(body);
			}
		}
	}

	controlMenu->step(dt);
	city->step(dt);
}

void PlayScene::stopAllLanes(){
	int size = lanes.size();
    for (int i = 0; i < size; i++) {
        lanes[i]->stopAtPosition(winSize.width/2 , player->getSize().width);
    }
}

void PlayScene::freezeAllLanes(){
	int size = lanes.size();
    for (int i = 0; i < size; i++) {
        lanes[i]->stop();
    }
    city->unscheduleAllSelectors();
    unscheduleUpdate();
}

void PlayScene::restartAllLanes(){
    int size = lanes.size();
    for (int i = 0; i < size; i++) {
            lanes[i]->reStart();
    }
}

void PlayScene::resumeAllLanes(){
	int size = lanes.size();
    for (int i = 0; i < size; i++) {
            lanes[i]->reStart();
    }
    city->scheduleSpecial();
    scheduleUpdate();
}

unsigned long PlayScene::getCurrentTime(){
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void PlayScene::destroyRandomCar(){
	int size = lanes.size();
    for (int i = 0; i < size; i++) {
        lanes[i]->destroyLastCar();
    }
}

void PlayScene::setSpecialChance(float chance , float length){
	int size = lanes.size();
    for (int i = 0; i < size; i++) {
    	lanes[i]->setSpecialChance(chance);
    }
    unschedule(schedule_selector(PlayScene::resetSpecialChance));
    scheduleOnce(schedule_selector(PlayScene::resetSpecialChance) , length);
}
void PlayScene:: resetSpecialChance(){
	int size = lanes.size();
    for (int i = 0; i < size; i++) {
    	lanes[i]->resetSpecialChance();
    }
}

void PlayScene::allSmallCars(){
    int size = lanes.size();
    for (int i = 0; i < size; i++) {
        lanes[i]->allSmallCars();
    }
}

void PlayScene::resumeCarNumbers(){
    int size = lanes.size();
    for (int i = 0; i < size; i++) {
        lanes[i]->resumeCarNumbers();
    }
}

void PlayScene::speedUp(float percent){
    int size = lanes.size();
    for (int i = 0; i < size; i++) {
        lanes[i]->speedUp(percent);
    }
}
void PlayScene::resumeSpeed(){
    int size = lanes.size();
    for (int i = 0; i < size; i++) {
        lanes[i]->resumeSpeed();
    }
}

