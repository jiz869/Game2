/*
 * Lane.cpp
 *
 *  Created on: May 16, 2014
 *      Author: mli
 */

#include "Lane.h"

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

	GameObj * carObj = new GameObj();

	int carNumber = rand()%7;

	CCString * carName = CCString::createWithFormat("car%d.png", carNumber);

	B2Sprite * car = carObj->load(carName->getCString() , b2_kinematicBody , CAR);

	if(description->left2right){
		car->setFlipX(true);
	}

	carObj->setPosition(description->initPos);

	carObj->setVelocity(description->velocity);

	addChild(car);
}
