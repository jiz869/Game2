/*
 * Lane.h
 *
 *  Created on: May 16, 2014
 *      Author: mli
 */

#ifndef LANE_H_
#define LANE_H_

#include "GameObj.h"
#include "GameController.h"

class Lane: public cocos2d::CCLayer {
public:
	Lane();
	virtual ~Lane();

	static Lane * creatWithDescription(LaneDescription * description);
	virtual bool initWithDescription(LaneDescription * description);

private:

	LaneDescription * description;
	void addACar(float dt);
};

#endif /* LANE_H_ */
