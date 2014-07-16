/*
 * SpecialObj.h
 *
 *  Created on: May 17, 2014
 *      Author: welcomelm
 */

#ifndef SPECIALOBJ_H_
#define SPECIALOBJ_H_

#include "CarObj.h"

class SpecialObj: public CarObj {
public:
	SpecialObj();
	virtual ~SpecialObj();
    virtual B2Sprite * load(bool left2right , CCPoint initPos , float speed , Lane * lane , vector<int> * carNumbers);
    virtual B2Sprite * load(CCPoint initPos);
    virtual B2Sprite * load(int specialId);
    virtual void begin(PlayerObj * player);
    virtual void step(PlayerObj * player);
    virtual void end(PlayerObj * player);
    virtual SpecialData * getSpecialData();
    virtual int getSpecialId();
    virtual bool hitByCar(PlayerObj * player , CCSprite * car);
    virtual void take();
    virtual bool isTaken();
    virtual void expire();
    virtual bool isExpired();
    virtual void unTake();

protected:
    int specialId;
    SpecialData * specialData;
    float timer_count;
    bool taken;
    bool expired;
};

#endif /* SPECIALOBJ_H_ */
