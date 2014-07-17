/*
 * Lane.cpp
 *
 *  Created on: May 16, 2014
 *      Author: mli
 */

#include "Lane.h"
#include "CarObj.h"
#include "SpecialObj.h"

int getRandom(int low, int high)
{
	if ( low - high < 0x10000L )
        return low + ( ( rand() >> 8 ) % ( high + 1 - low ) );

	return low + ( random() % ( high + 1 - low ) );
}

bool toss(float chance){

    int value = chance * 100;

    if (getRandom(0, 99) < value) {
        return true;
    }

    return false;
}

Lane::Lane() : status(RUNNING) , timePassedFromLastSchedule(0) {
	// TODO Auto-generated constructor stub

}

Lane::~Lane() {
	// TODO Auto-generated destructor stub
}

Lane * Lane::creatWithDescription(LaneDescription * description){
	Lane * lane = new Lane();

	if(lane && lane->initWithDescription(description)){
		lane->autorelease();
		return lane;
	}

	CC_SAFE_DELETE(lane);
	return NULL;
}

bool Lane::initWithDescription(LaneDescription * description){
	if(! CCLayer::init()){
		return false;
	}

	this->description = description;

	scheduleOnce(schedule_selector(Lane::addACar1) , 0);

	return true;
}

void Lane::addACar1(float dt){

	timePassedFromLastSchedule = 0;
	float speed , interval;

	UserData * userData = GameController::getGameController()->getUserData();

	if(userData->pvpMode == PREPARE_PLAY){
		interval = description->period;
	}else{
		interval = description->period - 0.1*getRandom(0,2);
	}
	speed = description->carSpeed;


    if (toss(description->specialChance)) {
        SpecialObj * speciaObj = new SpecialObj();
        speciaObj->load(description->left2right, description->initPos, speed, this , &description->carNumbers);
    }else{

		CarObj * car = new CarObj();

		car->load(description->left2right, description->initPos, speed, this , &description->carNumbers);
    }

    scheduleOnce(schedule_selector(Lane::addACar2) , interval);
}

void Lane::addACar2(float dt){

    timePassedFromLastSchedule = 0;

	float speed , interval;

	UserData * userData = GameController::getGameController()->getUserData();

	if(userData->pvpMode == PREPARE_PLAY){
		interval = description->period;
	}else{
		interval = description->period - 0.1*getRandom(0,2);
	}
	speed = description->carSpeed;

    if (toss(description->specialChance)) {
        SpecialObj * speciaObj = new SpecialObj();
        speciaObj->load(description->left2right, description->initPos, speed, this , &description->carNumbers);
    }else{

		CarObj * car = new CarObj();

		car->load(description->left2right, description->initPos, speed, this , &description->carNumbers);
    }

    scheduleOnce(schedule_selector(Lane::addACar1) , interval);
}

void Lane::stopAtPosition(float x){
    CCArray * children = getChildren();

    B2Sprite * car;

    CCObject * pObject;

    CCARRAY_FOREACH(children, pObject){
        car = (B2Sprite *)pObject;

        if (car && car->getTag() != kCCNodeTagInvalid) {
            if (description->left2right && car->getPosition().x < x - car->getContentSize().width) {
                car->getB2Body()->SetLinearVelocity(b2Vec2(0 , 0));
            }else if (!description->left2right && car->getPosition().x > x + car->getContentSize().width){
                car->getB2Body()->SetLinearVelocity(b2Vec2(0 , 0));
            }
        }
    }

    unscheduleAllSelectors();

    status = STOPPED;
}

void Lane::stop(){
    CCArray * children = getChildren();

    B2Sprite * car;

    CCObject * pObject;

    CCARRAY_FOREACH(children, pObject){
        car = (B2Sprite *)pObject;

        if (car && car->getTag() != kCCNodeTagInvalid) {
        	car->getB2Body()->SetLinearVelocity(b2Vec2(0 , 0));
        }
    }

    unscheduleAllSelectors();

    status = STOPPED;
}

void Lane::reStart(){
    CCArray * children = getChildren();

    B2Sprite * car;

    CCObject * pObject;

    CCARRAY_FOREACH(children, pObject){
        car = (B2Sprite *)pObject;

        if (car && car->getTag() != kCCNodeTagInvalid) {
        	car->getB2Body()->SetLinearVelocity(b2Vec2(description->carSpeed , 0));
        }
    }

    float delay;

    if(timePassedFromLastSchedule > description->period) delay = 0;
    else delay = description->period - timePassedFromLastSchedule;

    scheduleOnce(schedule_selector(Lane::addACar1) , delay);

    status = RUNNING;
}

void Lane::step(float dt){
	if(status == RUNNING) timePassedFromLastSchedule += dt;
}

void Lane::destroyLastCar(){
    CCArray * children = getChildren();

    B2Sprite * car = NULL , * lastCar = NULL;

    CCObject * pObject;

    CCARRAY_FOREACH(children, pObject){
        car = (B2Sprite *)pObject;

        if (car && car->getTag() != kCCNodeTagInvalid) {
        	lastCar = car;
        }
    }

    if(lastCar){
    	CarObj * carObj = (CarObj *)lastCar->getUserData();
    	carObj->doom();
    	lastCar->removeAllChildren();
    	CCAnimate * animate = CCAnimate::create(GameController::getGameController()->getAnimationData()->explodeAnim);
    	lastCar->runAction(CCSequence::createWithTwoActions(animate , CCCallFuncN::create(this, callfuncN_selector(Lane::destroyMe))));
    }
}

void Lane::destroyMe(CCObject * car){
    B2Sprite * carSprite = (B2Sprite *)car;
    carSprite->setPosition(ccp(-1000 , -1000));
}
