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

    initMenu();
    initStartMenu();

    CCLog("initMenu");

    initTimeLabel();
    initScoreLabel();

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

    scheduleUpdate();

    numFrame=0;
    score = 0;
    duration = userData->levelDuration;
    seconds = duration;

    startUpdateTime = false;
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

void PlayScene::initMenu(){
	upButton = CCMenuItemImage::create("button_arrow_normal.png" ,
                                                         "button_arrow_selected.png" , this , menu_selector(PlayScene::upHandler));
	CCSize buttonSize = upButton->getContentSize();

	//upButton is on the left side
	upButton->setPosition(ccp(buttonSize.width/2 , buttonSize.height/2));
	upButton->setRotation(180);
    upButton->setScale(0.6);

	CCMenuItemImage * downButton = CCMenuItemImage::create("button_arrow_normal.png" ,
                                                           "button_arrow_selected.png" , this , menu_selector(PlayScene::downHandler));
	//downButton is on the right side
	downButton->setPosition(ccp(winSize.width-buttonSize.width/2, buttonSize.height/2));
    downButton->setScale(0.6);

	CCArray * array = CCArray::createWithCapacity(2);

	array->addObject(upButton);

	array->addObject(downButton);

	menu = MenuForArrowButton::createWithArray(array);

    menu->ignoreAnchorPointForPosition(false);

	menu->registerTouchendHandler(this , menu_selector(PlayScene::touchendHandler));

	menu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));

	addChild(menu);
}

void PlayScene::initStartMenu(){
	StartMenu * menu = StartMenu::create();
	addChild(menu);
}

void PlayScene::startNewGame(){
	menu->setPosition(ccp(winSize.width/2, winSize.height/2));
	startUpdateTime=true;
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
		score++;
		updateScore();
		seconds += duration;
		updateGameTime();
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

    if (startUpdateTime==false) {
        return;
    }

    if(++numFrame % 60 == 0) {
        seconds--;
        updateGameTime();
    }

    if(seconds == 0)
    	GameOver();
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

bool PlayScene::isUpButtonSelected(){
    return upButton->isSelected();
}

void PlayScene::updateGameTime()
{
    char ss[10];
    sprintf(ss, "%d", seconds);
    timeLabel->setString(ss);
}

void PlayScene::updateScore()
{
    char ss[10];
    sprintf(ss, "%d", score);
    scoreLabel->setString(ss);
}

void PlayScene::initTimeLabel()
{
    timeLabel = CCLabelTTF::create("0", "Helvetica", 64 );
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    timeLabel->setColor( ccc3(255, 10, 10) );
    timeLabel->setPosition( ccp(winSize.width/2, winSize.height - 50) );
    this->addChild(timeLabel);
}

void PlayScene::initScoreLabel()
{
    scoreLabel = CCLabelTTF::create("0", "Helvetica", 64 );
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    scoreLabel->setColor( ccc3(0, 0, 128) );
    scoreLabel->setPosition( ccp(winSize.width - 80, winSize.height - 50) );

    this->addChild(scoreLabel);
}

void PlayScene::GameOver()
{
    userData->lastScore = score;
    GameOverScene *gameOverScene = GameOverScene::create();
	CCDirector::sharedDirector()->replaceScene( gameOverScene );
}

