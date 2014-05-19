/*
 * CarObj.cpp
 *
 *  Created on: May 17, 2014
 *      Author: welcomelm
 */

#include "CarObj.h"

CarObj::CarObj() {
	// TODO Auto-generated constructor stub

}

CarObj::~CarObj() {
	// TODO Auto-generated destructor stub
}

B2Sprite * CarObj::load(bool left2right, cocos2d::CCPoint initPos, float speed , Lane * lane){
    
    int carNumber = getRandom(0, 9);
    
    CCString * carName = CCString::createWithFormat("car%d.png", carNumber);
    
    return load(carName->getCString(), left2right, initPos, speed, lane , CAR);
}

B2Sprite * CarObj::load(const char * name , bool left2right, cocos2d::CCPoint initPos, float speed, Lane *lane , SpriteTag tag){
    
    lane->addChild(GameObj::load(name , b2_kinematicBody , tag));
    
    if(left2right){
		gameObj->setFlipX(true);
	}
    
	setPosition(initPos);
    
    setSpeed(speed);
    
	setVelocity(b2Vec2(speed , 0));
    
    this->lane = lane;
    
    return gameObj;
}
