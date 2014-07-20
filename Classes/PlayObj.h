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

#define MAX_SPECIAL_NUM 3
#define MAX_BAD_SPECIAL_NUM 3

class PlayerObj : public GameObj {

public:
    typedef enum{
        MIDDLE=0,
        LEFT,
        RIGHT,
    }PlayerOrder;

public:
    PlayerObj();
    ~PlayerObj();
    virtual B2Sprite * load(PlayerOrder order);
    void jumpUp();
    void jumpDown();
    void wait();
    void reset();
    void step(float dt);
    bool processContact(CCSprite * contact);
    SpecialObj * hasSpecial(int specialId);
    bool enoughSpecials(int specialId);
    void beginWithSpecial(SpecialObj * specialObj);
    void endWithSpecial(SpecialObj * specialObj);
    void removeSpecial(SpecialObj * specialObj);
    void speedUp(float delta);
    void slowDown(float delta);
    void tagPlayer(SpecialObj * specialObj);
    CCNode * getParent();
    void removeAllSpecials();
    void removeAllBadSpecials();
    CCPoint resetPos;
    void setTag(int tag);
    void freeze();
    void unfreeze();
    void changeAcc(float up_delta , float down_delta);
    void resumeAcc();
    void hitByCar(bool playDefaultSound = true);

protected:
    PlayerState movingState;
    vector<SpecialObj *> specials;
    vector<SpecialObj *> badSpecials;
    PlaySceneData * data;
    CCPoint velocity;
    CCPoint acc;	//acceleration
    float playerAccSpeed;		//starting acceleration
    float playerStopAccSpeed;	//stopping acceleration
    bool frozen;
};

#endif /* defined(__crossRoadMain__PlayObj__) */
