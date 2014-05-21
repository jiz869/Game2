/*
 * SpecialObj.cpp
 *
 *  Created on: May 17, 2014
 *      Author: welcomelm
 */

#include "SpecialObj.h"
#include "PlayObj.h"

SpecialObj::SpecialObj() : timer_count(0) {
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

void SpecialObj::begin(PlayerObj *player){
    world->DestroyBody(gameObj->getB2Body());
    gameObj->removeFromParent();
    if (specialData->begin) {
        specialData->begin(player , this);
    }
}

void SpecialObj::step(PlayerObj *player){
    timer_count = timer_count + 1.0/60;

    if (timer_count > specialData->duration) {
        end(player);
    }

    if (specialData->step) {
        specialData->step(player , this);
    }
}

void SpecialObj::end(PlayerObj *player){
    timer_count = 0;
    if (specialData->end) {
        specialData->end(player , this);
    }
    player->endWithSpecial(this);
}

bool SpecialObj::hitByCar(PlayerObj *player){
    if (specialData->hitByCar) {
        return specialData->hitByCar(player , this);
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
