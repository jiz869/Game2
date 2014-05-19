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
#include "SpecialObj.h"

enum PlayerState {
    WAIT,
    JMP_UP,
    JMP_DOWN
};

#define MAX_SPECIAL_NUM 2

class PlayerObj : public GameObj {

public:
    PlayerObj();
    ~PlayerObj();
    virtual B2Sprite * load();
    void jumpUp();
    void jumpDown();
    void wait();
    void reset();
    void step(float dt);
    void processContact(CCSprite * contact);
    bool hasSpecial(SpecialObj * specialObj);
    bool enoughSpecials();
    void contactWithSpecial(SpecialObj * specialObj);
    void endWithSpecial(SpecialObj * specialObj);
    void removeSpecial(SpecialObj * specialObj);
    void speedUp(float delta);
    void slowDown(float delta);
    void tagPlayer(SpecialObj * specialObj);

protected:
    PlayerState movingState;
    vector<SpecialObj *> specials;
};

#endif /* defined(__crossRoadMain__PlayObj__) */
