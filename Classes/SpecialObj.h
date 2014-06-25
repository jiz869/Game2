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
    virtual B2Sprite * load(bool left2right , CCPoint initPos , float speed , Lane * lane);
    virtual void begin(PlayerObj * player);
    virtual void step(PlayerObj * player);
    virtual void end(PlayerObj * player);
    virtual SpecialData * getSpecialData();
    virtual int getSpecialId();
    virtual bool hitByCar(PlayerObj * player , CCSprite * car);

protected:
    int specialId;
    SpecialData * specialData;
    float timer_count;
};

#endif /* SPECIALOBJ_H_ */
