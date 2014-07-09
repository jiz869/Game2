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

}

SpecialObj::~SpecialObj() {
	// TODO Auto-generated destructor stub
}

B2Sprite * SpecialObj::load(bool left2right, cocos2d::CCPoint initPos, float speed, Lane *lane , vector<int> * carNumbers){
    specialId = getRandom(SPECIAL_NUM + 1, BAD_SPECIAL_NUM - 1);
    specialData = GameController::getGameController()->getSpecialData(specialId);
    int carNumber = getRandom(0, carNumbers->size()-1);
    CCString * carName = CCString::createWithFormat("car%d.png", carNumbers->at(carNumber));
    CarObj::load(carName->getCString(), left2right, initPos, speed, lane , SPECIAL);
    //gameObj->runAction(CCRepeatForever::create(CCAnimate::create(specialData->animation)));
    CCSprite * tag = CCSprite::createWithSpriteFrameName(specialData->imageName->getCString());
    tag->setScale(0.6);
    gameObj->addChild(tag);
    CCSize size = gameObj->getContentSize();
    tag->setPosition(ccp(size.width/2, size.height/2));
    return gameObj;
}

B2Sprite * SpecialObj::load(cocos2d::CCPoint initPos){
    specialId = getRandom(0, SPECIAL_NUM - 1);
    specialData = GameController::getGameController()->getSpecialData(specialId);
    GameObj::load(specialData->imageName->getCString() , b2_kinematicBody , SPECIAL);
    gameObj->runAction(CCRepeatForever::create(CCAnimate::create(specialData->animation)));
    setPosition(initPos);
    return gameObj;
}

void SpecialObj::begin(PlayerObj *player){
	if(specialId < SPECIAL_NUM){
		gameObj->setVisible(false);
		gameObj->setPosition(ccp(-1000 , -1000));
	}else{
        gameObj->removeAllChildren();
    }
    if (specialData->begin) {
        specialData->begin(player);
    }
    if(specialData->duration < 0.1){
        end(player);
    }
}

void SpecialObj::step(PlayerObj *player){
    timer_count = timer_count + 1.0/60;

    if (timer_count > specialData->duration) {
        end(player);
        return;
    }

    if (specialData->step) {
        specialData->step(player);
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
