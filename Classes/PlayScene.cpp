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

    initMenu();

    CCLog("initMenu");

    initPlayer();

    CCLog("initPlayer");

    initTimeLabel();
    initScoreLabel();

    return true;
}

void PlayScene::initMisc(){
    world = new b2World(b2Vec2(0 , 0));
    world->SetContactListener(this);

    data = GameController::getGameController()->getPlaySceneData(0);

    winSize = CCDirector::sharedDirector()->getWinSize();
    ptmRatio = winSize.height / 10;

    GameObj::setB2world(world , ptmRatio);

    scheduleUpdate();

    numFrame=0;
    score = 0;
    duration = GameController::getGameController()->levelDuration;
    seconds = duration;
}

void PlayScene::initPlayer(){
	player = new PlayerObj();
	player->setSpeed(data->playerSpeed);
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

	MenuForArrowButton * menu = MenuForArrowButton::createWithArray(array);

    menu->ignoreAnchorPointForPosition(false);

	menu->registerTouchendHandler(this , menu_selector(PlayScene::touchendHandler));

	addChild(menu);
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
	}
    player->processContact(contact);
}

void PlayScene::update(float dt){
    numFrame++;
	world->Step(dt , 8 , 3);

	player->step(dt);

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

    if(numFrame % 60 == 0) {
        seconds--;
        updateGameTime();
    }

    if(seconds == 0)
    	GameOver();
}

void PlayScene::stopAllLanes(){
    for (int i = 0; i < lanes.size(); i++) {
        lanes[i]->stopAtPosition(winSize.width/2);
    }
}

void PlayScene::restartAllLanes(){
    for (int i = 0; i < lanes.size(); i++) {
            lanes[i]->reStart();
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
    GameController *gc = GameController::getGameController();
    gc->lastScore = score;
    GameOverScene *gameOverScene = GameOverScene::create();
	CCDirector::sharedDirector()->replaceScene( gameOverScene );
}

