#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.h"
#include "cocos2d.h"
using namespace cocos2d;

const float JMP_Y_SPEED = 3.0;
const float GRAVITY_Y = -0.4;
const CCPoint INIT_POS = ccp(240, 26);
const float SHORT_JMP_SPEED = 3.0;
const float INIT_GAME_SPEED = 3.0;

class GPlayer : public CCObject
{
    CCTexture2D *playerTexture;
    CCAnimation *animationWait;
    CCAnimation *animationJumpUp;
    CCAnimation *animationJumpDown;
    float animRunRate;

    float width;    // width of single frame in sprite sheet
    float height;
    CCSprite *sprite;

    //physics
    CCPoint velocity;
    CCSize designSize;

public:
    enum ePlayerState {
        WAIT,
        JMP_UP,
        JMP_DOWN
    };
    GPlayer();
    ~GPlayer();

    CCSprite* CreatePlayerSprite();

    //move/control
    void SetPlayerPosition(float x, float y);

    CCPoint GetPlayerPosition() {
    	return sprite->getPosition();
    }

    CCPoint GetPlayerVelocity() {
        return velocity;
    }
    void SetPlayerVelocity(float x, float y) {
        velocity = ccp(x, y);
    }

    void JumpUp();
    void JumpDown();
    void Wait();

    void Step(float);

    //collision detection
    void GetAABB(CCPoint &o, float &w, float &h);

    //data
    ePlayerState state;

    //player collision test
    bool RightTopTest(GObject *obj);
    bool RightBottomTest(GObject *obj);
    bool CheckObjectCollision(GObject *obj);

    //        t
    //        |
    // l------|------r
    //        |
    //        b
    bool SegmentsTest(CCPoint b, CCPoint t, CCPoint l, CCPoint r);
};

#endif

