/*
 * SpecialObj.cpp
 *
 *  Created on: May 17, 2014
 *      Author: welcomelm
 */

#include "SpecialObj.h"
#include "PlayObj.h"

SpecialObj::SpecialObj() : timer_count(0) , taken(false) , expired(false) {
	// TODO Auto-generated constructor stub
    UserData * userData = GameController::getGameController()->getUserData();
    PlaySceneData * playSceneData = GameController::getGameController()->getPlaySceneData(userData->currentLevel);
    goodMax = playSceneData->goodMax;
    badMax = playSceneData->badMax;
}

SpecialObj::~SpecialObj() {
	// TODO Auto-generated destructor stub
}

B2Sprite * SpecialObj::load(bool left2right, cocos2d::CCPoint initPos, float speed, Lane *lane , vector<int> * carNumbers){
    specialId = getRandom(SPECIAL_NUM + 1, badMax);
    specialData = GameController::getGameController()->getSpecialData(specialId);
    int carNumber = getRandom(0, carNumbers->size()-1);
    CCString * carName = CCString::createWithFormat("car%d%d.png", currentLevel , carNumbers->at(carNumber));
    CarObj::load(carName->getCString(), left2right, initPos, speed, lane , SPECIAL);
    //gameObj->runAction(CCRepeatForever::create(CCAnimate::create(specialData->animation)));
    CCSprite * tag = CCSprite::createWithSpriteFrameName(specialData->imageName->getCString());
    tag->setScale(0.45);
    //tag->setScale(0.8);
    gameObj->addChild(tag);
    CCSize size = gameObj->getContentSize();
    tag->setPosition(ccp(size.width/2 , size.height/2));
    return gameObj;
}

B2Sprite * SpecialObj::load(cocos2d::CCPoint initPos){
    specialId = getRandom(0, goodMax);
    specialData = GameController::getGameController()->getSpecialData(specialId);
    GameObj::load(specialData->imageName->getCString() , b2_kinematicBody , SPECIAL);
    //gameObj->runAction(CCRepeatForever::create(CCAnimate::create(specialData->animation)));
    setPosition(initPos);
    gameObj->setScale(0.45);
    return gameObj;
}

B2Sprite * SpecialObj::load(int exceptId){
    int specialId = getRandom(0 , exceptId - 1);
    this->specialId = specialId;
    specialData = GameController::getGameController()->getSpecialData(specialId);
    GameObj::load(specialData->imageName->getCString() , b2_kinematicBody , SPECIAL);
    //gameObj->runAction(CCRepeatForever::create(CCAnimate::create(specialData->animation)));
    setPosition(ccp(-1000 , -1000));
    gameObj->setScale(0.5);
    return gameObj;
}

void SpecialObj::begin(PlayerObj *player){
	if(specialId <= SPECIAL_NUM){
		gameObj->setVisible(false);
		gameObj->setPosition(ccp(-1000 , -1000));
	}else{
        gameObj->removeAllChildren();
    }

	timer_count = 0;

    if (specialData->begin) {
        specialData->begin(player);
    }
}

void SpecialObj::step(PlayerObj *player){
    timer_count = timer_count + 1.0/60;

    if (timer_count > specialData->duration) {
        end(player);
        return;
    }

    if (specialData->step) {
        specialData->step(player , timer_count);
    }
}

void SpecialObj::end(PlayerObj *player){
    timer_count = 0;
    if (specialData->end) {
        specialData->end(player);
    }
    player->endWithSpecial(this);
}

bool SpecialObj::hitByCar(PlayerObj *player , CCSprite * car){
    if (specialData->hitByCar) {
        return specialData->hitByCar(player , car);
    }

    //default is reset player when hit by car
    return true;
}

SpecialData * SpecialObj::getSpecialData(){
    return specialData;
}

int SpecialObj::getSpecialId(){
    return specialId;
}

void SpecialObj::take(){
	taken = true;
}

void SpecialObj::unTake(){
	taken = false;
}

bool SpecialObj::isTaken(){
	return taken;
}

void SpecialObj::expire(){
	expired = true;
}

bool SpecialObj::isExpired(){
	return expired;
}
