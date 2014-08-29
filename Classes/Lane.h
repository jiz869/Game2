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
    virtual void stopAtPosition(float x , float playerSize);
    virtual void stop();
    virtual void reStart();
    void step(float dt);
    void destroyLastCar();
	void destroyMe(cocos2d::CCNode *car);
    void setSpecialChance(float chance);
    void resetSpecialChance();
    void allSmallCars();
    void resumeCarNumbers();
    void speedUp(float percent);
    void resumeSpeed();

private:
	LaneDescription * description;
	void addACar1(float dt);
	void addACar2(float dt);
    LaneStatus status;
    float timePassedFromLastSchedule;
    PlaySceneData * playSceneData;
    float specialChance;
    vector<int> carNumbers;
    float speed;
    float period;
};

int getRandom(int low, int high);

#endif /* LANE_H_ */
