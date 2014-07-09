/*
 * GameObj.cpp
 *
 *  Created on: May 15, 2014
 *      Author: mli
 */

#include "GameObj.h"

float GameObj::ptmRatio;
b2World* GameObj::world;

GameObj::GameObj() : gameObj(NULL) {
	// TODO Auto-generated constructor stub
}

GameObj::~GameObj() {
	// TODO Auto-generated destructor stub
	if(gameObj){
		gameObj->removeAllChildren();
		gameObj->removeFromParent();
	}
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

    shape.SetAsBox(size->width/2/ptmRatio * 0.8, size->height/2/ptmRatio * 0.8);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    //birdfixtureDef.restitution = 0.7;
    if (tag == PLAYER) {
        fixtureDef.isSensor = true;
    }
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

const CCPoint& GameObj::getPosition(){
    return gameObj->getPosition();
}

void GameObj::setVelocity(b2Vec2 velocity){
    gameObj->getB2Body()->SetLinearVelocity(velocity);
}

const b2Vec2 GameObj::getVelocity(){
    return gameObj->getB2Body()->GetLinearVelocity();
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

void GameObj::runAction(cocos2d::CCAction *action){
    gameObj->runAction(action);
}

