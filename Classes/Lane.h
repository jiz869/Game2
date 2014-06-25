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

extern bool toss(float);

typedef enum{
	RUNNING = 0x1 << 0,
	STOPPED = 0x1 << 1,
}LaneStatus;

class Lane: public cocos2d::CCLayer {
public:
	Lane();
	virtual ~Lane();

	static Lane * creatWithDescription(LaneDescription * description);
	virtual bool initWithDescription(LaneDescription * description);
    virtual void stopAtPosition(float x);
    virtual void stop();
    virtual void reStart();
    void step(float dt);
    void destroyLastCar(CCObject * obj);

private:
	LaneDescription * description;
	void addACar1(float dt);
	void addACar2(float dt);
    LaneStatus status;
    float timePassedFromLastSchedule;
};

int getRandom(int low, int high);

#endif /* LANE_H_ */
