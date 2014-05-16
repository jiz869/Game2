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

    initPlayer();

    CCLog("initPlayer");

    initLanes();

    CCLog("initLanes");

    initMenu();

    CCLog("initMenu");

    return true;
}

void PlayScene::initMisc(){
    world = new b2World(b2Vec2(0 , 0));
    world->SetContactListener(this);

    data = GameController::getGameController()->getPlaySceneData(0);

    winSize = CCDirector::sharedDirector()->getWinSize();
    ptmRatio = winSize.height / 10;

    GameObj::setB2world(world , ptmRatio);

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprites.plist");

    scheduleUpdate();
}

void PlayScene::initPlayer(){
	player.setSpeed(data->playerSpeed);
	addChild(player.load("frog0.png" , b2_dynamicBody , PLAYER));
}

void PlayScene::initLanes(){
	lanes.reserve(data->laneNumber);
	for(int i = 0 ; i < data->laneNumber ; i++){
		lanes[i] = Lane::creatWithDescription(data->laneDescriptions[i]);
		addChild(lanes[i]);
	}
}

void PlayScene::initMenu(){
	CCMenuItemImage * upButton = CCMenuItemImage::create("button_arrow_normal.png" ,
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

	B2Sprite * boundary = lowerBoundary.load(b2_staticBody , &size , LOWER_BOUNDARY);
	lowerBoundary.setPosition(ccp(winSize.width / 2 , 0));
	addChild(boundary);

	boundary = upperBoundary.load(b2_staticBody , &size , UPPER_BOUNDARY);
	upperBoundary.setPosition(ccp(winSize.width / 2 , winSize.height));
	addChild(boundary);
}

void PlayScene::upHandler(CCObject * sender){
	player.jumpUp();
}

void PlayScene::downHandler(CCObject * sender){
	player.jumpDown();
}

void PlayScene::touchendHandler(CCObject * sender){
	player.wait();
}

void PlayScene::BeginContact(b2Contact *contact){
	CCSprite * spriteA = (CCSprite *)contact->GetFixtureA()->GetBody()->GetUserData();
	CCSprite * spriteB = (CCSprite *)contact->GetFixtureB()->GetBody()->GetUserData();

	if(spriteA->getTag() == LOWER_BOUNDARY ||
			spriteB->getTag() == LOWER_BOUNDARY ||
			spriteB->getTag() == UPPER_BOUNDARY ||
			spriteA->getTag() == UPPER_BOUNDARY){
		player.resetNextUpdate(true);
	}

	if(spriteA->getTag() == CAR || spriteB->getTag() == CAR){
		player.resetNextUpdate(true);
	}
}

void PlayScene::update(float dt){
	world->Step(dt , 8 , 3);
	player.step(dt);

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
}

