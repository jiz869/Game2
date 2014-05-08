#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "cocos2d.h"
using namespace cocos2d;

typedef enum{
    OBJ_INVALID,
    OBJ_LOADED,
    OBJ_ACTIVE,
    OBJ_INACTIVE
}ObjectState;

class GObject : public CCNode {
public:
    GObject() : state(OBJ_INVALID) 
    {
        velocity = ccp(0.0, 0.0);
    }

    virtual ~GObject(){
    }

    virtual void Load(char *name) {
    }

    virtual void SetObjectPosition(float x, float y) {
    }

    virtual CCNode* Node() {
        return NULL;
    }

    virtual void Step(float dt) {
    }

    virtual void GetAABB(CCPoint &o, float &w, float &h) {
    }

    virtual void SetVelocity( CCPoint v) {
    	velocity = v;
    }

    ObjectState state;
    float width;
    float height;
    char objName[50];
    CCPoint velocity;
};

#endif





