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

B2Sprite * SpecialObj::load(bool left2right, cocos2d::CCPoint initPos, float speed, Lane *lane){
    specialId = getRandom(0, SPECIAL_NUM - 1);
    specialData = GameController::getGameController()->getSpecialData(specialId);
    CarObj::load(specialData->imageName->getCString(), left2right, initPos, speed, lane , SPECIAL);
    gameObj->runAction(CCRepeatForever::create(CCAnimate::create(specialData->animation)));
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
    gameObj->setVisible(false);
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
