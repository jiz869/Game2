//
//  PlayObj.h
//  crossRoadMain
//
//  Created by Min Li on 2014-05-15.
//
//

#ifndef __crossRoadMain__PlayObj__
#define __crossRoadMain__PlayObj__

#include "GameObj.h"

class PlayerObj : public GameObj {
    enum PlayerState {
        WAIT,
        JMP_UP,
        JMP_DOWN
    };

public:
    PlayerObj();
    ~PlayerObj();
    virtual B2Sprite * load(const char * name , b2BodyType type , SpriteTag tag);
    void jumpUp();
    void jumpDown();
    void wait();
    void reset();
    void resetNextUpdate(bool shouldReset);
    void step(float dt);

protected:
    PlayerState movingState;
    bool shouldReset;
    CCString * waitFrameName;
};

#endif /* defined(__crossRoadMain__PlayObj__) */
