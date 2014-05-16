/*
 * GameObj.h
 *
 *  Created on: May 15, 2014
 *      Author: mli
 */

#ifndef GAMEOBJ_H_
#define GAMEOBJ_H_

#include "cocos2d.h"
#include "B2Sprite.h"
#include "box2d/Box2D.h"
USING_NS_CC;

class GameObj {
public:
	GameObj();
	virtual ~GameObj();
	virtual B2Sprite * load(const char * name , float ratio , b2BodyType type , b2World * world);
    virtual void setPosition(const b2Vec2& position);
    virtual void setVelocity(const b2Vec2& velocity);
    virtual void setSpeed(float speed);

protected:
	B2Sprite * gameObj;
    float speed;
};

#endif /* GAMEOBJ_H_ */
