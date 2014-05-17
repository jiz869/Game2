/*
 * GameObj.cpp
 *
 *  Created on: May 15, 2014
 *      Author: mli
 */

#include "GameObj.h"

float GameObj::ptmRatio;
b2World* GameObj::world;

GameObj::GameObj() {
	// TODO Auto-generated constructor stub
    animationData = GameController::getGameController()->getAnimationData();
}

GameObj::~GameObj() {
	// TODO Auto-generated destructor stub
}


B2Sprite * GameObj::load(const char * name , b2BodyType type , SpriteTag tag){
	return load(name , type , NULL , tag);
}

B2Sprite * GameObj::load(b2BodyType type , const CCSize * size , SpriteTag tag){
	return load(NULL , type , size , tag);
}

B2Sprite * GameObj::load(const char * name , b2BodyType type , const CCSize * size , SpriteTag tag){

	if(name){
		gameObj = B2Sprite::createWithSpriteFrameName(name);
	}else{
		gameObj = B2Sprite::create();
	}

	if(size == NULL) size = &gameObj->getContentSize();

    b2BodyDef bodyDef;
    bodyDef.type = type;
    b2Body * body = world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(size->width/2/ptmRatio, size->height/2/ptmRatio);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    //birdfixtureDef.restitution = 0.7;
    body->CreateFixture(&fixtureDef);

    gameObj->setB2Body(body);
    gameObj->setPTMRatio(ptmRatio);

    gameObj->setUserData(this);
    gameObj->setTag(tag);

	return gameObj;
}

void GameObj::setPosition(const CCPoint& position){
    gameObj->getB2Body()->SetTransform(b2Vec2(position.x/ptmRatio , position.y/ptmRatio) , 0);
}

void GameObj::setVelocity(const CCPoint& velocity){
    gameObj->getB2Body()->SetLinearVelocity(b2Vec2(velocity.x , velocity.y));
}

void GameObj::setSpeed(float speed){
    this->speed = speed;
}

const CCSize& GameObj::getSize(){
	return gameObj->getContentSize();
}

void GameObj::setB2world(b2World * world , float ptmRatio){
	GameObj::world = world;
	GameObj::ptmRatio = ptmRatio;
}

bool GameObj::isBottom(){
    return (gameObj->getPosition().y - gameObj->getContentSize().height / 2 < 1);
}

