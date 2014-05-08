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

class GObject : public CCNode {
    void LoadCar1();
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

    ObjectState state;
    float width;
    float height;
    char objName[50];
    CCPoint velocity;
};

#endif





