//
//  PlayScene.cpp
//  crossRoadMain
//
//  Created by Min Li on 2014-05-15.
//
//

#include "PlayScene.h"
#include "MenuForArrowButton.h"

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
        world->DestroyBody(body);
    }
    
    delete world;
}

bool PlayScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    initWorld();
    
    initMisc();
    
    initPlayer();
    
    initMenu();
    
    return true;
}

void PlayScene::initWorld(){
    world = new b2World(b2Vec2(0 , 0));
    world->SetContactListener(this);
}

void PlayScene::initMisc(){
    winSize = CCDirector::sharedDirector()->getWinSize();
    ptmRatio = winSize.height / 10;
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprites.plist");
    
    batchNode = CCSpriteBatchNode::create("sprites.png");
}

void PlayScene::initPlayer(){
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
	downButton->setPosition(ccp(winSize.width-buttonSize.width/2, winSize.height/2));
    downButton->setScale(0.6);
    
	CCArray * array = CCArray::createWithCapacity(2);
    
	array->addObject(upButton);
    
	array->addObject(downButton);
    
	MenuForArrowButton * menu = MenuForArrowButton::createWithArray(array);
    
    menu->ignoreAnchorPointForPosition(false);
    
	menu->registerTouchendHandler(this , menu_selector(PlayScene::touchendHandler));
    
	addChild(menu);
}

void PlayScene::upHandler(CCObject * sender){
	player.JumpUp();
}

void PlayScene::downHandler(CCObject * sender){
	player.JumpDown();
}

void PlayScene::touchendHandler(CCObject * sender){
	player.Wait();
}

void PlayScene::BeginContact(b2Contact *contact){
    
}

void PlayScene::update(float dt){
    
}

