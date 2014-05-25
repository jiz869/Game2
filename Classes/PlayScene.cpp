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
#include "GameOverScene.h"
#include "StartMenu.h"
#include "ControlMenu.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene* PlayScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    PlayScene *layer = PlayScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


PlayScene::~PlayScene(){
	player->removeAllSpecials();

    for (b2Body * body = world->GetBodyList(); body; body = body->GetNext()) {
    	CCSprite *sprite = (CCSprite *)body->GetUserData();
    	if(sprite) delete (GameObj *)(sprite->getUserData());
        world->DestroyBody(body);
    }

    delete world;
}

bool PlayScene::init(){
    if (!CCLayerColor::initWithColor(ccc4(255,255,255,255))) {
        return false;
    }

    initMisc();

    CCLog("initMisc");

    initBoundary();

    CCLog("initBoundary");

    initLanes();

    CCLog("initLanes");

    initPlayer();

    CCLog("initPlayer");

    initControlMenu();
    initStartMenu();

    CCLog("initMenu");

    return true;
}

void PlayScene::initMisc(){
    world = new b2World(b2Vec2(0 , 0));
    world->SetContactListener(this);

    data = GameController::getGameController()->getPlaySceneData(0);
    userData = GameController::getGameController()->getUserData();

    winSize = CCDirector::sharedDirector()->getWinSize();
    ptmRatio = winSize.height / 10;

    GameObj::setB2world(world , ptmRatio);
    
    if (userData->sound == UNMUTE) {
        startPlayBackgroundMusic();
    }

    scheduleUpdate();
}

void PlayScene::startPlayBackgroundMusic(){
    if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(data->backgroundSoundImage->getCString(), true);
}

void PlayScene::initPlayer(){
	player = new PlayerObj();
	player->setData(data);
	addChild(player->load());
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

void PlayScene::initStartMenu(){
	startMenu = StartMenu::create();
	addChild(startMenu);
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

void PlayScene::upHandler(CCObject * sender){
	player->jumpUp();
}

void PlayScene::downHandler(CCObject * sender){
	player->jumpDown();
}

void PlayScene::touchendHandler(CCObject * sender){
	player->wait();
}

void PlayScene::BeginContact(b2Contact *contact){
	CCSprite * contactA = (CCSprite *)contact->GetFixtureA()->GetBody()->GetUserData();
	CCSprite * contactB = (CCSprite *)contact->GetFixtureB()->GetBody()->GetUserData();

    if (contactA->getTag() == PLAYER) {
        this->contact = contactB;
        scheduleOnce(schedule_selector(PlayScene::processContact), 0);

        return;
    }

    if (contactB->getTag() == PLAYER) {
        this->contact = contactA;
        scheduleOnce(schedule_selector(PlayScene::processContact), 0);
    }

}

void PlayScene::processContact(float dt)
{
	if( contact->getTag() == UPPER_BOUNDARY ) {
		controlMenu->doScore();
	}
    player->processContact(contact);
}

void PlayScene::update(float dt){

	world->Step(dt , 8 , 3);

	player->step(dt);

	for(int i = 0 ; i < lanes.size() ; i++){
		lanes[i]->step(dt);
	}

	for(b2Body *b = world->GetBodyList(); b; b=b->GetNext()){
		CCSprite *sprite = (CCSprite *)b->GetUserData();

		if (sprite != NULL && sprite->getTag() == CAR) {
			if(sprite->getPosition().x < 0 || sprite->getPosition().x > winSize.width){
				delete (GameObj *)(sprite->getUserData());
				sprite->removeFromParentAndCleanup(true);
				world->DestroyBody(b);
			}
		}
	}

	controlMenu->step(dt);
}

void PlayScene::stopAllLanes(){
	int size = lanes.size();
    for (int i = 0; i < size; i++) {
        lanes[i]->stopAtPosition(winSize.width/2);
    }
}

void PlayScene::restartAllLanes(){
	int size = lanes.size();
    for (int i = 0; i < size; i++) {
            lanes[i]->reStart();
    }
}

void PlayScene::slowAllLanes(float speed_decrease, float interval_increase){
	int size = lanes.size();
    for (int i = 0; i < size; i++) {
        lanes[i]->slow(speed_decrease, interval_increase);
    }
}

void PlayScene::resumeAllLanesFromSlow(float speed_decrease, float interval_increase){
	int size = lanes.size();
    for (int i = 0; i < size; i++) {
        lanes[i]->resumeFromSlow(speed_decrease, interval_increase);
    }
}

