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

}

City::~City() {
	// TODO Auto-generated destructor stub
}

bool City::init(){
	if(!CCLayer::init()){
		return false;
	}

	winSize = CCDirector::sharedDirector()->getWinSize();

    addCityObj();

    addRoad();

    scheduleSpecial();

	return true;
}

void City::addRoad(){

    int number = 5;//getRandom(0,6);
    CCString * roadName = CCString::createWithFormat("road0.png");//, number);
    CCSprite * road;

	for(int i = 0 ; i < 9 ; i++){
		road = CCSprite::createWithSpriteFrameName(roadName->getCString());

		road->setPosition(ccp(winSize.width/2 , winSize.height * 0.1 * i));

		road->setScaleY(0.8);

		addChild(road);
	}
}

void City::addCityObj(){

    CCSprite * background = CCSprite::create("background0.png");

    background->setPosition(ccp(winSize.width/2 , winSize.height/2));

    CCSize size = background->getContentSize();

    background->setScaleY(winSize.height/size.height);

    background->setScaleX(winSize.width/size.width);

    background->setOpacity(220);

    addChild(background);

	schedule(schedule_selector(City::addHornSounds), 8, kCCRepeatForever, 0.1);
}

void City::addHornSounds(){
    AnimationData * data = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(
            data->hornSoundImages[getRandom(0,data->hornSoundImages.size()-1)]->getCString());
}

void City::addSpecial(){

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

void City::scheduleSpecial(){
	playSceneData = GameController::getGameController()->getPlaySceneData(
			GameController::getGameController()->getUserData()->currentLevel);

	if(playSceneData->specialInterval > 0.5)
		schedule(schedule_selector(City::addSpecial) , playSceneData->specialInterval , kCCRepeatForever , 1.5);
}
