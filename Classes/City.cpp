/*
 * City.cpp
 *
 *  Created on: May 29, 2014
 *      Author: mli
 */

#include "City.h"
#include "Lane.h"
#include "SimpleAudioEngine.h"

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

	return true;
}

void City::addRoad(){

    int number = 5;//getRandom(0,6);
    CCString * roadName = CCString::createWithFormat("road%d.png", number);;
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

    background->setOpacity(128);

    addChild(background);

	schedule(schedule_selector(City::addHornSounds), 8, kCCRepeatForever, 0.1);
}

void City::addHornSounds(){
    AnimationData * data = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(
            data->hornSoundImages[getRandom(0,data->hornSoundImages.size()-1)]->getCString());
}
