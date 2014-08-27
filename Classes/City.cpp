/*
 * City.cpp
 *
 *  Created on: May 29, 2014
 *      Author: mli
 */

#include "City.h"
#include "Lane.h"
#include "SimpleAudioEngine.h"
#include "SpecialObj.h"

using namespace CocosDenshion;

City::City() {
	// TODO Auto-generated constructor stub
    timeSinceLastSpecial = 10000.0;
}

City::~City() {
	// TODO Auto-generated destructor stub
}

bool City::init(){
	if(!CCLayer::init()){
		return false;
	}

	winSize = CCDirector::sharedDirector()->getWinSize();
	userData = GameController::getGameController()->getUserData();

    addCityObj();

    addRoad();

    //scheduleSpecial();

	return true;
}

void City::addRoad(){

    CCString * roadName = CCString::createWithFormat("road%d.png", userData->currentLevel);
    CCSprite * road;

	for(int i = 0 ; i < 9 ; i++){
		road = CCSprite::createWithSpriteFrameName(roadName->getCString());

		road->setPosition(ccp(winSize.width/2 , winSize.height * 0.1 * i));

		addChild(road);
	}
}

void City::addCityObj(){



	CCString * name = CCString::createWithFormat("background%d.png", userData->currentLevel);

    CCSprite * background = CCSprite::create(name->getCString());

    background->setPosition(ccp(winSize.width/2 , winSize.height/2));

    CCSize size = background->getContentSize();

    background->setScaleY(winSize.height/size.height);

    background->setScaleX(winSize.width/size.width);

    background->setOpacity(180);

    addChild(background);
}

void City::addHornSounds(){
    AnimationData * data = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(
            data->hornSoundImages[getRandom(0,data->hornSoundImages.size()-1)]->getCString());
}

void City::addSpecial(){

    timeSinceLastSpecial = 0;

	float initPosY = getRandom(1, playSceneData->laneNumber) * 0.1;

	CCPoint initPos = ccp(winSize.width * 0.5 , initPosY * winSize.height);

	SpecialObj * specialObj = new SpecialObj();

	CCSprite * specialSprite = specialObj->load(initPos);

    specialSprite->runAction(CCSequence::create(CCDelayTime::create(specialObj->getSpecialData()->life) ,
                                              CCCallFuncN::create(this, callfuncN_selector(City::expireSpecial)) , NULL));

	addChild(specialSprite);
}

void City::expireSpecial(cocos2d::CCObject *special){
    CCSprite * specialSprite = (CCSprite *)special;
    SpecialObj * specialObj = (SpecialObj *)specialSprite->getUserData();
    specialObj->expire();
}

void City::step(float dt){
    timeSinceLastSpecial += dt;
}

void City::scheduleSpecial(){
	playSceneData = GameController::getGameController()->getPlaySceneData(
			GameController::getGameController()->getUserData()->currentLevel);

	float delay;

	if(timeSinceLastSpecial > playSceneData->specialInterval) delay = 0.01;
	else delay = playSceneData->specialInterval - timeSinceLastSpecial;

	if(playSceneData->specialInterval > 0.5)
		schedule(schedule_selector(City::addSpecial) , playSceneData->specialInterval ,
		        kCCRepeatForever , delay);

	schedule(schedule_selector(City::addHornSounds), 8, kCCRepeatForever, 0.01);
}
