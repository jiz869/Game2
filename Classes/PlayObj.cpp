//
//  PlayObj.cpp
//  crossRoadMain
//
//  Created by Min Li on 2014-05-15.
//
//

#include "PlayObj.h"

void PlayerObj::Wait()
{
    gameObj->stopAllActions();
//    CCAnimate *aa = CCAnimate::create(animationWait);
//    CCRepeatForever *rep = CCRepeatForever::create(aa);
//    sprite->runAction(rep);
    movingState = WAIT;
    setVelocity(b2Vec2(0 , 0));
    //CCLog("set player state WAIT");
}

//Customize
void PlayerObj::JumpUp()
{
    if( movingState != JMP_UP ) {
        gameObj->stopAllActions();
//        CCAnimate *aa = CCAnimate::create(animationJumpUp);
//        CCRepeatForever *rep = CCRepeatForever::create(aa);
//        sprite->runAction(rep);
        movingState = JMP_UP;
        setVelocity(b2Vec2(0 , speed));
        //CCLog("set player state JMP");
    }
}

void PlayerObj::JumpDown()
{
    if (gameObj->getPosition().y <= 0) {
        return;
    }
    
    if( movingState != JMP_DOWN ) {
        gameObj->stopAllActions();
        //        CCAnimate *aa = CCAnimate::create(animationJumpUp);
        //        CCRepeatForever *rep = CCRepeatForever::create(aa);
        //        sprite->runAction(rep);
        movingState = JMP_DOWN;
        setVelocity(b2Vec2(0 , -speed));
        //CCLog("set player state JMP_DOWN");
    }
}