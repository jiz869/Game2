/*
 * CarObj.h
 *
 *  Created on: May 17, 2014
 *      Author: welcomelm
 */

#ifndef CAROBJ_H_
#define CAROBJ_H_

#include "GameObj.h"
#include "Lane.h"

class CarObj: public GameObj {
public:
	CarObj();
	virtual ~CarObj();
    virtual B2Sprite * load(bool left2right , CCPoint initPos , float speed , Lane * lane);
    virtual B2Sprite * load(const char * name , bool left2right , CCPoint initPos , float speed , Lane * lane , SpriteTag tag);
    Lane * lane;
    
protected:
};

#endif /* CAROBJ_H_ */
