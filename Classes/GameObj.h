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
#include "GameController.h"
USING_NS_CC;

typedef enum{
	PLAYER = 0,
	LOWER_BOUNDARY,
	UPPER_BOUNDARY,
	CAR,
    SPECIAL,
    ENEMY,
}SpriteTag;

class GameObj {
public:
	GameObj();
	virtual ~GameObj();
	virtual B2Sprite * load(const char * name , b2BodyType type , const CCSize * size , SpriteTag tag);
	virtual B2Sprite * load(b2BodyType type , const CCSize * size , SpriteTag tag);
	virtual B2Sprite * load(const char * name , b2BodyType type , SpriteTag tag);
    virtual void setPosition(const CCPoint& position);
    virtual const CCPoint& getPosition();
    virtual void setVelocity(b2Vec2 velocity);
    virtual const b2Vec2 getVelocity();
    static void setB2world(b2World * world , float ptmRatio);
    virtual const CCSize& getSize();
    virtual bool isBottom();
    virtual void runAction(CCAction* action);

protected:
	B2Sprite * gameObj;

    static b2World * world;
    static float ptmRatio;
};

#endif /* GAMEOBJ_H_ */
