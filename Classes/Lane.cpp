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
        return low + ( ( random() >> 8 ) % ( high + 1 - low ) );

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

    if (toss(description->specialChance)) {
        SpecialObj * speciaObj = new SpecialObj();
        speciaObj->load(description->left2right, description->initPos, description->carSpeed, this);
        return;
    }

	CarObj * car = new CarObj();

	float speed , interval;

    if(toss(0.5)){
        speed = description->carSpeed + 0.1*getRandom(0,3);
        interval = description->period - 0.1*getRandom(0,3);
    }else{
        speed = description->carSpeed - 0.1*getRandom(0,3);
        interval = description->period + 0.1*getRandom(0,3);
    }

    car->load(description->left2right, description->initPos, speed, this , &description->carNumbers);

    scheduleOnce(schedule_selector(Lane::addACar2) , interval);
}

void Lane::addACar2(float dt){

    timePassedFromLastSchedule = 0;

    if (toss(description->specialChance)) {
        SpecialObj * speciaObj = new SpecialObj();
        speciaObj->load(description->left2right, description->initPos, description->carSpeed, this);
        return;
    }

    CarObj * car = new CarObj();

    float speed , interval;

    if(toss(0.5)){
        speed = description->carSpeed + 0.1*getRandom(0,3);
        interval = description->period - 0.1*getRandom(0,3);
    }else{
        speed = description->carSpeed - 0.1*getRandom(0,3);
        interval = description->period + 0.1*getRandom(0,3);
    }

    car->load(description->left2right, description->initPos, speed, this , &description->carNumbers);

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

    if(timePassedFromLastSchedule > description->period) delay = 0.1;
    else delay = description->period - timePassedFromLastSchedule;

    unscheduleAllSelectors();
    schedule(schedule_selector(Lane::addACar1) , description->period ,
    		kCCRepeatForever , delay);

    status = RUNNING;
}

void Lane::slow(float speed_decrease , float interval_increase){

	if (description->left2right){
		description->carSpeed -= speed_decrease;
	}else if (!description->left2right){
		description->carSpeed += speed_decrease;
	}

	description->period += interval_increase;

	if(status != STOPPED){
		reStart();
	}
}

void Lane::resumeFromSlow(float speed_decrease , float interval_increase){
	if (description->left2right){
		description->carSpeed += speed_decrease;
	}else if (!description->left2right){
		description->carSpeed -= speed_decrease;
	}

	description->period -= interval_increase;

	if(status != STOPPED){
		reStart();
	}
}

void Lane::step(float dt){
	if(status == RUNNING) timePassedFromLastSchedule += dt;
}
