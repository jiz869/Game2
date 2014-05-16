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
    void JumpUp();
    void JumpDown();
    void Wait();
    
protected:
    PlayerState movingState;
};

#endif /* defined(__crossRoadMain__PlayObj__) */
