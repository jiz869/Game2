#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <vector>
#include "cocos2d.h"
using namespace cocos2d;

using namespace std;

typedef enum{
    OBJ_INVALID,
    OBJ_LOADED,
    OBJ_ACTIVE,
    OBJ_INACTIVE
}ObjectState;

typedef enum{
    CAR_INVALID,    // not a car object
    CAR_NORMAL,
    CAR_SPEED_UP_PLAYER,
    CAR_DUP_PLAYER
}CarType;

class GObject : public CCNode {
    // various cars
    void LoadCar1();
    void LoadCar2();
    void LoadSpeedUpCar();
protected:
    CCSpriteBatchNode *batchNode;
    CCTexture2D *texture;

    void LoadTiles(int nc, int nr, float w, float h,
            char *fn, vector<CCPoint> &tile_xy, bool flipY);

public:
    GObject() : state(OBJ_INVALID)
    {
        velocity = ccp(0.0, 0.0);
    }

    virtual ~GObject();

    virtual void Load(char *name);

    virtual void SetObjectPosition(float x, float y);

    virtual CCNode* Node();

    virtual void Step(float dt);

    virtual void GetAABB(CCPoint &o, float &w, float &h);

    virtual void SetVelocity( CCPoint v) {
    	velocity = v;
    }

    bool InScreen(float w, float h);

    ObjectState state;
    float width;
    float height;
    char objName[50];
    CCPoint velocity;
    CarType carType;
};

#endif





