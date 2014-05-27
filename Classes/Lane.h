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
    virtual void slow(float speed_decrease , float interval_increase);
    virtual void resumeFromSlow(float speed_decrease , float interval_increase);
    void step(float dt);

private:
	LaneDescription * description;
	void addACar(float dt);
    bool isSpecialCar(float chance);
    void addRoad();
    LaneStatus status;
    float timePassedFromLastSchedule;
};

int getRandom(int low, int high);

#endif /* LANE_H_ */
