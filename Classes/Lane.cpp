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

Lane::Lane() {
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

	schedule(schedule_selector(Lane::addACar) , description->period);

	return true;
}

void Lane::addACar(float dt){
    
    if (isSpecialCar(description->specialChance)) {
        SpecialObj * speciaObj = new SpecialObj();
        speciaObj->load(description->left2right, description->initPos, description->velocity.x, this);
        return;
    }

	CarObj * car = new CarObj();
    
    car->load(description->left2right, description->initPos, description->velocity.x, this);
}

bool Lane::isSpecialCar(float chance){
    
    int value = chance * 100;
    
    if (getRandom(0, 100) < value) {
        return true;
    }
    
    return false;
}

void Lane::stopAtPosition(float x){
    CCArray * children = getChildren();
    
    B2Sprite * car;
    
    CCObject * pObject;
    
    CCARRAY_FOREACH(children, pObject){
        car = (B2Sprite *)pObject;
        
        if (car) {
            if (description->left2right && car->getPosition().x < x - car->getContentSize().width) {
                car->getB2Body()->SetLinearVelocity(b2Vec2(0 , 0));
            }else if (!description->left2right && car->getPosition().x > x + car->getContentSize().width){
                car->getB2Body()->SetLinearVelocity(b2Vec2(0 , 0));
            }
        }
    }
    
    unschedule(schedule_selector(Lane::addACar));
}

void Lane::reStart(){
    CCArray * children = getChildren();
    
    B2Sprite * car;
    
    CCObject * pObject;
    
    CCARRAY_FOREACH(children, pObject){
        car = (B2Sprite *)pObject;
        
        if (car) {
            GameObj * gameObj = (GameObj *)car->getUserData();
            car->getB2Body()->SetLinearVelocity(b2Vec2(gameObj->getSpeed() , 0));
        }
    }
    
    schedule(schedule_selector(Lane::addACar) , description->period);
}

