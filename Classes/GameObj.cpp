/*
 * GameObj.cpp
 *
 *  Created on: May 15, 2014
 *      Author: mli
 */

#include "GameObj.h"

GameObj::GameObj() {
	// TODO Auto-generated constructor stub

}

GameObj::~GameObj() {
	// TODO Auto-generated destructor stub
}

B2Sprite * GameObj::load(const char * name , float ratio , b2BodyType type , b2World * world){

	gameObj = B2Sprite::createWithSpriteFrameName(name);
    
    CCSize size = gameObj->getContentSize();

    b2BodyDef bodyDef;
    bodyDef.type = type;
    b2Body * body = world->CreateBody(&bodyDef);
    
    b2PolygonShape shape;
    shape.SetAsBox(size.width/2/ratio, size.height/2/ratio);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    //birdfixtureDef.restitution = 0.7;
    body->CreateFixture(&fixtureDef);
    
    gameObj->setB2Body(body);
    gameObj->setPTMRatio(ratio);
    
    gameObj->setUserData(this);

	return gameObj;
}

void GameObj::setPosition(const b2Vec2 &position){
    gameObj->getB2Body()->SetTransform(position, 0);
}

void GameObj::setVelocity(const b2Vec2 &velocity){
    gameObj->getB2Body()->SetLinearVelocity(velocity);
}

void GameObj::setSpeed(float speed){
    this->speed = speed;
}

